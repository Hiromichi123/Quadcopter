#pragma once

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <cmath>

/**
 * @brief 位置目标点类
 */
class Target {
public:
    Target() = default;

    Target(float x, float y, float z, float yaw) {
        pose_.header.frame_id = "map";
        set_position(x, y, z);
        set_yaw(yaw);
    }
    
    // 从 PoseStamped 快速显式构造（防止隐式转换）
    explicit Target(const geometry_msgs::msg::PoseStamped& pose) : pose_(pose) {}

    // 读取
    float get_x() const { return pose_.pose.position.x; }
    float get_y() const { return pose_.pose.position.y; }
    float get_z() const { return pose_.pose.position.z; }
    float get_yaw() const {
        float yaw = std::atan2(
            2.0f * pose_.pose.orientation.z * pose_.pose.orientation.w,
            1.0f - 2.0f * pose_.pose.orientation.z * pose_.pose.orientation.z);
        return (yaw < 0) ? yaw + 2.0f * static_cast<float>(M_PI) : yaw;
    }

    // 返回可发布的 PoseStamped
    const geometry_msgs::msg::PoseStamped& get_pose() const { return pose_; } // 常量引用，避免拷贝

    // 写入
    void set_position(float x, float y, float z) {
        pose_.pose.position.x = x;
        pose_.pose.position.y = y;
        pose_.pose.position.z = z;
    }
    void set_x(float x) { pose_.pose.position.x = x; }
    void set_y(float y) { pose_.pose.position.y = y; }
    void set_z(float z) { pose_.pose.position.z = z; }
    void set_yaw(float yaw) {
        pose_.pose.orientation.x = 0.0;
        pose_.pose.orientation.y = 0.0;
        pose_.pose.orientation.z = std::sin(yaw / 2.0f);
        pose_.pose.orientation.w = std::cos(yaw / 2.0f);
    }

    // 发布前更新时间戳
    void set_time(rclcpp::Time time) { pose_.header.stamp = time; }

private:
    geometry_msgs::msg::PoseStamped pose_{};
};
