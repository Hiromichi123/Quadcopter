#pragma once

#include <mutex>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <mavros_msgs/srv/command_bool.hpp>
#include <mavros_msgs/srv/command_long.hpp>
#include <mavros_msgs/srv/set_mode.hpp>
#include <mavros_msgs/msg/state.hpp>
#include <std_msgs/msg/string.hpp>

#include "ros2_tools/msg/lidar_pose.hpp"
#include "messages/msg/platform_target.hpp"
#include "messages/msg/vision.hpp"

#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_command_publisher.hpp"
#include "layer1_hal/i_vision_provider.hpp"
#include "layer1_hal/i_dvs_avoid_provider.hpp"

/**
 * @brief 硬件抽象层（Layer 1 · HAL Concrete）
 *
 * 整个系统直接操作 ROS2 通信原语（pub/sub/client）的中间层。
 * 实现三个接口：
 *   - IStateProvider    — 位姿状态
 *   - ICommandPublisher — 收发指令
 *   - IVisionProvider   — 视觉结果
 * 
 *   - 对外暴露的 MAVRos服务 仅供 DroneSystem 的预飞行阶段。
 */
class DroneHAL
    : public rclcpp::Node
    , public IStateProvider
    , public ICommandPublisher
    , public IVisionProvider
    , public IDvsAvoidProvider
{
public:
    explicit DroneHAL();

    // 状态提供接口 IStateProvider
    [[nodiscard]] DroneState get_state() const override;
    [[nodiscard]] bool       has_state() const override;

    // 指令发布接口 ICommandPublisher
    void publish_position(Target& target)   override;
    void publish_velocity(Velocity& velocity) override;
    [[nodiscard]] bool uses_planar_position_control() const override;

    // 视觉结果提供接口 IVisionProvider
    [[nodiscard]] messages::msg::Vision get_vision() const override;
    [[nodiscard]] bool                   has_vision() const override;

    // DVS 规避提供接口 IDvsAvoidProvider
    [[nodiscard]] geometry_msgs::msg::Twist get_dvs_avoid_cmd() const override;
    [[nodiscard]] bool has_recent_dvs_avoid(double max_age_sec) const override;
    [[nodiscard]] int64_t get_last_dvs_detect_time_ns() const override;

    // MAVROS 服务接口（供 DroneSystem 触发）
    bool request_arm(bool arm = true); // 请求px4解锁并等待响应，返回飞控是否接受
    bool request_set_mode(const std::string& mode); // 请求切换px4模式并等待响应，返回飞控是否接受

    // 查询 MAVROS 状态
    [[nodiscard]] mavros_msgs::msg::State get_mavros_state() const;
    [[nodiscard]] bool requires_mavros_preflight() const;
    [[nodiscard]] std::string get_platform_mode_name() const;

private:
    enum class PlatformMode {
        Px4MavrosDrone,
        Px4MavrosDiffCar,
        CustomAckermannCar
    };

    // ===== 回调组 =====
    void lidar_cb(const ros2_tools::msg::LidarPose::SharedPtr msg);
    void state_cb(const mavros_msgs::msg::State::SharedPtr msg);
    void vision_cb(const messages::msg::Vision::SharedPtr msg);
    void dvs_detection_cb(const std_msgs::msg::String::SharedPtr msg);
    void dvs_avoid_cb(const geometry_msgs::msg::Twist::SharedPtr msg);

    static bool extract_json_int64(const std::string& json, const std::string& key, int64_t& out);
    static bool extract_json_bool(const std::string& json, const std::string& key, bool& out);
    static PlatformMode parse_platform_mode(const std::string& mode_name);
    static float normalize_angle(float angle_rad);
    void log_dvs_pipeline_latency_if_applicable(const char* command_type);
    void publish_px4_drone_position(Target& target);
    void publish_px4_drone_velocity(Velocity& velocity);
    void publish_car_position_target(const Target& target, bool use_custom_ackermann);
    void publish_car_velocity_target(Velocity& velocity, bool use_custom_ackermann);
    messages::msg::PlatformTarget make_platform_target_from_position(const Target& target) const;
    messages::msg::PlatformTarget make_platform_target_from_velocity(const Velocity& velocity) const;

    // ===== 发布器组 =====
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr  pos_pub_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr vel_pub_;
    rclcpp::Publisher<messages::msg::PlatformTarget>::SharedPtr    platform_target_pub_;

    // ===== 订阅器 =====
    rclcpp::Subscription<ros2_tools::msg::LidarPose>::SharedPtr    lidar_sub_;
    rclcpp::Subscription<mavros_msgs::msg::State>::SharedPtr       state_sub_;
    rclcpp::Subscription<messages::msg::Vision>::SharedPtr        vision_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr         dvs_detection_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr     dvs_avoid_sub_;

    // ===== 服务客户端 =====
    rclcpp::Client<mavros_msgs::srv::CommandBool>::SharedPtr arming_client_;
    rclcpp::Client<mavros_msgs::srv::CommandLong>::SharedPtr command_client_;
    rclcpp::Client<mavros_msgs::srv::SetMode>::SharedPtr     set_mode_client_;

    // 结果状态
    mutable std::mutex state_mutex_;
    DroneState         state_{};
    bool               has_state_{false};

    // vision结果
    mutable std::mutex     vision_mutex_;
    messages::msg::Vision vision_{};
    bool                   has_vision_{false};

    // DVS 规避结果
    mutable std::mutex          dvs_mutex_;
    geometry_msgs::msg::Twist   dvs_avoid_cmd_{};
    rclcpp::Time                dvs_avoid_rx_time_{0, 0, RCL_SYSTEM_TIME};
    bool                        has_dvs_avoid_{false};
    int64_t                     last_dvs_detect_time_ns_{0};
    int64_t                     last_latency_logged_detect_ns_{0};

    // MAVRos状态
    mutable std::mutex      mavros_mutex_;
    mavros_msgs::msg::State mavros_state_{};

    PlatformMode platform_mode_{PlatformMode::Px4MavrosDrone};
    std::string platform_mode_name_{"px4_drone"};
    std::string position_setpoint_topic_{"/mavros/setpoint_position/local"};
    std::string velocity_setpoint_topic_{"/mavros/setpoint_velocity/cmd_vel"};
    std::string platform_target_topic_{"/platform/target"};
    float car_position_kp_speed_{0.8f};
    float car_yaw_kp_{1.5f};
    float car_max_speed_mps_{0.6f};
    float car_max_yaw_rate_radps_{1.0f};
    float car_xy_tolerance_m_{0.08f};
};
