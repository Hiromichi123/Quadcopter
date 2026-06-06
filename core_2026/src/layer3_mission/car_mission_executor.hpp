#pragma once

#include <rclcpp/rclcpp.hpp>
#include <messages/msg/smart_car_control_setpoint.hpp>
#include <messages/msg/smart_car_motion_state.hpp>
#include <memory>

namespace car_mission {

/**
 * @brief 小车任务执行器
 * 
 * 实现操场式轨迹：直边(1m) + 半圆(R=0.5m) x3
 * 
 * 轨迹分解：
 * - 直边：1米直线，曲率κ=0，速度0.5 m/s
 * - 半圆：半径0.5m的顺时针半圆，曲率κ=-2.0 (1/m)，速度0.5 m/s
 * - 重复3次
 */
class CarMissionExecutor : public rclcpp::Node {
public:
    CarMissionExecutor();
    ~CarMissionExecutor() override = default;

private:
    // ===== 状态机 =====
    enum class MissionState {
        INIT,           // 初始化
        STRAIGHT_1,     // 直边1
        CURVE_1,        // 半圆1
        STRAIGHT_2,     // 直边2
        CURVE_2,        // 半圆2
        STRAIGHT_3,     // 直边3
        CURVE_3,        // 半圆3
        DONE,           // 任务完成
        ERROR           // 错误
    };

    // ===== 定时器回调 =====
    void on_mission_timer();
    void on_motion_state_feedback(const void * msg);

    // ===== 状态处理方法 =====
    void handle_state(MissionState state);
    void send_control_command(float speed, float curvature);
    void transition_state(MissionState next_state);

    // ===== 轨迹计算 =====
    bool is_straight_segment_done();  // 判断直边完成
    bool is_curve_segment_done();     // 判断曲线完成

    // ===== 常量 =====
    static constexpr float kStraightLength = 1.0f;      // 直边长度 (m)
    static constexpr float kCurveRadius = 0.5f;         // 曲线半径 (m)
    static constexpr float kCurveKappa = -2.0f;         // 顺时针曲线曲率 (1/m) = -1/R
    static constexpr float kTargetSpeed = 0.5f;         // 目标速度 (m/s)
    static constexpr float kDistanceTolerance = 0.05f;  // 距离容差 (m)
    static constexpr int kMaxMissionCircles = 3;        // 最大圈数

    // ===== 成员变量 =====
    MissionState current_state_{MissionState::INIT};
    int circle_count_{0};                               // 当前圈数
    float distance_traveled_{0.0f};                     // 该段已行驶距离 (m)
    rclcpp::Time segment_start_time_;                   // 段开始时间
    float current_speed_{0.0f};                         // 当前速度 (m/s)

    // ===== ROS接口 =====
    rclcpp::Subscription<messages::msg::SmartCarMotionState>::SharedPtr motion_sub_;
    rclcpp::Publisher<messages::msg::SmartCarControlSetpoint>::SharedPtr control_pub_;
    rclcpp::TimerBase::SharedPtr mission_timer_;

    // ===== 日志 =====
    rclcpp::Logger logger_ = get_logger();
};

} // namespace car_mission
