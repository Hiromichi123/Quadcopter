#pragma once

#include "layer0_common/drone_state.hpp"

/**
 * @brief 无人机状态读取接口（纯虚）
 * FlightController 和 MissionExecutor 获取底层状态的唯一依赖
 */
class IStateProvider {
public:
    virtual ~IStateProvider() = default;

    // 获取当前无人机状态快照
    [[nodiscard]] virtual DroneState get_state() const = 0;

    // 是否已收到至少一帧有效状态数据
    [[nodiscard]] virtual bool has_state() const = 0;
};
