#include "layer2_control/flight_controller.hpp"
#include <algorithm>
#include <cmath>

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

// ===== 飞行运动组 =====
# pragma region movement_methods
// 定点移动
void FlightController::fly_to_target(
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
        if (pos_check(cmd_target)) ++stable_count;
        else stable_count = 0;
        rate_->sleep();
    } while (rclcpp::ok() && stable_count < required);

    if (stable_count >= required) {
        RCLCPP_INFO(logger_, "[fly_to_target]: 到达目标点：(%.2f, %.2f, %.2f)", target.get_x(), target.get_y(), target.get_z());
    }
}

// 定点移动（PID）
void FlightController::fly_to_target_pid(
    const Target& target,
    float timeout_sec, float stable_time_sec, int frame_rate)
{
    PidController pid_x(pid_cfg_.xy);
    PidController pid_y(pid_cfg_.xy);
    PidController pid_z(pid_cfg_.z);

    int  stable_count  = 0;
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
        } else {
            stable_count = 0;
        }

        const DroneState s = state_.get_state();
        float vx = pid_x.update(target.get_x() - s.x, dt);
        float vy = pid_y.update(target.get_y() - s.y, dt);
        float vz = pid_z.update(target.get_z() - s.z, dt);

        fly_by_velocity(Velocity(vx, vy, vz));
        rate_->sleep();
    }
}

// 单次速度发布
void FlightController::fly_by_velocity(const Velocity& velocity) {
    Velocity cmd_vel = velocity; // 创建可修改副本（设置时间戳用）
    cmd_.publish_velocity(cmd_vel);
}

// 持续速度发布（含高度hang）
void FlightController::fly_by_vel_duration(const Velocity& velocity, float duration) {
    const rclcpp::Time start_time = clock_->now();
    const float start_altitude = state_.get_state().z;
    Velocity vel_cmd = velocity;

    while (rclcpp::ok()) {
        if ((clock_->now() - start_time).seconds() >= duration) break;

        // 高度反馈控制（防止漂移）
        const float z_error = start_altitude - state_.get_state().z;
        if (std::abs(z_error) > 0.1f) {
            vel_cmd.set_vz(std::clamp(z_error * 1.0f, -0.1f, 0.1f));
        } else {
            vel_cmd.set_vz(0.0f);
        }

        fly_by_velocity(vel_cmd);
        rate_->sleep();
    }
}

// 路径航点飞行
void FlightController::fly_by_path(Path& path) {
    Target waypoint;
    while (rclcpp::ok()) {
        if (path.get_next_waypoint(waypoint)) {
            fly_to_target(waypoint);
        } else {
            RCLCPP_INFO(logger_, "[fly_by_path]: 所有航点已执行完毕");
            break;
        }
    }
}
# pragma endregion

// 运行时热更新 PID
void FlightController::set_pid_config(PidConfig cfg) { pid_cfg_ = cfg; }

// 位置检查（球径）
bool FlightController::pos_check(const Target& target, float distance) {
    const DroneState s = state_.get_state();

    float dist = std::sqrt(
        std::pow(s.x - target.get_x(), 2) +
        std::pow(s.y - target.get_y(), 2) +
        std::pow(s.z - target.get_z(), 2));

    RCLCPP_INFO_THROTTLE(logger_, *clock_, 2000, "距目标: %.3f m", dist);
    return dist < distance &&
           std::abs(s.yaw - target.get_yaw()) < 0.1f;
}

// 位置检查（各轴分别设置误差阈值）
bool FlightController::pos_check(
    const Target& target,
    float distance_x, float distance_y, float distance_z)
{
    const DroneState s = state_.get_state();
    return std::abs(s.x - target.get_x()) < distance_x &&
           std::abs(s.y - target.get_y()) < distance_y &&
           std::abs(s.z - target.get_z()) < distance_z &&
           std::abs(s.yaw - target.get_yaw()) < 0.1f;
}
