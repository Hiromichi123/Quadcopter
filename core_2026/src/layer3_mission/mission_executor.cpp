#include "layer3_mission/mission_executor.hpp"
#include <cmath>

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
    , takeoff_target_(0.0f, 0.0f, kHoverStartAltitude, 0.0f)
    , hover_target_(0.0f, 0.0f, kHoverStartAltitude, 0.0f)
{}

// 主循环
void MissionExecutor::run() {
    RCLCPP_INFO(logger_, "[Mission] 任务开始: 原地起飞 → 1.70m~2.20m每10cm悬停8s → 降落");
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

// 状态：TAKEOFF - 原地起飞到第一个高度刻度
void MissionExecutor::on_takeoff() {
    const auto s = state_.get_state();
    hover_anchor_x_ = s.x;
    hover_anchor_y_ = s.y;
    hover_anchor_yaw_ = s.yaw;
    takeoff_target_ = Target(hover_anchor_x_, hover_anchor_y_, kHoverStartAltitude, hover_anchor_yaw_);

    RCLCPP_INFO(logger_, "[TAKEOFF] 原地上升至 %.2f m", kHoverStartAltitude);
    fc_.fly_to_target(target = takeoff_target_);
    hover_step_index_ = 0;
    hover_step_start_time_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
    set_hover_step_target();
    hover_initialized_ = true;
    RCLCPP_INFO(logger_, "[TAKEOFF] 到达第一个高度刻度，切换 HOVER");
    current_state_ = State::HOVER;
}

// 状态：HOVER - 1.70m 到 2.20m，每 10cm 刻度悬停 8s
void MissionExecutor::on_hover() {
    if (!hover_initialized_) {
        const auto s = state_.get_state();
        hover_anchor_x_ = s.x;
        hover_anchor_y_ = s.y;
        hover_anchor_yaw_ = s.yaw;
        hover_step_index_ = 0;
        hover_step_start_time_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
        set_hover_step_target();
        hover_initialized_ = true;
    }

    if (hover_step_index_ >= kHoverStepCount) {
        RCLCPP_INFO(logger_, "[HOVER] 所有高度刻度已完成，切换 LAND");
        current_state_ = State::LAND;
        return;
    }

    const auto now = steady_clock_.now();
    const auto s = state_.get_state();
    if (!is_near_hover_target(s)) {
        hover_step_start_time_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
        RCLCPP_INFO_THROTTLE(
            logger_, steady_clock_, 1000,
            "[HOVER] 前往高度刻度 %.2f m",
            hover_target_.get_z());
        cmd_.publish_position(hover_target_);
        return;
    }

    if (hover_step_start_time_.nanoseconds() == 0) {
        hover_step_start_time_ = now;
        RCLCPP_INFO(
            logger_,
            "[HOVER] 到达 %.2f m，开始悬停 %.1f s",
            hover_target_.get_z(),
            kHoverHoldSec);
        cmd_.publish_position(hover_target_);
        return;
    }

    const double elapsed = (now - hover_step_start_time_).seconds();
    if (elapsed >= kHoverHoldSec) {
        RCLCPP_INFO(logger_, "[HOVER] %.2f m 悬停完成", hover_target_.get_z());
        ++hover_step_index_;
        if (hover_step_index_ >= kHoverStepCount) {
            current_state_ = State::LAND;
            return;
        }
        set_hover_step_target();
        return;
    }

    RCLCPP_INFO_THROTTLE(
        logger_, steady_clock_, 1000,
        "[HOVER] %.2f m 悬停中，剩余 %.1f s",
        hover_target_.get_z(),
        kHoverHoldSec - static_cast<float>(elapsed));
    cmd_.publish_position(hover_target_);
}

bool MissionExecutor::is_near_hover_target(const DroneState& s) const {
    const float dx = std::fabs(s.x - hover_target_.get_x());
    const float dy = std::fabs(s.y - hover_target_.get_y());
    const float dz = std::fabs(s.z - hover_target_.get_z());
    return dx <= kHoverStablePosTolXY && dy <= kHoverStablePosTolXY && dz <= kHoverStablePosTolZ;
}

float MissionExecutor::hover_altitude_for_step(std::size_t step_index) const {
    return kHoverStartAltitude + static_cast<float>(step_index) * kHoverAltitudeStep;
}

void MissionExecutor::set_hover_step_target() {
    hover_step_start_time_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
    hover_target_ = Target(
        hover_anchor_x_,
        hover_anchor_y_,
        hover_altitude_for_step(hover_step_index_),
        hover_anchor_yaw_);
    RCLCPP_INFO(
        logger_,
        "[HOVER] 切换到第 %zu/%zu 个高度刻度: %.2f m",
        hover_step_index_ + 1,
        kHoverStepCount,
        hover_target_.get_z());
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
