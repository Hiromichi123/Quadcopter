#pragma once

#include <rclcpp/rclcpp.hpp>
#include <array>
#include <cstddef>
#include <memory>

#include "layer2_control/flight_controller.hpp"
#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_vision_provider.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

/**
 * @brief 任务执行层
 *
 * 将原 quadcopter::main_loop() 中的状态机完整提取为独立类：
 *   TAKEOFF → FORWARD → LINE_FOLLOW → ALIGN_SHAPE
 *           → RETURN_LINE → LINE_FOLLOW → ALIGN_LAND → LAND → DONE
 *
 * 改进：
 * - 枚举类替代裸 int flag，含义清晰、不可越界赋值。
 * - 每个状态对应一个私有方法，避免巨型 switch-case。
 * - 使用具名常量替代魔法数字（1000.0、20、5.0 等）。
 * - 通过接口获取状态和视觉数据，不依赖任何具体硬件类。
 */
class MissionExecutor {
public:
    MissionExecutor(FlightController&  fc,     // 飞行控制器
                    IStateProvider&    state,  // 状态接口
                    IVisionProvider&   vision, // 视觉提供接口
                    rclcpp::Logger     logger, // DroneHAL日志记录器
                    float              default_altitude = 1.5f);

    void run(); // 开始执行任务

private:
    // ===== 状态机组 =====
    enum class State {
        TAKEOFF,       // 上升至指定高度
        FORWARD,       // 前进直到发现巡线
        LINE_FOLLOW,   // 视觉巡线
        ALIGN_SHAPE,   // 对准目标形状（投掷）
        RETURN_LINE,   // 返回巡线起点，继续巡线
        ALIGN_LAND,    // 对准降落区域
        LAND,          // 缓慢降落
        DONE           // 任务完成
    };

    // ===== 状态方法组 =====
    void on_takeoff();
    void on_forward();
    void on_line_follow();
    void on_align_shape();
    void on_return_line();
    void on_align_land();
    void on_land();

    // ===== 具名常量组 =====
    static constexpr float  kForwardVx        = 0.10f;    // 巡线前速度
    static constexpr float  kFollowVx         = 0.15f;    // 巡线 X 速度
    static constexpr float  kLateralScale     = -1000.0f; // 横向误差比例
    static constexpr float  kAngleScale       = 5.0f;     // 角度误差比例
    static constexpr float  kAlignThreshold   = 20.0f;    // 对准阈值 (像素)
    static constexpr float  kAltDeadband      = 0.05f;    // 高度死区
    static constexpr float  kLandVz           = -0.20f;   // 降落速度
    static constexpr float  kLandDuration     = 5.0f;     // 降落持续秒
    static constexpr int    kWaypointCount    = 5;        // 航点数量

    // ===== 成员组 =====
    FlightController& fc_;
    IStateProvider&   state_;
    IVisionProvider&  vision_;
    rclcpp::Logger    logger_;

    float default_altitude_;
    bool  is_cast_complete_{false};
    State current_state_{State::TAKEOFF};

    Target  takeoff_target_;   // 起飞目标点（高度 = default_altitude）
    Target  shape_return_pos_; // 发现形状时记录的位置（用于 RETURN_LINE）
    Velocity vel_follow_;      // 巡线速度指令（复用，避免重复构造）

    std::array<Target, kWaypointCount> waypoints_{};
    std::size_t waypoint_index_{0};
    bool waypoints_initialized_{false};
};
