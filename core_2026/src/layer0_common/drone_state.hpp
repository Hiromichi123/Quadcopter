#pragma once

/**
 * @brief 无人机状态 POD
 */
struct DroneState {
    float x{};   // 前方   (body frame forward / map-x)
    float y{};   // 左方   (body frame left  / map-y)
    float z{};   // 高度   (map-z, positive up)
    float yaw{}; // 偏航角 (rad)

    float vx{}, vy{}, vz{}; // 速度   (m/s)
    float roll{}, pitch{};  // 姿态角 (rad)
};
