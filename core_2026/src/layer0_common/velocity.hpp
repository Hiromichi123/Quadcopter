#pragma once

#include <geometry_msgs/msg/twist_stamped.hpp>
#include <rclcpp/rclcpp.hpp>

/**
 * @brief 速度类
 */
class Velocity {
public:
    Velocity() = default;

    explicit Velocity(float vx, float vy, float vz,
                      float vyaw = 0.0f, float vpitch = 0.0f, float vroll = 0.0f) {
        ts_.twist.linear.x  = vx;
        ts_.twist.linear.y  = vy;
        ts_.twist.linear.z  = vz;
        ts_.twist.angular.z = vyaw;
        ts_.twist.angular.y = vpitch;
        ts_.twist.angular.x = vroll;
    }

    // 读取
    float get_vx()     const { return ts_.twist.linear.x; }
    float get_vy()     const { return ts_.twist.linear.y; }
    float get_vz()     const { return ts_.twist.linear.z; }
    float get_vyaw()   const { return ts_.twist.angular.z; }
    float get_vpitch() const { return ts_.twist.angular.y; }
    float get_vroll()  const { return ts_.twist.angular.x; }

    /// 返回可发布的 TwistStamped
    geometry_msgs::msg::TwistStamped get_twist() const { return ts_; }

    // 写入
    void set_vx(float v)     { ts_.twist.linear.x  = v; }
    void set_vy(float v)     { ts_.twist.linear.y  = v; }
    void set_vz(float v)     { ts_.twist.linear.z  = v; }
    void set_vyaw(float v)   { ts_.twist.angular.z = v; }
    void set_vpitch(float v) { ts_.twist.angular.y = v; }
    void set_vroll(float v)  { ts_.twist.angular.x = v; }

    /// 发布前更新时间戳
    void set_time(rclcpp::Time time) { ts_.header.stamp = time; }

protected:
    geometry_msgs::msg::TwistStamped ts_{};
};
