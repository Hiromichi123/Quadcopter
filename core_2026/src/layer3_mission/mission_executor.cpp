#include "layer3_mission/mission_executor.hpp"

using namespace fly_to_target_args;

MissionExecutor::MissionExecutor(
    FlightController& fc,
    IStateProvider&   state,
    IVisionProvider&  vision,
    IDvsAvoidProvider& dvs,
    ICommandPublisher& cmd,
    rclcpp::Logger    logger,
    float             default_altitude)
    : fc_(fc)
    , state_(state)
    , vision_(vision)
    , dvs_(dvs)
    , cmd_(cmd)
    , logger_(logger)
    , default_altitude_(default_altitude)
    , takeoff_target_(0.0f, 0.0f, kHoverAltitude, 0.0f)
    , hover_target_(0.0f, 0.0f, kHoverAltitude, 0.0f)
{}

// 主循环
void MissionExecutor::run() {
    RCLCPP_INFO(logger_, "[Mission] 任务开始: 原地起飞 → 1.70m持续发布悬停目标50s → 降落");
    while (rclcpp::ok() && current_state_ != State::DONE) {
        switch (current_state_) {
            case State::TAKEOFF:     on_takeoff();     break;
            case State::HOVER:       on_hover();       break;
            case State::LAND:        on_land();        break;
            case State::DONE:        break;
            default:                 break;
        }
    }
    RCLCPP_INFO(logger_, "[Mission] 任务完成");
}

// 状态：TAKEOFF - 原地起飞到 1.70m
void MissionExecutor::on_takeoff() {
    const auto s = state_.get_state();
    hover_anchor_x_ = s.x;
    hover_anchor_y_ = s.y;
    hover_anchor_yaw_ = s.yaw;
    takeoff_target_ = Target(hover_anchor_x_, hover_anchor_y_, kHoverAltitude, hover_anchor_yaw_);

    RCLCPP_INFO(logger_, "[TAKEOFF] 原地上升至 %.2f m", kHoverAltitude);
    fc_.fly_to_target(target = takeoff_target_);
    hover_target_ = Target(hover_anchor_x_, hover_anchor_y_, kHoverAltitude, hover_anchor_yaw_);
    hover_start_time_ = steady_clock_.now();
    RCLCPP_INFO(logger_, "[TAKEOFF] 到达 %.2f m，切换 HOVER", kHoverAltitude);
    current_state_ = State::HOVER;
}

// 状态：HOVER - 不做稳定检查，持续发布 1.70m 原地悬停目标 50s
void MissionExecutor::on_hover() {
    const auto now = steady_clock_.now();
    const double elapsed = (now - hover_start_time_).seconds();
    cmd_.publish_position(hover_target_);

    RCLCPP_INFO_THROTTLE(
        logger_, steady_clock_, 1000,
        "[HOVER] 持续发布 %.2f m 原地悬停目标，剩余 %.1f s",
        hover_target_.get_z(),
        kHoverDurationSec - static_cast<float>(elapsed));

    if (elapsed >= kHoverDurationSec) {
        RCLCPP_INFO(logger_, "[HOVER] 已持续发布悬停目标 %.1f s，切换 LAND", elapsed);
        current_state_ = State::LAND;
    }
}

// 状态：LAND
void MissionExecutor::on_land() {
    RCLCPP_INFO(logger_, "[LAND] 开始降落，速度 %.2f m/s，持续 %.1f s",
        kLandVz, kLandDuration);
    Velocity land_vel(0.0f, 0.0f, kLandVz);
    fc_.fly_by_vel_duration(land_vel, kLandDuration);
    RCLCPP_INFO(logger_, "[LAND] 降落完成");
    current_state_ = State::DONE;
}
