#pragma once

#include <mutex>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <mavros_msgs/srv/command_bool.hpp>
#include <mavros_msgs/srv/command_long.hpp>
#include <mavros_msgs/srv/set_mode.hpp>
#include <mavros_msgs/msg/state.hpp>

#include "ros2_tools/msg/lidar_pose.hpp"
#include "vision_py/msg/vision.hpp"

#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_command_publisher.hpp"
#include "layer1_hal/i_vision_provider.hpp"

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
{
public:
    explicit DroneHAL();

    // 状态提供接口 IStateProvider
    [[nodiscard]] DroneState get_state() const override;
    [[nodiscard]] bool       has_state() const override;

    // 指令发布接口 ICommandPublisher
    void publish_position(Target& target)   override;
    void publish_velocity(Velocity& velocity) override;

    // 视觉结果提供接口 IVisionProvider
    [[nodiscard]] vision_py::msg::Vision get_vision() const override;
    [[nodiscard]] bool                   has_vision() const override;

    // MAVROS 服务接口（供 DroneSystem 触发）
    bool request_arm(bool arm = true); // 请求px4解锁（非阻塞）
    bool request_set_mode(const std::string& mode); // 请求切换px4模式"OFFBOARD"(非阻塞)

    // 查询 MAVROS 状态
    [[nodiscard]] mavros_msgs::msg::State get_mavros_state() const;

private:
    // ===== 回调组 =====
    void lidar_cb(const ros2_tools::msg::LidarPose::SharedPtr msg);
    void state_cb(const mavros_msgs::msg::State::SharedPtr msg);
    void vision_cb(const vision_py::msg::Vision::SharedPtr msg);

    // ===== 发布器组 =====
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr  pos_pub_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr vel_pub_;

    // ===== 订阅器 =====
    rclcpp::Subscription<ros2_tools::msg::LidarPose>::SharedPtr    lidar_sub_;
    rclcpp::Subscription<mavros_msgs::msg::State>::SharedPtr       state_sub_;
    rclcpp::Subscription<vision_py::msg::Vision>::SharedPtr        vision_sub_;

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
    vision_py::msg::Vision vision_{};
    bool                   has_vision_{false};

    // MAVRos状态
    mutable std::mutex      mavros_mutex_;
    mavros_msgs::msg::State mavros_state_{};
};
