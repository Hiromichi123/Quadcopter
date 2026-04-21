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
    , takeoff_target_(0.0f, 0.0f, default_altitude, 0.0f)
    , hover_target_(0.0f, 0.0f, default_altitude, 0.0f)
{}

// 主循环
void MissionExecutor::run() {
    RCLCPP_INFO(logger_, "[Mission] 新任务开始: 起飞到1m → 执行5点飞行 → 自动降落");
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
    hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
    dvs_block_until_ = steady_clock_.now() + rclcpp::Duration::from_seconds(kHoverStableEnableSec);
    dvs_accept_enabled_ = false;
    hover_initialized_ = true;
    RCLCPP_INFO(logger_, "[TAKEOFF] 到达目标高度，切换 HOVER");
    current_state_ = State::HOVER;
}

// 状态：HOVER - 执行简单5点飞行任务（已禁用 DVS 规避状态机）
void MissionExecutor::on_hover() {
    if (!hover_initialized_) {
        const auto s = state_.get_state();
        hover_target_ = Target(s.x, s.y, default_altitude_, s.yaw);
        hover_start_time_ = steady_clock_.now();
        hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
        dvs_block_until_ = steady_clock_.now() + rclcpp::Duration::from_seconds(kHoverStableEnableSec);
        dvs_accept_enabled_ = false;
        hover_initialized_ = true;
    }

    const auto s = state_.get_state();
    const float z = default_altitude_;
    const float yaw = s.yaw;
    const float x0 = s.x;
    const float y0 = s.y;

    const std::array<Target, 4> waypoints = {
        Target(x0 + 0.6f, y0 + 0.4f, z, yaw),
        Target(x0 + 1.0f, y0 + 0.0f, z, yaw),
        Target(x0 + 1.4f, y0 - 0.4f, z, yaw),
        Target(x0 + 1.8f, y0 + 0.0f, z, yaw),
    };

    RCLCPP_INFO(logger_, "[HOVER] DVS规避逻辑已禁用，开始执行4点飞行任务");
    for (std::size_t i = 0; i < waypoints.size() && rclcpp::ok(); ++i) {
        RCLCPP_INFO(logger_, "[HOVER] 飞往航点 %zu/4", i + 1);
        fc_.fly_to_target_pid(waypoints[i], 8.0f, 0.1f, 30);
    }

    RCLCPP_INFO(logger_, "[HOVER] 4点飞行完成，切换 LAND");
    current_state_ = State::LAND;
}

bool MissionExecutor::is_near_hover_target(const DroneState& s) const {
    const float dx = std::fabs(s.x - hover_target_.get_x());
    const float dy = std::fabs(s.y - hover_target_.get_y());
    const float dz = std::fabs(s.z - hover_target_.get_z());
    return dx <= kHoverStablePosTolXY && dy <= kHoverStablePosTolXY && dz <= kHoverStablePosTolZ;
}

void MissionExecutor::update_dvs_accept_gate(const rclcpp::Time& now) {
    const auto s = state_.get_state();
    if (!is_near_hover_target(s)) {
        hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
        dvs_accept_enabled_ = false;
        return;
    }

    if (hover_stable_since_.nanoseconds() == 0) {
        hover_stable_since_ = now;
        dvs_accept_enabled_ = false;
        return;
    }

    const bool stable_enough = (now - hover_stable_since_).seconds() >= kHoverStableEnableSec;
    const bool still_blocked =
        (dvs_block_until_.nanoseconds() != 0) && ((dvs_block_until_ - now).seconds() > 0.0);

    dvs_accept_enabled_ = stable_enough && !still_blocked;
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
