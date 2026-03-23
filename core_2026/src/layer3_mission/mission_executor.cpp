#include "layer3_mission/mission_executor.hpp"
#include <cmath>

MissionExecutor::MissionExecutor(
    FlightController& fc,
    IStateProvider&   state,
    IVisionProvider&  vision,
    rclcpp::Logger    logger,
    float             default_altitude)
    : fc_(fc)
    , state_(state)
    , vision_(vision)
    , logger_(logger)
    , default_altitude_(default_altitude)
    , takeoff_target_(0.0f, 0.0f, default_altitude, 0.0f)
    , shape_return_pos_(0.0f, 0.0f, 0.0f, 0.0f)
    , vel_follow_(kFollowVx, 0.0f, 0.0f)
{}

// 主循环
void MissionExecutor::run() {
    RCLCPP_INFO(logger_, "[Mission] 简化测试任务开始: 起飞 → 依次飞行5个航点 → 降落");
    while (rclcpp::ok() && current_state_ != State::DONE) {
        switch (current_state_) {
            case State::TAKEOFF:     on_takeoff();     break;
            case State::FORWARD:     on_forward();     break;
            case State::LAND:        on_land();        break;
            // case State::LINE_FOLLOW: on_line_follow(); break;
            // case State::ALIGN_SHAPE: on_align_shape(); break;
            // case State::RETURN_LINE: on_return_line(); break;
            // case State::ALIGN_LAND:  on_align_land();  break;
            case State::DONE:        break;
            default:                 break;
        }
    }
    RCLCPP_INFO(logger_, "[Mission] 任务完成");
}

// 状态：TAKEOFF - 起飞到1米高度
void MissionExecutor::on_takeoff() {
    RCLCPP_INFO(logger_, "[TAKEOFF] 上升至 %.2f m", default_altitude_);
    fc_.fly_to_target(takeoff_target_);
    RCLCPP_INFO(logger_, "[TAKEOFF] 到达目标高度，切换 FORWARD");
    current_state_ = State::FORWARD;
}

// 状态：FORWARD - 依次飞行5个航点后降落
void MissionExecutor::on_forward() {
    if (!waypoints_initialized_) {
        const auto s = state_.get_state();
        waypoints_ = {
            Target(s.x + 0.0f, s.y + 0.0f, default_altitude_, s.yaw), // 起点
            Target(s.x + 1.0f, s.y + 1.0f, default_altitude_, s.yaw),
            Target(s.x + 4.0f, s.y - 2.0f, default_altitude_, s.yaw),
            Target(s.x + 5.0f, s.y + 1.0f, default_altitude_, s.yaw),
            Target(s.x + 6.0f, s.y + 0.0f, default_altitude_, s.yaw)  // 回到起点
        };
        
        waypoints_initialized_ = true;
        waypoint_index_ = 0;
        RCLCPP_INFO(logger_, "[FORWARD] 初始化5个航点，起点(%.2f, %.2f, %.2f)",
            s.x, s.y, s.z);
    }

    if (waypoint_index_ >= kWaypointCount) {
        RCLCPP_INFO(logger_, "[FORWARD] 已完成所有航点，切换 LAND");
        current_state_ = State::LAND;
        return;
    }

    const auto& target = waypoints_[waypoint_index_];
    RCLCPP_INFO(logger_, "[FORWARD] 飞行至航点 %zu/%d: (%.2f, %.2f, %.2f)",
        waypoint_index_ + 1, kWaypointCount, target.get_x(), target.get_y(), target.get_z());
    fc_.fly_to_target(target);
    ++waypoint_index_;

    if (waypoint_index_ >= kWaypointCount) {
        RCLCPP_INFO(logger_, "[FORWARD] 完成第%zu个航点，切换 LAND", waypoint_index_);
        current_state_ = State::LAND;
    } else {
        RCLCPP_INFO(logger_, "[FORWARD] 完成航点，继续下一个");
        current_state_ = State::FORWARD;
    }
}

// // 原始复杂逻辑已注释
// void MissionExecutor::on_forward() {
//     const auto v = vision_.get_vision();
//     if (v.is_line_detected) {
//         RCLCPP_INFO(logger_, "[FORWARD] 发现直线，切换 LINE_FOLLOW");
//         vel_follow_.set_vx(kFollowVx);
//         vel_follow_.set_vy(0.0f);
//         current_state_ = State::LINE_FOLLOW;
//         return;
//     }
//     Velocity fwd(kForwardVx, 0.0f, 0.0f);
//     fc_.fly_by_velocity(fwd);
// }

// // 状态：LINE_FOLLOW
// void MissionExecutor::on_line_follow() {
//     const auto v = vision_.get_vision();
//     const auto s = state_.get_state();
//
//     // 视觉横向 & 角度修正
//     vel_follow_.set_vy(v.lateral_error / kLateralScale);
//     vel_follow_.set_vyaw(v.angle_error / kAngleScale);
//
//     // 高度保持
//     float z_err = default_altitude_ - s.z;
//     vel_follow_.set_vz(std::abs(z_err) > kAltDeadband ? z_err : 0.0f);
//
//     fc_.fly_by_velocity(vel_follow_);
//
//     if (v.is_square_detected && !is_cast_complete_) {
//         // 记录当前位置以便投掷后返回
//         shape_return_pos_.set_x(s.x);
//         shape_return_pos_.set_y(s.y);
//         shape_return_pos_.set_z(s.z);
//         shape_return_pos_.set_yaw(s.yaw);
//         RCLCPP_INFO(logger_, "[LINE_FOLLOW] 发现形状，记录位置 (%.2f, %.2f, %.2f)，切换 ALIGN_SHAPE",
//             s.x, s.y, s.z);
//         current_state_ = State::ALIGN_SHAPE;
//     } else if (v.is_circle_detected && is_cast_complete_) {
//         RCLCPP_INFO(logger_, "[LINE_FOLLOW] 发现降落区域，切换 ALIGN_LAND");
//         current_state_ = State::ALIGN_LAND;
//     }
// }

// // 状态：ALIGN_SHAPE
// void MissionExecutor::on_align_shape() {
//     const auto v = vision_.get_vision();
//     const auto s = state_.get_state();
//
//     Velocity align_v(
//         v.center_x1_error / kLateralScale,
//         v.center_y1_error / kLateralScale,
//         0.0f, 0.0f);
//
//     float z_err = default_altitude_ - s.z;
//     align_v.set_vz(std::abs(z_err) > kAltDeadband ? z_err : 0.0f);
//
//     fc_.fly_by_velocity(align_v);
//
//     if (std::abs(v.center_x1_error) < kAlignThreshold &&
//         std::abs(v.center_y1_error) < kAlignThreshold)
//     {
//         RCLCPP_INFO(logger_, "[ALIGN_SHAPE] 对准成功，执行投掷，切换 RETURN_LINE");
//         // TODO: 触发投掷动作（舵机/GPIO 命令）
//         is_cast_complete_ = true;
//         current_state_    = State::RETURN_LINE;
//     }
// }

// // 状态：RETURN_LINE
// void MissionExecutor::on_return_line() {
//     RCLCPP_INFO(logger_, "[RETURN_LINE] 返回巡线位置");
//     fc_.fly_to_target(shape_return_pos_);
//     // 恢复巡线速度
//     vel_follow_.set_vx(kFollowVx);
//     vel_follow_.set_vy(0.0f);
//     RCLCPP_INFO(logger_, "[RETURN_LINE] 完成，恢复 LINE_FOLLOW");
//     current_state_ = State::LINE_FOLLOW;
// }

// // 状态：ALIGN_LAND
// void MissionExecutor::on_align_land() {
//     const auto v = vision_.get_vision();
//     const auto s = state_.get_state();
//
//     Velocity align_v(
//         v.center_x2_error / kLateralScale,
//         v.center_y2_error / kLateralScale,
//         0.0f, 0.0f);
//
//     float z_err = default_altitude_ - s.z;
//     align_v.set_vz(std::abs(z_err) > kAltDeadband ? 0.03f * (z_err > 0 ? 1.f : -1.f) : 0.0f);
//
//     fc_.fly_by_velocity(align_v);
//
//     if (std::abs(v.center_x2_error) < kAlignThreshold &&
//         std::abs(v.center_y2_error) < kAlignThreshold)
//     {
//         RCLCPP_INFO(logger_, "[ALIGN_LAND] 对准成功，切换 LAND");
//         current_state_ = State::LAND;
//     }
// }

// 状态：LAND
void MissionExecutor::on_land() {
    RCLCPP_INFO(logger_, "[LAND] 开始降落，速度 %.2f m/s，持续 %.1f s",
        kLandVz, kLandDuration);
    Velocity land_vel(0.0f, 0.0f, kLandVz);
    fc_.fly_by_vel_duration(land_vel, kLandDuration);
    RCLCPP_INFO(logger_, "[LAND] 降落完成");
    current_state_ = State::DONE;
}
