#include <rclcpp/rclcpp.hpp>
#include "layer4_system/drone_system.hpp"

/**
 * @file main.cpp
 * @brief 程序入口
 */
int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    DroneSystem system;
    system.run();
    rclcpp::shutdown();
    return 0;
}
