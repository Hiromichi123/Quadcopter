#include "layer3_mission/car_mission_executor.hpp"
#include <messages/msg/smart_car_control_setpoint.hpp>
#include <messages/msg/smart_car_motion_state.hpp>
#include <chrono>
#include <cmath>

namespace car_mission {

CarMissionExecutor::CarMissionExecutor()
    : rclcpp::Node("car_mission_executor")
{
    RCLCPP_INFO(logger_, "Initializing Car Mission Executor");
    RCLCPP_INFO(logger_, "Mission: Clockwise oval track (3 circles)");
    RCLCPP_INFO(logger_, "  - Straight: %.2f m", kStraightLength);
    RCLCPP_INFO(logger_, "  - Curve radius: %.2f m (κ=%.2f 1/m)", kCurveRadius, kCurveKappa);
    RCLCPP_INFO(logger_, "  - Target speed: %.2f m/s", kTargetSpeed);

    // 订阅运动状态
    motion_sub_ = create_subscription<messages::msg::SmartCarMotionState>(
        "/smart_car/motion_state",
        10,
        [this](const messages::msg::SmartCarMotionState::SharedPtr msg) {
            current_speed_ = msg->speed_mps;
        });

    // 发布控制指令
    control_pub_ = create_publisher<messages::msg::SmartCarControlSetpoint>(
        "/smart_car/control_setpoint",
        10);

    // 创建定时器 (20 Hz)
    mission_timer_ = create_wall_timer(
        std::chrono::milliseconds(50),
        std::bind(&CarMissionExecutor::on_mission_timer, this));

    current_state_ = MissionState::INIT;
    segment_start_time_ = now();

    RCLCPP_INFO(logger_, "Car Mission Executor ready. Starting mission...");
}

void CarMissionExecutor::on_mission_timer()
{
    switch (current_state_) {
        case MissionState::INIT:
            RCLCPP_INFO(logger_, "Mission started: Circle 1/3");
            transition_state(MissionState::STRAIGHT_1);
            break;

        case MissionState::STRAIGHT_1:
        case MissionState::STRAIGHT_2:
        case MissionState::STRAIGHT_3:
            send_control_command(kTargetSpeed, 0.0f);  // 直线，曲率=0
            if (is_straight_segment_done()) {
                RCLCPP_INFO(logger_, "Straight segment done (%.2f m)", distance_traveled_);
                if (current_state_ == MissionState::STRAIGHT_1) {
                    transition_state(MissionState::CURVE_1);
                } else if (current_state_ == MissionState::STRAIGHT_2) {
                    transition_state(MissionState::CURVE_2);
                } else {
                    transition_state(MissionState::CURVE_3);
                }
            }
            break;

        case MissionState::CURVE_1:
        case MissionState::CURVE_2:
        case MissionState::CURVE_3:
            send_control_command(kTargetSpeed, kCurveKappa);  // 顺时针半圆，曲率=-2.0
            if (is_curve_segment_done()) {
                RCLCPP_INFO(logger_, "Curve segment done (traveled %.2f m)", distance_traveled_);
                circle_count_++;
                
                if (circle_count_ >= kMaxMissionCircles) {
                    RCLCPP_INFO(logger_, "Mission completed! All 3 circles finished.");
                    transition_state(MissionState::DONE);
                } else {
                    RCLCPP_INFO(logger_, "Circle %d complete. Starting circle %d/3", 
                                 circle_count_, circle_count_ + 1);
                    transition_state(MissionState::STRAIGHT_1);
                }
            }
            break;

        case MissionState::DONE:
            // 任务完成，保持停止
            send_control_command(0.0f, 0.0f);
            break;

        case MissionState::ERROR:
            // 错误状态，停止
            send_control_command(0.0f, 0.0f);
            RCLCPP_ERROR(logger_, "Mission in ERROR state");
            break;
    }
}

void CarMissionExecutor::send_control_command(float speed, float curvature)
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

bool CarMissionExecutor::is_straight_segment_done()
{
    // 根据速度和时间估计距离
    // distance = speed * time
    const auto elapsed = (now() - segment_start_time_).seconds();
    distance_traveled_ = current_speed_ * static_cast<float>(elapsed);

    return distance_traveled_ >= (kStraightLength - kDistanceTolerance);
}

bool CarMissionExecutor::is_curve_segment_done()
{
    // 半圆的弧长 = π * R
    const float curve_arc_length = M_PI * kCurveRadius;
    
    const auto elapsed = (now() - segment_start_time_).seconds();
    distance_traveled_ = current_speed_ * static_cast<float>(elapsed);

    return distance_traveled_ >= (curve_arc_length - kDistanceTolerance);
}

void CarMissionExecutor::transition_state(MissionState next_state)
{
    current_state_ = next_state;
    distance_traveled_ = 0.0f;
    segment_start_time_ = now();

    std::string state_name;
    switch (next_state) {
        case MissionState::STRAIGHT_1: state_name = "STRAIGHT_1"; break;
        case MissionState::CURVE_1:    state_name = "CURVE_1";    break;
        case MissionState::STRAIGHT_2: state_name = "STRAIGHT_2"; break;
        case MissionState::CURVE_2:    state_name = "CURVE_2";    break;
        case MissionState::STRAIGHT_3: state_name = "STRAIGHT_3"; break;
        case MissionState::CURVE_3:    state_name = "CURVE_3";    break;
        case MissionState::DONE:       state_name = "DONE";       break;
        case MissionState::ERROR:      state_name = "ERROR";      break;
        default:                       state_name = "UNKNOWN";    break;
    }
    
    RCLCPP_DEBUG(logger_, "State transition: %s", state_name.c_str());
}

} // namespace car_mission

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<car_mission::CarMissionExecutor>());
    rclcpp::shutdown();
    return 0;
}
