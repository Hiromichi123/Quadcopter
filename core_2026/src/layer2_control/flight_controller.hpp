#pragma once

#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_command_publisher.hpp"
#include "layer2_control/pid_controller.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"
#include "layer0_common/path.hpp"

constexpr float DEFAULT_POS_CHECK_DISTANCE = 0.25f;

/**
 * @struct PidConfig
 * @brief XYZ轴 PID，供 fly_to_target_pid() 使用。
 */
struct PidConfig {
    PidGains xy{1.0f, 0.1f, 0.2f, 0.5f, 1.0f};  // XY 轴增益
    PidGains z {1.5f, 0.2f, 0.1f, 0.3f, 0.5f};  // Z  轴增益
};

/**
 * @brief 飞行控制层
 * - 负责将 mission层 的goal或path转换为具体的vel指令下发到 hal层，执行核心飞行控制逻辑。
 * - 通过依赖注入接收 IStateProvider 和 ICommandPublisher，与具体的 HAL-ROS层 解耦。
 * - PID 参数通过 PidConfig 注入
 */
class FlightController {
public:
    /**
     * @param state     位姿状态提供者（DroneHAL 实现）
     * @param cmd       指令发布者（DroneHAL 实现）
     * @param logger    日志记录器（由 DroneHAL::get_logger() 传入）
     * @param clock     稳态时钟（由 DroneHAL::get_clock() 传入）
     * @param rate_hz   控制频率（Hz），默认 20
     * @param pid_cfg   PID 增益配置，默认与原版常量一致
     */
    FlightController(IStateProvider&          state,
                     ICommandPublisher&        cmd,
                     rclcpp::Logger           logger,
                     rclcpp::Clock::SharedPtr clock,
                     int                      rate_hz = 20,
                     PidConfig                pid_cfg = {});

    // ===== 飞行动作组 =====
    // 定点移动（阻塞）
    void fly_to_target(const Target& target,
                       float timeout_sec     = 10.0f,
                       float stable_time_sec = 0.25f,
                       int   frame_rate      = 30);

    /// 定点移动（PID 速度闭环）
    void fly_to_target_pid(const Target& target,
                           float timeout_sec     = 10.0f,
                           float stable_time_sec = 0.25f,
                           int   frame_rate      = 30);

    /// 速度飞行，单次发布
    void fly_by_velocity(const Velocity& velocity);

    /// 速度飞行，保持持续 duration 秒（含高度 P 控制）
    void fly_by_vel_duration(const Velocity& velocity, float duration);

    /// 路径航点飞行
    void fly_by_path(Path& path);

    /// 运行时更新 PID 增益
    void set_pid_config(PidConfig cfg);

private:
    IStateProvider&               state_;
    ICommandPublisher&            cmd_;
    rclcpp::Logger                logger_;
    rclcpp::Clock::SharedPtr      clock_;
    std::shared_ptr<rclcpp::Rate> rate_;
    PidConfig                     pid_cfg_;

    /**
     * @brief 球形半径到达检查。
     * @return true = 位置与偏航均在误差内
     */
    bool pos_check(const Target& target, float distance = DEFAULT_POS_CHECK_DISTANCE);

    /**
     * @brief 各轴分别设置误差阈值，严格检查    
     * @return true = 位置与偏航均在误差内
     */
    bool pos_check(const Target& target,
                   float distance_x, float distance_y, float distance_z);
};
