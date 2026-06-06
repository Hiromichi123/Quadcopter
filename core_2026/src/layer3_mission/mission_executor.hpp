#pragma once

#include <rclcpp/rclcpp.hpp>
#include <cstddef>

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
        TAKEOFF,       // 原地上升至第一个高度刻度
        HOVER,         // 逐个高度刻度悬停
        LAND,          // 缓慢降落
        DONE           // 任务完成
    };

    // ===== 状态方法组 =====
    void on_takeoff();
    void on_hover();
    void on_land();
    bool is_near_hover_target(const DroneState& s) const;
    float hover_altitude_for_step(std::size_t step_index) const;
    void set_hover_step_target();

    // ===== 具名常量组 =====
    static constexpr float  kHoverStartAltitude = 1.70f;  // 第一个悬停刻度
    static constexpr float  kHoverAltitudeStep  = 0.10f;  // 高度刻度间隔
    static constexpr std::size_t kHoverStepCount = 6;      // 1.70m 到 2.20m，共 6 个刻度
    static constexpr float  kHoverHoldSec       = 8.0f;   // 每个高度刻度悬停时长
    static constexpr float  kHoverStablePosTolXY = 0.12f; // 悬停稳定判定 XY 误差门限
    static constexpr float  kHoverStablePosTolZ  = 0.10f; // 悬停稳定判定 Z 误差门限
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

    Target takeoff_target_;  // 起飞目标点（高度 = 第一个刻度）
    Target hover_target_;    // 当前高度刻度悬停目标

    rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
    rclcpp::Time  hover_step_start_time_{0, 0, RCL_STEADY_TIME};
    std::size_t   hover_step_index_{0};
    float         hover_anchor_x_{0.0f};
    float         hover_anchor_y_{0.0f};
    float         hover_anchor_yaw_{0.0f};
    bool          hover_initialized_{false};
};
