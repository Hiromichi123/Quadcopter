#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>

#include <geometry_msgs/msg/twist_stamped.hpp>
#include <mavros_msgs/msg/state.hpp>
#include <mavros_msgs/srv/command_bool.hpp>
#include <mavros_msgs/srv/set_mode.hpp>
#include <rclcpp/rclcpp.hpp>

#include "ros2_tools/msg/lidar_pose.hpp"

namespace {

float shortest_angular_error(float target_yaw, float current_yaw)
{
    return std::remainder(target_yaw - current_yaw, 2.0f * static_cast<float>(M_PI));
}

float positive_yaw_delta(float current_yaw, float start_yaw)
{
    const float delta = std::remainder(current_yaw - start_yaw, 2.0f * static_cast<float>(M_PI));
    return delta < 0.0f ? delta + 2.0f * static_cast<float>(M_PI) : delta;
}

}  // namespace

class DiffCarTestMission : public rclcpp::Node {
public:
    DiffCarTestMission()
        : Node("diff_car_test_mission")
    {
        cmd_vel_topic_ = declare_parameter<std::string>(
            "cmd_vel_topic", "/mavros/setpoint_velocity/cmd_vel");
        lidar_topic_ = declare_parameter<std::string>("lidar_topic", "lidar_data");
        enable_px4_preflight_ = declare_parameter<bool>("enable_px4_preflight", true);
        offboard_mode_ = declare_parameter<std::string>("offboard_mode", "OFFBOARD");
        target_distance_m_ = static_cast<float>(declare_parameter<double>("target_distance_m", 1.0));
        target_turn_rad_ = static_cast<float>(declare_parameter<double>(
            "target_turn_rad", M_PI / 2.0));
        forward_speed_mps_ = static_cast<float>(declare_parameter<double>("forward_speed_mps", 0.25));
        turn_rate_radps_ = static_cast<float>(declare_parameter<double>("turn_rate_radps", 0.45));
        yaw_hold_kp_ = static_cast<float>(declare_parameter<double>("yaw_hold_kp", 1.2));
        max_yaw_correction_radps_ = static_cast<float>(declare_parameter<double>(
            "max_yaw_correction_radps", 0.35));
        distance_tolerance_m_ = static_cast<float>(declare_parameter<double>(
            "distance_tolerance_m", 0.03));
        yaw_tolerance_rad_ = static_cast<float>(declare_parameter<double>(
            "yaw_tolerance_rad", 0.04));

        lidar_sub_ = create_subscription<ros2_tools::msg::LidarPose>(
            lidar_topic_,
            10,
            [this](const ros2_tools::msg::LidarPose::SharedPtr msg) {
                x_ = msg->x;
                y_ = msg->y;
                yaw_ = msg->yaw;
                has_pose_ = true;
            });
        mavros_state_sub_ = create_subscription<mavros_msgs::msg::State>(
            "/mavros/state",
            10,
            [this](const mavros_msgs::msg::State::SharedPtr msg) {
                mavros_state_ = *msg;
                has_mavros_state_ = true;
            });

        cmd_pub_ = create_publisher<geometry_msgs::msg::TwistStamped>(cmd_vel_topic_, 10);
        service_callback_group_ = create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        arming_client_ = create_client<mavros_msgs::srv::CommandBool>(
            "/mavros/cmd/arming",
            rmw_qos_profile_services_default,
            service_callback_group_);
        set_mode_client_ = create_client<mavros_msgs::srv::SetMode>(
            "/mavros/set_mode",
            rmw_qos_profile_services_default,
            service_callback_group_);
        timer_ = create_wall_timer(
            std::chrono::milliseconds(50),
            [this]() { on_timer(); });

        RCLCPP_INFO(
            get_logger(),
            "Diff car test mission ready: forward %.2f m, left turn %.1f deg, forward %.2f m",
            target_distance_m_,
            target_turn_rad_ * 180.0f / static_cast<float>(M_PI),
            target_distance_m_);
    }

private:
    enum class State {
        WAIT_POSE,
        FORWARD_1,
        TURN_LEFT_90,
        FORWARD_2,
        DONE
    };

    void on_timer()
    {
        if (!handle_preflight()) {
            publish_stop();
            return;
        }

        switch (state_) {
            case State::WAIT_POSE:
                start_segment(State::FORWARD_1, "FORWARD_1");
                break;
            case State::FORWARD_1:
                handle_forward(State::TURN_LEFT_90, "TURN_LEFT_90");
                break;
            case State::TURN_LEFT_90:
                handle_turn_left();
                break;
            case State::FORWARD_2:
                handle_forward(State::DONE, "DONE");
                break;
            case State::DONE:
                publish_stop();
                RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 2000, "Mission done, sending stop");
                break;
        }
    }

    bool handle_preflight()
    {
        if (!has_pose_) {
            RCLCPP_WARN_THROTTLE(
                get_logger(),
                *get_clock(),
                2000,
                "[PreFlight] Waiting for pose topic '%s'",
                lidar_topic_.c_str());
            return false;
        }

        if (!enable_px4_preflight_ || preflight_done_) {
            preflight_done_ = true;
            return true;
        }

        publish_stop();

        if (!has_mavros_state_ || !mavros_state_.connected) {
            RCLCPP_WARN_THROTTLE(
                get_logger(),
                *get_clock(),
                2000,
                "[PreFlight] Waiting for FCU connection on /mavros/state");
            return false;
        }

        RCLCPP_INFO_THROTTLE(
            get_logger(),
            *get_clock(),
            2000,
            "[PreFlight] state: mode='%s', armed=%d, connected=%d, has_pose=%d",
            mavros_state_.mode.c_str(),
            mavros_state_.armed,
            mavros_state_.connected,
            has_pose_);

        const bool request_timeout = (now() - last_request_time_) > rclcpp::Duration::from_seconds(1.0);
        if (mavros_state_.mode != offboard_mode_ && request_timeout) {
            const bool success = request_set_mode(offboard_mode_);
            RCLCPP_INFO(
                get_logger(),
                "[PreFlight] request %s mode... %s",
                offboard_mode_.c_str(),
                success ? "FCU accepted" : "FCU rejected/timeout");
            last_request_time_ = now();
            return false;
        }

        if (!mavros_state_.armed && mavros_state_.mode == offboard_mode_ && request_timeout) {
            const bool success = request_arm(true);
            RCLCPP_INFO(
                get_logger(),
                "[PreFlight] arming... %s",
                success ? "FCU accepted" : "FCU rejected/timeout");
            last_request_time_ = now();
            return false;
        }

        if (mavros_state_.armed && mavros_state_.mode == offboard_mode_) {
            preflight_done_ = true;
            RCLCPP_INFO(get_logger(), "[PreFlight] Armed + %s ready", offboard_mode_.c_str());
            return true;
        }

        return false;
    }

    bool request_arm(bool arm)
    {
        if (!arming_client_->wait_for_service(std::chrono::milliseconds(200))) {
            RCLCPP_WARN(get_logger(), "[PreFlight] /mavros/cmd/arming service not ready");
            return false;
        }

        auto req = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
        req->value = arm;
        auto future = arming_client_->async_send_request(req);
        if (future.wait_for(std::chrono::milliseconds(800)) != std::future_status::ready) {
            return false;
        }
        const auto resp = future.get();
        return resp && resp->success;
    }

    bool request_set_mode(const std::string& mode)
    {
        if (!set_mode_client_->wait_for_service(std::chrono::milliseconds(200))) {
            RCLCPP_WARN(get_logger(), "[PreFlight] /mavros/set_mode service not ready");
            return false;
        }

        auto req = std::make_shared<mavros_msgs::srv::SetMode::Request>();
        req->custom_mode = mode;
        auto future = set_mode_client_->async_send_request(req);
        if (future.wait_for(std::chrono::milliseconds(800)) != std::future_status::ready) {
            return false;
        }
        const auto resp = future.get();
        return resp && resp->mode_sent;
    }

    void start_segment(State next_state, const char * state_name)
    {
        state_ = next_state;
        segment_start_x_ = x_;
        segment_start_y_ = y_;
        segment_start_yaw_ = yaw_;
        RCLCPP_INFO(
            get_logger(),
            "Start %s at x=%.2f y=%.2f yaw=%.2f rad",
            state_name,
            segment_start_x_,
            segment_start_y_,
            segment_start_yaw_);
    }

    void handle_forward(State next_state, const char * next_state_name)
    {
        const float distance = std::hypot(x_ - segment_start_x_, y_ - segment_start_y_);
        if (distance >= target_distance_m_ - distance_tolerance_m_) {
            publish_stop();
            RCLCPP_INFO(get_logger(), "Forward segment finished: %.2f m", distance);
            start_segment(next_state, next_state_name);
            return;
        }

        const float yaw_error = shortest_angular_error(segment_start_yaw_, yaw_);
        const float yaw_cmd = std::clamp(
            yaw_error * yaw_hold_kp_,
            -max_yaw_correction_radps_,
            max_yaw_correction_radps_);
        publish_cmd(forward_speed_mps_, yaw_cmd);

        RCLCPP_INFO_THROTTLE(
            get_logger(),
            *get_clock(),
            1000,
            "Forward: %.2f / %.2f m, yaw_error=%.3f rad",
            distance,
            target_distance_m_,
            yaw_error);
    }

    void handle_turn_left()
    {
        const float turned = positive_yaw_delta(yaw_, segment_start_yaw_);
        const float remaining = target_turn_rad_ - turned;
        if (remaining <= yaw_tolerance_rad_) {
            publish_stop();
            RCLCPP_INFO(
                get_logger(),
                "Left turn finished: %.1f deg",
                turned * 180.0f / static_cast<float>(M_PI));
            start_segment(State::FORWARD_2, "FORWARD_2");
            return;
        }

        const float turn_cmd = std::min(turn_rate_radps_, std::max(0.18f, remaining * 1.2f));
        publish_cmd(0.0f, turn_cmd);

        RCLCPP_INFO_THROTTLE(
            get_logger(),
            *get_clock(),
            1000,
            "Turn left: %.1f / %.1f deg",
            turned * 180.0f / static_cast<float>(M_PI),
            target_turn_rad_ * 180.0f / static_cast<float>(M_PI));
    }

    void publish_cmd(float linear_x, float angular_z)
    {
        geometry_msgs::msg::TwistStamped cmd;
        cmd.header.stamp = now();
        cmd.header.frame_id = "base_link";
        cmd.twist.linear.x = linear_x;
        cmd.twist.angular.z = angular_z;
        cmd_pub_->publish(cmd);
    }

    void publish_stop()
    {
        publish_cmd(0.0f, 0.0f);
    }

    State state_{State::WAIT_POSE};
    bool enable_px4_preflight_{true};
    bool preflight_done_{false};
    bool has_pose_{false};
    bool has_mavros_state_{false};
    float x_{0.0f};
    float y_{0.0f};
    float yaw_{0.0f};
    float segment_start_x_{0.0f};
    float segment_start_y_{0.0f};
    float segment_start_yaw_{0.0f};

    std::string cmd_vel_topic_;
    std::string lidar_topic_;
    std::string offboard_mode_{"OFFBOARD"};
    float target_distance_m_{1.0f};
    float target_turn_rad_{static_cast<float>(M_PI / 2.0)};
    float forward_speed_mps_{0.25f};
    float turn_rate_radps_{0.45f};
    float yaw_hold_kp_{1.2f};
    float max_yaw_correction_radps_{0.35f};
    float distance_tolerance_m_{0.03f};
    float yaw_tolerance_rad_{0.04f};

    rclcpp::Subscription<ros2_tools::msg::LidarPose>::SharedPtr lidar_sub_;
    rclcpp::Subscription<mavros_msgs::msg::State>::SharedPtr mavros_state_sub_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_pub_;
    rclcpp::Client<mavros_msgs::srv::CommandBool>::SharedPtr arming_client_;
    rclcpp::Client<mavros_msgs::srv::SetMode>::SharedPtr set_mode_client_;
    rclcpp::CallbackGroup::SharedPtr service_callback_group_;
    rclcpp::TimerBase::SharedPtr timer_;
    mavros_msgs::msg::State mavros_state_{};
    rclcpp::Time last_request_time_{0, 0, RCL_ROS_TIME};
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DiffCarTestMission>();
    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 2);
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}
