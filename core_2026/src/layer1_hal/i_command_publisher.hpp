#pragma once

#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

/**
 * @brief 无人机指令发布接口（纯虚）
 * FlightController 通过此接口下发 pose/vel cmd
 */
class ICommandPublisher {
public:
    virtual ~ICommandPublisher() = default;

    // 发布位置设定点（PoseStamped topic）
    virtual void publish_position(Target& target) = 0;

    // 发布速度设定点（TwistStamped topic）
    virtual void publish_velocity(Velocity& velocity) = 0;
};
