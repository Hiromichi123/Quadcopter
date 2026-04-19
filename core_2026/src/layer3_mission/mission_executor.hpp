#pragma once

#include <rclcpp/rclcpp.hpp>
#include <array>
#include <cstddef>
#include <memory>

#include "layer2_control/flight_controller.hpp"
#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_vision_provider.hpp"
#include "layer1_hal/i_dvs_avoid_provider.hpp"
#include "layer1_hal/i_command_publisher.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

/**
 * @brief 任务执行层
 *
 * - 通过接口获取状态和视觉数据，不依赖任何具体硬件类。
 */
class MissionExecutor {
public:
    MissionExecutor(FlightController&  fc,     // 飞行控制器
                    IStateProvider&    state,  // 状态接口
                    IVisionProvider&   vision, // 视觉提供接口
                    IDvsAvoidProvider& dvs,    // DVS规避接口
                    ICommandPublisher& cmd,    // 指令发布接口（悬停直发setpoint）
                    rclcpp::Logger     logger, // DroneHAL日志记录器
                    float              default_altitude = 1.0f);

    void run(); // 开始执行任务

private:
    // ===== 状态机组 =====
    enum class State {
        TAKEOFF,       // 上升至指定高度
        HOVER,         // 悬停并监测视觉触发
        LAND,          // 缓慢降落
        DONE           // 任务完成
    };

    // ===== 状态方法组 =====
    void on_takeoff();
    void on_hover();
    void on_land();

    // ===== 具名常量组 =====
    static constexpr float  kMissionDurationSec = 20.0f;  // 悬停阶段总时长
    static constexpr int    kAvoidTriggerValue  = 100;    // 视觉触发特定值
    static constexpr int    kAvoidTriggerTol    = 5;      // 触发容差
    static constexpr float  kAvoidOffsetX       = 0.8f;   // 规避动作X方向偏移
    static constexpr float  kAvoidCooldownSec   = 3.0f;   // 相邻规避最小间隔
    static constexpr float  kDvsCmdFreshSec     = 0.12f;  // DVS规避指令新鲜度门限
    static constexpr float  kDvsAvoidOffsetX    = 0.8f;   // DVS规避固定方向位移（X）
    static constexpr float  kDvsAvoidOffsetY    = 0.0f;   // DVS规避固定方向位移（Y）
    static constexpr float  kDvsAvoidHoldSec    = 0.5f;   // DVS规避后停留时间
    static constexpr float  kDvsMoveTimeoutSec  = 4.0f;   // DVS位移动作超时
    static constexpr float  kDvsMoveStableSec   = 0.1f;   // DVS位移动作稳定时间
    static constexpr float  kLandVz           = -0.20f;   // 降落速度
    static constexpr float  kLandDuration     = 5.0f;     // 降落持续秒

    // ===== 成员组 =====
    FlightController& fc_;
    IStateProvider&   state_;
    IVisionProvider&  vision_;
    IDvsAvoidProvider& dvs_;
    ICommandPublisher& cmd_;
    rclcpp::Logger    logger_;

    float default_altitude_;
    State current_state_{State::TAKEOFF};

    Target takeoff_target_;  // 起飞目标点（高度 = default_altitude）
    Target hover_target_;    // 悬停基准点（规避后返回）

    rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
    rclcpp::Time  hover_start_time_{0, 0, RCL_STEADY_TIME};
    rclcpp::Time  last_avoid_time_{0, 0, RCL_STEADY_TIME};
    bool          hover_initialized_{false};
};
