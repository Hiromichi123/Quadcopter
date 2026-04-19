#pragma once

#include "messages/msg/vision.hpp"

/**
 * @brief 视觉数据访问接口（纯虚）
 * MissionExecutor 通过此接口读取视觉检测结果，
 */
class IVisionProvider {
public:
    virtual ~IVisionProvider() = default;

    // 获取最新视觉消息
    [[nodiscard]] virtual messages::msg::Vision get_vision() const = 0;

    // 是否已收到至少一帧视觉数据
    [[nodiscard]] virtual bool has_vision() const = 0;
};
