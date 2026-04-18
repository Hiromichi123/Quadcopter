#include "layer3_mission/mission_executor.hpp"
#include <cmath>

using namespace fly_to_target_args;

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
    , hover_target_(0.0f, 0.0f, default_altitude, 0.0f)
{}

// 主循环
void MissionExecutor::run() {
    RCLCPP_INFO(logger_, "[Mission] 新任务开始: 起飞到1m悬停 → 触发规避后返回原位 → 20s后降落");
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

// 状态：TAKEOFF - 起飞到1米高度
void MissionExecutor::on_takeoff() {
    RCLCPP_INFO(logger_, "[TAKEOFF] 上升至 %.2f m", default_altitude_);
    fc_.fly_to_target(target = takeoff_target_);
    const auto s = state_.get_state();
    hover_target_ = Target(s.x, s.y, default_altitude_, s.yaw);
    hover_start_time_ = steady_clock_.now();
    last_avoid_time_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
    hover_initialized_ = true;
    RCLCPP_INFO(logger_, "[TAKEOFF] 到达目标高度，切换 HOVER");
    current_state_ = State::HOVER;
}

// 状态：HOVER - 悬停并监测视觉触发，20s后自动降落
void MissionExecutor::on_hover() {
    if (!hover_initialized_) {
        const auto s = state_.get_state();
        hover_target_ = Target(s.x, s.y, default_altitude_, s.yaw);
        hover_start_time_ = steady_clock_.now();
        hover_initialized_ = true;
    }

    const auto now = steady_clock_.now();
    const double elapsed = (now - hover_start_time_).seconds();
    if (elapsed >= kMissionDurationSec) {
        RCLCPP_INFO(logger_, "[HOVER] 已悬停 %.1f s，切换 LAND", elapsed);
        current_state_ = State::LAND;
        return;
    }

    bool trigger_avoid = false;
    if (vision_.has_vision()) {
        const auto v = vision_.get_vision();
        const bool in_window = std::abs(v.center_x1_error - kAvoidTriggerValue) <= kAvoidTriggerTol;
        const bool cooldown_ok = (last_avoid_time_.nanoseconds() == 0) || ((now - last_avoid_time_).seconds() >= kAvoidCooldownSec);
        trigger_avoid = in_window && cooldown_ok;
    }

    if (trigger_avoid) {
        last_avoid_time_ = now;
        const auto s = state_.get_state();
        Target avoid_target(s.x + kAvoidOffsetX, s.y, default_altitude_, s.yaw);
        RCLCPP_WARN(logger_, "[HOVER] 视觉触发规避：center_x1_error 命中特定值，执行侧向规避并返回原位");
        fc_.fly_to_target(target = avoid_target, timeout_sec = 6.0f, stable_time_sec = 0.2f, frame_rate = 20);
        fc_.fly_to_target(target = hover_target_, timeout_sec = 6.0f, stable_time_sec = 0.2f, frame_rate = 20);
    } else {
        RCLCPP_INFO_THROTTLE(
            logger_, steady_clock_, 1000,
            "[HOVER] 悬停中，剩余 %.1f s",
            kMissionDurationSec - static_cast<float>(elapsed));
        fc_.fly_to_target(target = hover_target_, timeout_sec = kHoverTimeoutSec, stable_time_sec = kHoverStableSec, frame_rate = 20);
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
