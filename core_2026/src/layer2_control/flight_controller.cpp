#include "layer2_control/flight_controller.hpp"
#include <algorithm>
#include <cmath>

namespace {
float shortest_angular_error(float target_yaw, float current_yaw) {
    return std::remainder(target_yaw - current_yaw, 2.0f * static_cast<float>(M_PI));
}
} // namespace

FlightController::FlightController(
    IStateProvider&          state,
    ICommandPublisher&        cmd,
    rclcpp::Logger           logger,
    rclcpp::Clock::SharedPtr clock,
    int                      rate_hz,
    PidConfig                pid_cfg)
    : state_(state)
    , cmd_(cmd)
    , logger_(logger)
    , clock_(clock)
    , rate_(std::make_shared<rclcpp::Rate>(rate_hz))
    , pid_cfg_(pid_cfg)
{}

// 定点移动（阻塞）非具名重载
void FlightController::fly_to_target(
    const Target& target,
    float timeout_sec, float stable_time_sec, int frame_rate)
{
    fly_to_target_impl(target, timeout_sec, stable_time_sec, frame_rate);
}

void FlightController::fly_to_target_pid(
    const Target& target,
    float timeout_sec, float stable_time_sec, int frame_rate)
{
    fly_to_target_pid_impl(target, timeout_sec, stable_time_sec, frame_rate);
}


// 定点移动（阻塞）impl
void FlightController::fly_to_target_impl(
    const Target& target,
    float timeout_sec, float stable_time_sec, int frame_rate)
{
    auto start_time  = clock_->now();
    int  stable_count  = 0;
    const int required = static_cast<int>(stable_time_sec * frame_rate);

    Target cmd_target = target; // 可修改副本（设置时间戳用）

    do {
        if ((clock_->now() - start_time).seconds() > timeout_sec) {
            RCLCPP_WARN(logger_, "[fly_to_target]: 超时 (%.1f s)", timeout_sec);
            break;
        }
        cmd_.publish_position(cmd_target);
        (pos_check(target)) ? ++stable_count : (stable_count = 0);
        rate_->sleep();
    } while (rclcpp::ok() && stable_count < required);

    if (stable_count >= required) {
        RCLCPP_INFO(logger_, "[fly_to_target]: 到达目标点：(%.2f, %.2f, %.2f)", target.get_x(), target.get_y(), target.get_z());
    }
}

// 定点移动（PID）impl
void FlightController::fly_to_target_pid_impl(
    const Target& target,
    float timeout_sec, float stable_time_sec, int frame_rate)
{
    PidController pid_x(pid_cfg_.xy);
    PidController pid_y(pid_cfg_.xy);
    PidController pid_z(pid_cfg_.z);
    PidController pid_yaw(pid_cfg_.yaw);

    int stable_count  = 0;
    const int required = static_cast<int>(stable_time_sec * frame_rate);

    rclcpp::Time last_time  = clock_->now();
    rclcpp::Time start_time = last_time;

    while (rclcpp::ok()) {
        auto current_time = clock_->now();
        double dt = (current_time - last_time).seconds();
        last_time = current_time;

        if ((current_time - start_time).seconds() > timeout_sec) {
            RCLCPP_WARN(logger_, "[fly_to_target_pid]: 超时 (%.1f s)", timeout_sec);
            break;
        }

        if (pos_check(target)) {
            if (++stable_count >= required) {
                RCLCPP_INFO(logger_, "[fly_to_target_pid]: 到达目标点：(%.2f, %.2f, %.2f)", target.get_x(), target.get_y(), target.get_z());
                break;
            }
        } else { stable_count = 0; }

        const DroneState s = state_.get_state();
        float vx = pid_x.update(target.get_x() - s.x, dt);
        float vy = pid_y.update(target.get_y() - s.y, dt);
        float vz = pid_z.update(target.get_z() - s.z, dt);
        float vyaw = pid_yaw.update(shortest_angular_error(target.get_yaw(), s.yaw), dt);
        fly_by_velocity(Velocity(vx, vy, vz, vyaw));
        rate_->sleep();
    }
}

// 单次速度发布
void FlightController::fly_by_velocity(const Velocity& velocity) {
    fly_by_velocity_impl(velocity);
}

// 单次速度发布impl
void FlightController::fly_by_velocity_impl(const Velocity& velocity) {
    Velocity cmd_vel = velocity; // 创建可修改副本（设置时间戳用）
    cmd_.publish_velocity(cmd_vel);
}

// 持续速度发布（含高度hang）
void FlightController::fly_by_vel_duration(const Velocity& velocity, float duration) {
    fly_by_vel_duration_impl(velocity, duration);
}

// 持续速度发布（含高度hang）impl
void FlightController::fly_by_vel_duration_impl(const Velocity& velocity, float duration) {
    const rclcpp::Time start_time = clock_->now();
    const float start_altitude = state_.get_state().z;
    Velocity vel_cmd = velocity;

    while (rclcpp::ok()) {
        if ((clock_->now() - start_time).seconds() >= duration) break;

        // 高度反馈控制（防止漂移）
        const float z_error = start_altitude - state_.get_state().z;
        std::abs(z_error) > 0.1f 
            ? vel_cmd.set_vz(std::clamp(z_error * 1.0f, -0.1f, 0.1f))  // 限幅0.1
            : vel_cmd.set_vz(0.0f);

        fly_by_velocity(vel_cmd);
        rate_->sleep();
    }
}

// 路径航点飞行
void FlightController::fly_by_path(Path& path) {
    fly_by_path_impl(path);
}

// 路径航点飞行核心逻辑
void FlightController::fly_by_path_impl(Path& path) {
    using namespace fly_to_target_args;
    Target waypoint;
    while (rclcpp::ok()) {
        if (path.get_next_waypoint(waypoint)) {
            fly_to_target(target = waypoint, timeout_sec = 10.0f, stable_time_sec = 0.5f, frame_rate = 30);
        } else {
            RCLCPP_INFO(logger_, "[fly_by_path]: 所有航点已执行完毕");
            break;
        }
    }
}

// 运行时热更新 PID
void FlightController::set_pid_config(PidConfig cfg) { set_pid_config_impl(cfg); }

// 运行时热更新 PID impl
void FlightController::set_pid_config_impl(PidConfig cfg) { pid_cfg_ = cfg; }

// 位置检查（球径）
bool FlightController::pos_check(const Target& target, float distance) {
    const DroneState s = state_.get_state();

    float dist = std::sqrt(
        std::pow(s.x - target.get_x(), 2) +
        std::pow(s.y - target.get_y(), 2) +
        std::pow(s.z - target.get_z(), 2));

    const float yaw_error = std::fabs(shortest_angular_error(target.get_yaw(), s.yaw));
    RCLCPP_INFO_THROTTLE(logger_, *clock_, 2000, "距目标: %.3f m", dist);
    return dist < distance && yaw_error < 0.1f;
}

// 位置检查（严格三轴）
bool FlightController::pos_check(
    const Target& target,
    float distance_x, float distance_y, float distance_z)
{
    const DroneState s = state_.get_state();
    const float yaw_error = std::fabs(shortest_angular_error(target.get_yaw(), s.yaw));
    return std::abs(s.x - target.get_x()) < distance_x &&
           std::abs(s.y - target.get_y()) < distance_y &&
           std::abs(s.z - target.get_z()) < distance_z &&
           yaw_error < 0.1f;
}
