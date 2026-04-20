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
    bool is_near_hover_target(const DroneState& s) const;
    void update_dvs_accept_gate(const rclcpp::Time& now);

    // ===== 具名常量组 =====
    static constexpr float  kMissionDurationSec = 20.0f;  // 悬停阶段总时长
    static constexpr float  kDvsAvoidCooldownSec = 0.8f;  // DVS相邻规避最小间隔
    static constexpr float  kDvsCmdFreshSec     = 0.12f;  // DVS规避指令新鲜度门限
    static constexpr float  kDvsAvoidBackX      = -0.55f; // DVS规避后退位移（X）
    static constexpr float  kDvsAvoidSideY      = 0.45f;  // DVS规避侧向位移幅值（Y，按指令左右决定符号）
    static constexpr float  kDvsAvoidUpZ        = 0.30f;  // DVS规避上抬位移（Z）
    static constexpr float  kDvsAvoidL1Max      = 1.5f;   // DVS规避位移三轴累计上限 |dx|+|dy|+|dz|
    static constexpr float  kDvsMoveTimeoutSec  = 6.0f;   // DVS位移动作超时
    static constexpr float  kDvsMoveStableSec   = 0.05f;  // DVS位移动作稳定时间
    static constexpr float  kHoverStablePosTolXY = 0.12f; // 悬停稳定判定 XY 误差门限
    static constexpr float  kHoverStablePosTolZ  = 0.10f; // 悬停稳定判定 Z 误差门限
    static constexpr float  kHoverStableEnableSec = 1.0f; // 连续稳定悬停多久后才接收 DVS
    static constexpr float  kDvsRearmDelaySec    = 0.6f; // 规避回位后 DVS 解锁额外延时
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
    rclcpp::Time  hover_stable_since_{0, 0, RCL_STEADY_TIME};
    rclcpp::Time  dvs_block_until_{0, 0, RCL_STEADY_TIME};
    bool          hover_initialized_{false};
    bool          dvs_accept_enabled_{false};
};
