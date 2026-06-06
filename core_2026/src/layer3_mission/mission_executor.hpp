#pragma once

#include <rclcpp/rclcpp.hpp>

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
                    float              default_altitude = 1.2f);

    void run(); // 开始执行任务

private:
    // ===== 状态机组 =====
    enum class State {
        TAKEOFF,       // 原地上升至目标高度
        HOVER,         // 持续发布原地悬停目标
        LAND,          // 缓慢降落
        DONE           // 任务完成
    };

    // ===== 状态方法组 =====
    void on_takeoff();
    void on_hover();
    void on_land();

    // ===== 具名常量组 =====
    static constexpr float  kHoverAltitude      = 1.70f;  // 原地悬停目标高度
    static constexpr float  kHoverDurationSec   = 50.0f;  // 持续发布悬停目标时长
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

    Target takeoff_target_;  // 起飞目标点（高度 = 1.70m）
    Target hover_target_;    // 原地悬停目标

    rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
    rclcpp::Time  hover_start_time_{0, 0, RCL_STEADY_TIME};
    float         hover_anchor_x_{0.0f};
    float         hover_anchor_y_{0.0f};
    float         hover_anchor_yaw_{0.0f};
};
