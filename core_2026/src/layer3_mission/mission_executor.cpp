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
    hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
    dvs_block_until_ = steady_clock_.now() + rclcpp::Duration::from_seconds(kHoverStableEnableSec);
    dvs_accept_enabled_ = false;
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
        hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
        dvs_block_until_ = steady_clock_.now() + rclcpp::Duration::from_seconds(kHoverStableEnableSec);
        dvs_accept_enabled_ = false;
        hover_initialized_ = true;
    }

    const auto now = steady_clock_.now();
    const double elapsed = (now - hover_start_time_).seconds();
    if (elapsed >= kMissionDurationSec) {
        RCLCPP_INFO(logger_, "[HOVER] 已悬停 %.1f s，切换 LAND", elapsed);
        current_state_ = State::LAND;
        return;
    }

    update_dvs_accept_gate(now);
    const bool dvs_still_blocked =
        (dvs_block_until_.nanoseconds() != 0) && ((dvs_block_until_ - now).seconds() > 0.0);

    // 仅在规避后的阻塞窗口内屏蔽 DVS；稳定判定阶段允许继续接收。
    if (dvs_still_blocked) {
        RCLCPP_INFO_THROTTLE(
            logger_, steady_clock_, 1000,
            "[HOVER] 规避阻塞窗口内，DVS暂不接收，剩余 %.1f s",
            kMissionDurationSec - static_cast<float>(elapsed));
        cmd_.publish_position(hover_target_);
        return;
    }

    if (!dvs_accept_enabled_) {
        RCLCPP_INFO_THROTTLE(
            logger_, steady_clock_, 2000,
            "[HOVER] 悬停稳定判定中，DVS接收已放行");
    }

    // 纯 DVS 被动触发规避（后退 + 左/右 + 上抬）
    if (dvs_.has_recent_dvs_avoid(kDvsCmdFreshSec)) {
        const auto cmd = dvs_.get_dvs_avoid_cmd();
        const bool has_motion =
            (std::abs(cmd.linear.x) > 1e-4) ||
            (std::abs(cmd.linear.y) > 1e-4) ||
            (std::abs(cmd.linear.z) > 1e-4);

        if (has_motion) {
            const bool cooldown_ok =
                (last_avoid_time_.nanoseconds() == 0) ||
                ((now - last_avoid_time_).seconds() >= kDvsAvoidCooldownSec);
            if (!cooldown_ok) {
                cmd_.publish_position(hover_target_);
                return;
            }

            last_avoid_time_ = now;
            dvs_accept_enabled_ = false;
            const auto s = state_.get_state();
            Target origin_target(s.x, s.y, default_altitude_, s.yaw);
            const float side_sign = (cmd.linear.y >= 0.0) ? 1.0f : -1.0f;

            float avoid_dx = kDvsAvoidBackX;
            float avoid_dy = kDvsAvoidSideY * side_sign;
            float avoid_dz = kDvsAvoidUpZ;
            const float l1 = std::fabs(avoid_dx) + std::fabs(avoid_dy) + std::fabs(avoid_dz);
            if (l1 > kDvsAvoidL1Max) {
                const float scale = kDvsAvoidL1Max / l1;
                avoid_dx *= scale;
                avoid_dy *= scale;
                avoid_dz *= scale;
            }

            Target avoid_target(
                s.x + avoid_dx,
                s.y + avoid_dy,
                default_altitude_ + avoid_dz,
                s.yaw);

            RCLCPP_WARN_THROTTLE(
                logger_,
                steady_clock_,
                500,
                "[HOVER] DVS触发规避: 后退上抬侧移 (dx=%.2f, dy=%.2f, dz=%.2f), 到达后立即返回",
                avoid_dx,
                avoid_dy,
                avoid_dz);

            fc_.fly_to_target_pid(
                avoid_target,
                kDvsMoveTimeoutSec,
                kDvsMoveStableSec,
                30);

            fc_.fly_to_target_pid(
                origin_target,
                kDvsMoveTimeoutSec,
                kDvsMoveStableSec,
                30);

            // 回到悬停点后再解锁 DVS，期间认为 DVS 输入无效。
            hover_stable_since_ = rclcpp::Time(0, 0, RCL_STEADY_TIME);
            dvs_block_until_ = steady_clock_.now() + rclcpp::Duration::from_seconds(kDvsRearmDelaySec);
            dvs_accept_enabled_ = false;
            return;
        }
    }

    RCLCPP_INFO_THROTTLE(
        logger_, steady_clock_, 1000,
        "[HOVER] 悬停中，剩余 %.1f s",
        kMissionDurationSec - static_cast<float>(elapsed));
    cmd_.publish_position(hover_target_);
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
