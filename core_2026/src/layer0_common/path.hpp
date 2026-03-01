#pragma once

#include <vector>
#include <iostream>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include "target.hpp"

/**
 * @brief 航点路径容器
 */
class Path {
public:
    /// 添加航点（PoseStamped 版）
    void add_waypoint(const geometry_msgs::msg::PoseStamped& pose) {
        waypoints_.emplace_back(pose);
    }

    /// 添加航点（Target 版）
    void add_waypoint(const Target& target) {
        waypoints_.push_back(target);
    }

    /// 添加航点（直接参数版）
    void add_waypoint(float x, float y, float z, float yaw = 0.0f) {
        waypoints_.emplace_back(x, y, z, yaw);
    }

    /// 按索引删除航点
    void remove_waypoint(size_t index) {
        if (waypoints_.empty()) {
            std::cerr << "[Path] 路径已全部删除！\n";
            return;
        }
        if (index < waypoints_.size()) {
            waypoints_.erase(waypoints_.begin() + static_cast<ptrdiff_t>(index));
        } else {
            std::cerr << "[Path] 非法航点索引: " << index << '\n';
        }
    }

    /**
     * @brief 取得下一个航点。
     * @param[out] waypoint 填充目标点。
     * @return true  = 成功取出，false = 已全部遍历，索引重置。
     */
    bool get_next_waypoint(Target& waypoint) {
        if (current_index_ < waypoints_.size()) {
            waypoint = waypoints_[current_index_++];
            return true;
        }
        current_index_ = 0;
        return false;
    }

    void reset() { current_index_ = 0; }
    [[nodiscard]] bool empty() const { return waypoints_.empty(); }
    [[nodiscard]] size_t size() const { return waypoints_.size(); }

private:
    std::vector<Target> waypoints_;
    size_t current_index_{0};
};
