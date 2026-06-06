#include <rclcpp/rclcpp.hpp>
#include <messages/msg/smart_car_control_setpoint.hpp>
#include <messages/msg/smart_car_motor_status.hpp>
#include <messages/msg/smart_car_motion_state.hpp>
#include <messages/msg/smart_car_status.hpp>
#include <messages/srv/smart_car_command.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

/**
 * @brief 最大曲率转圈测试程序
 * 
 * 以最大曲率(κ=2.5 1/m)进行圆形行驶
 * 转弯半径: R = 1/κ = 1/2.5 = 0.4m
 * 
 * 控制流程：
 * 1. 加速阶段：速度从0线性增加到目标速度
 * 2. 稳定阶段：以目标速度+最大曲率转圈
 * 3. 可按Ctrl+C停止
 */
class TestMaxCurvatureNode : public rclcpp::Node {
public:
    TestMaxCurvatureNode()
        : rclcpp::Node("test_max_curvature")
    {
        // 参数声明
        target_speed_ = declare_parameter<double>("target_speed", 0.5);
        accel_time_ = declare_parameter<double>("accel_time", 3.0);
        target_curvature_ = declare_parameter<double>("target_curvature", kMaxCurvature_);
        
        RCLCPP_INFO(get_logger(), "=== Max Curvature Test ===");
        RCLCPP_INFO(get_logger(), "Target speed: %.2f m/s", target_speed_);
        RCLCPP_INFO(get_logger(), "Acceleration time: %.2f s", accel_time_);
        RCLCPP_INFO(get_logger(), "Target curvature: %.2f 1/m", target_curvature_);
        if (std::abs(target_curvature_) > 1.0e-4) {
            RCLCPP_INFO(get_logger(), "Turn radius: %.2f m (R=1/κ)", 1.0 / std::abs(target_curvature_));
        }
        
        // 订阅运动反馈
        motion_sub_ = create_subscription<messages::msg::SmartCarMotionState>(
            "/smart_car/motion_state",
            10,
            [this](const messages::msg::SmartCarMotionState::SharedPtr msg) {
                last_motion_time_ = now();
                has_motion_ = true;
                current_speed_ = msg->speed_mps;
                accepted_target_speed_ = msg->target_speed_mps;
                curvature_cmd_ = msg->curvature_cmd;
                steering_angle_deg_ = msg->steering_angle_deg;
            });

        motor_sub_ = create_subscription<messages::msg::SmartCarMotorStatus>(
            "/smart_car/motor_status",
            10,
            [this](const messages::msg::SmartCarMotorStatus::SharedPtr msg) {
                last_motor_time_ = now();
                has_motor_ = true;
                target_rpm_1_ = msg->target_rpm_1;
                target_rpm_2_ = msg->target_rpm_2;
                actual_rpm_1_ = msg->actual_rpm_1;
                actual_rpm_2_ = msg->actual_rpm_2;
                current_cmd_1_ = msg->current_cmd_1;
                current_cmd_2_ = msg->current_cmd_2;
                feedback_current_1_ = msg->feedback_current_1;
                feedback_current_2_ = msg->feedback_current_2;
            });

        status_sub_ = create_subscription<messages::msg::SmartCarStatus>(
            "/smart_car/status",
            10,
            [this](const messages::msg::SmartCarStatus::SharedPtr msg) {
                last_status_time_ = now();
                has_status_ = true;
                fault_flags_ = msg->fault_flags;
                state_ = msg->state;
                host_online_ = msg->host_online;
                cmd_age_ms_ = msg->cmd_age_ms;
                motor_online_mask_ = msg->motor_online_mask;
            });

        // 发布控制命令
        control_pub_ = create_publisher<messages::msg::SmartCarControlSetpoint>(
            "/smart_car/control_setpoint",
            10);

        command_client_ = create_client<messages::srv::SmartCarCommand>("/smart_car/command");

        arm_timer_ = create_wall_timer(
            std::chrono::milliseconds(100),
            [this]() { on_arm_timer(); });

        // 控制定时器 (20 Hz)
        control_timer_ = create_wall_timer(
            std::chrono::milliseconds(50),
            [this]() { on_control_timer(); });

        start_time_ = now();
        RCLCPP_INFO(get_logger(), "Waiting for smart car bridge command service...");
    }

    ~TestMaxCurvatureNode() override
    {
        // 停止小车
        send_control_command(0.0f, 0.0f);
        RCLCPP_INFO(get_logger(), "Test stopped. Car stopped.");
    }

private:
    static constexpr float kMaxCurvature_ = 2.5f;  // 最大曲率 (1/m)

    double target_speed_{0.5};                     // 目标速度 (m/s)
    double accel_time_{3.0};                       // 加速时间 (s)
    double target_curvature_{kMaxCurvature_};      // 目标曲率 (1/m)
    float current_speed_{0.0f};                    // 当前速度 (m/s)
    float accepted_target_speed_{0.0f};
    float curvature_cmd_{0.0f};
    float steering_angle_deg_{0.0f};
    int16_t target_rpm_1_{0};
    int16_t target_rpm_2_{0};
    int16_t actual_rpm_1_{0};
    int16_t actual_rpm_2_{0};
    int16_t current_cmd_1_{0};
    int16_t current_cmd_2_{0};
    int16_t feedback_current_1_{0};
    int16_t feedback_current_2_{0};
    uint32_t fault_flags_{0};
    uint8_t state_{0};
    uint8_t host_online_{0};
    uint16_t cmd_age_ms_{0};
    uint8_t motor_online_mask_{0};
    bool armed_{false};
    bool has_motion_{false};
    bool has_motor_{false};
    bool has_status_{false};
    
    rclcpp::Time start_time_;
    rclcpp::Time last_motion_time_;
    rclcpp::Time last_motor_time_;
    rclcpp::Time last_status_time_;

    rclcpp::Subscription<messages::msg::SmartCarMotionState>::SharedPtr motion_sub_;
    rclcpp::Subscription<messages::msg::SmartCarMotorStatus>::SharedPtr motor_sub_;
    rclcpp::Subscription<messages::msg::SmartCarStatus>::SharedPtr status_sub_;
    rclcpp::Publisher<messages::msg::SmartCarControlSetpoint>::SharedPtr control_pub_;
    rclcpp::Client<messages::srv::SmartCarCommand>::SharedPtr command_client_;
    rclcpp::TimerBase::SharedPtr arm_timer_;
    rclcpp::TimerBase::SharedPtr control_timer_;

    void on_arm_timer()
    {
        if (!command_client_->service_is_ready()) {
            RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000,
                                 "Waiting for /smart_car/command service...");
            return;
        }

        send_command(messages::srv::SmartCarCommand::Request::SMART_CAR_COMMAND_CLEAR_FAULTS);
        send_command(messages::srv::SmartCarCommand::Request::SMART_CAR_COMMAND_ENABLE);
        start_time_ = now();
        armed_ = true;
        arm_timer_->cancel();
        RCLCPP_INFO(get_logger(), "Smart car enabled. Test started. Press Ctrl+C to stop.");
    }

    void on_control_timer()
    {
        if (!armed_) {
            send_control_command(0.0f, 0.0f);
            return;
        }

        const auto elapsed = (now() - start_time_).seconds();
        
        // 加速阶段
        float target_speed;
        if (elapsed < accel_time_) {
            // 线性加速
            target_speed = static_cast<float>(target_speed_ * elapsed / accel_time_);
        } else {
            // 稳定阶段
            target_speed = static_cast<float>(target_speed_);
        }

        // 发送控制命令：以最大曲率转圈
        send_control_command(target_speed, static_cast<float>(target_curvature_));

        // 定期打印状态
        static int log_count = 0;
        if (++log_count % 20 == 0) {  // 每1秒打印一次 (20 * 50ms = 1s)
            const double motion_age =
                has_motion_ ? (now() - last_motion_time_).seconds() : -1.0;
            const double motor_age =
                has_motor_ ? (now() - last_motor_time_).seconds() : -1.0;
            const double status_age =
                has_status_ ? (now() - last_status_time_).seconds() : -1.0;
            RCLCPP_INFO(get_logger(),
                        "t=%.2fs send speed=%.3f curv=%.2f | motion_age=%.2fs motor_age=%.2fs status_age=%.2fs car speed=%.3f target_speed=%.3f curv_cmd=%.2f steer=%.1fdeg rpm_tgt=(%d,%d) rpm_act=(%d,%d) curr_cmd=(%d,%d) curr_fb=(%d,%d) state=%u faults=0x%08x host=%u cmd_age=%u motor_mask=0x%02x",
                        elapsed,
                        target_speed,
                        target_curvature_,
                        motion_age,
                        motor_age,
                        status_age,
                        current_speed_,
                        accepted_target_speed_,
                        curvature_cmd_,
                        steering_angle_deg_,
                        target_rpm_1_,
                        target_rpm_2_,
                        actual_rpm_1_,
                        actual_rpm_2_,
                        current_cmd_1_,
                        current_cmd_2_,
                        feedback_current_1_,
                        feedback_current_2_,
                        state_,
                        fault_flags_,
                        host_online_,
                        cmd_age_ms_,
                        motor_online_mask_);
        }
    }

    void send_control_command(float speed, float curvature)
    {
        auto msg = std::make_shared<messages::msg::SmartCarControlSetpoint>();
        msg->mode = messages::msg::SmartCarControlSetpoint::SMART_CAR_MODE_MANUAL;
        msg->flags = messages::msg::SmartCarControlSetpoint::SMART_CAR_CONTROL_FLAG_ENABLE;
        msg->target_speed_mps = speed;
        msg->target_curvature = curvature;
        msg->target_yaw_rate_dps = 0.0f;
        msg->target_accel_mps2 = 0.0f;

        control_pub_->publish(*msg);
    }

    void send_command(uint16_t command)
    {
        auto request = std::make_shared<messages::srv::SmartCarCommand::Request>();
        request->command = command;
        request->param1 = 0.0f;
        request->param2 = 0.0f;
        request->param3 = 0.0f;
        request->param4 = 0.0f;
        command_client_->async_send_request(request);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TestMaxCurvatureNode>());
    rclcpp::shutdown();
    return 0;
}
