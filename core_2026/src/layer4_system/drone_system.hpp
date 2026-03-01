#pragma once

#include <memory>
#include <thread>
#include <rclcpp/rclcpp.hpp>

#include "layer1_hal/drone_hal.hpp"
#include "layer2_control/flight_controller.hpp"
#include "layer3_mission/mission_executor.hpp"

/**
 * @brief 系统编排层（System Orchestration）
 *
 * 1. 所有创建对象与全体生命周期管理
 * 2. spin 线程start与join
 * 3. 调用 pre_flight_checks()：起飞前检查（arming + OFFBOARD 模式切换），以及开始 mission.run()
 */
class DroneSystem {
public:
    DroneSystem();
    ~DroneSystem();

    void run(); // 起飞前准备阶段：pre_flight → mission（阻塞）

private:
    void pre_flight_checks(); // 起飞前检查（arming + OFFBOARD）

    std::shared_ptr<DroneHAL>          hal_;     // HAL-ROS2 node，提供接口实现
    std::unique_ptr<FlightController>  fc_;      // flight controller node，依赖 HAL 接口
    std::unique_ptr<MissionExecutor>   mission_; // mission executor，依赖 flight controller + HAL 接口

    std::shared_ptr<std::thread> spin_thread_;   // spin 线程
};
