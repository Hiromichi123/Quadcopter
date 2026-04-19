#pragma once

#include <cstdint>
#include <geometry_msgs/msg/twist.hpp>

/**
 * @brief DVS 规避指令访问接口（纯虚）
 * MissionExecutor 通过此接口被动读取最新 DVS 规避指令。
 */
class IDvsAvoidProvider {
public:
    virtual ~IDvsAvoidProvider() = default;

    // 获取最新 DVS 规避速度指令。
    [[nodiscard]] virtual geometry_msgs::msg::Twist get_dvs_avoid_cmd() const = 0;

    // 是否存在足够新鲜的 DVS 规避指令。
    [[nodiscard]] virtual bool has_recent_dvs_avoid(double max_age_sec) const = 0;

    // 最近一次“检测到球体”的时间戳（ns）。无有效值时返回 <= 0。
    [[nodiscard]] virtual int64_t get_last_dvs_detect_time_ns() const = 0;
};
