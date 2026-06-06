#pragma once

#include <mavconn/mavlink_dialect.hpp>

#include <algorithm>
#include <cstdint>

namespace smart_car_bridge::mavlink_dialect
{

constexpr mavlink::msgid_t MSG_ID_SMART_CAR_CONTROL_SETPOINT = 42000;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_STATUS = 42001;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_MOTION_STATE = 42002;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_MOTOR_STATUS = 42003;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_IMU_STATUS = 42004;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_ACTUATOR_TEST = 42005;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_CALIB_STATUS = 42006;
constexpr mavlink::msgid_t MSG_ID_SMART_CAR_COMMAND = 42007;

constexpr uint8_t CRC_SMART_CAR_CONTROL_SETPOINT = 134;
constexpr uint8_t CRC_SMART_CAR_ACTUATOR_TEST = 105;
constexpr uint8_t CRC_SMART_CAR_COMMAND = 145;

constexpr uint8_t SMART_CAR_MODE_MANUAL = 1;
constexpr uint16_t SMART_CAR_CONTROL_FLAG_ENABLE = 1;
constexpr uint16_t SMART_CAR_CONTROL_FLAG_BRAKE = 2;
constexpr uint16_t SMART_CAR_COMMAND_STOP = 3;

struct ControlSetpoint
{
  uint32_t time_boot_ms{};
  float target_speed_mps{};
  float target_curvature{};
  float target_yaw_rate_dps{};
  float target_accel_mps2{};
  uint16_t seq{};
  uint16_t flags{};
  uint8_t target_system{};
  uint8_t target_component{};
  uint8_t mode{};
};

struct Status
{
  uint32_t time_boot_ms{};
  uint32_t fault_flags{};
  uint32_t warn_flags{};
  uint16_t cmd_age_ms{};
  uint16_t control_loop_hz{};
  uint8_t mode{};
  uint8_t state{};
  uint8_t imu_online{};
  uint8_t can_online{};
  uint8_t host_online{};
  uint8_t servo_online{};
  uint8_t motor_online_mask{};
};

struct MotionState
{
  uint32_t time_boot_ms{};
  float speed_mps{};
  float target_speed_mps{};
  float yaw_rate_dps{};
  float yaw_deg{};
  float curvature_meas{};
  float curvature_cmd{};
  float steering_angle_deg{};
  uint16_t steering_pwm_us{};
  uint8_t steering_clamped{};
};

struct MotorStatus
{
  uint32_t time_boot_ms{};
  uint32_t can_tx_busy_count{};
  uint32_t can_error_count{};
  int16_t target_rpm_1{};
  int16_t target_rpm_2{};
  int16_t actual_rpm_1{};
  int16_t actual_rpm_2{};
  int16_t current_cmd_1{};
  int16_t current_cmd_2{};
  int16_t feedback_current_1{};
  int16_t feedback_current_2{};
  uint16_t angle_1{};
  uint16_t angle_2{};
  uint8_t online_mask{};
};

struct ImuStatus
{
  uint32_t time_boot_ms{};
  uint32_t sample_count{};
  uint32_t overrun_count{};
  uint32_t error_count{};
  int32_t gyro_x_mdps{};
  int32_t gyro_y_mdps{};
  int32_t gyro_z_mdps{};
  float yaw_rate_raw_dps{};
  float yaw_rate_dps{};
  float gyro_bias_z_dps{};
  int16_t accel_x_mg{};
  int16_t accel_y_mg{};
  int16_t accel_z_mg{};
  int16_t temperature_c_x100{};
  uint8_t calibrated{};
};

struct ActuatorTest
{
  uint32_t time_boot_ms{};
  float servo_angle_deg{};
  uint16_t test_mask{};
  uint16_t servo_pwm_us{};
  uint16_t duration_ms{};
  int16_t motor1_rpm{};
  int16_t motor2_rpm{};
  uint8_t target_system{};
  uint8_t target_component{};
};

struct CalibStatus
{
  uint32_t time_boot_ms{};
  uint32_t point_id{};
  uint32_t sweep_index{};
  uint32_t sweep_count{};
  uint32_t valid_count{};
  uint32_t invalid_count{};
  float v_center_avg{};
  float yaw_rate_avg{};
  float kappa_avg{};
  float radius_est{};
  int16_t target_rpm{};
  uint16_t servo_pwm_us{};
  uint8_t state{};
  uint8_t sweep_enabled{};
  uint8_t yaw_sign_inverted{};
};

struct Command
{
  uint32_t time_boot_ms{};
  float param1{};
  float param2{};
  float param3{};
  float param4{};
  uint16_t command{};
  uint8_t target_system{};
  uint8_t target_component{};
};

inline void finalize(
  mavlink::mavlink_message_t & msg,
  uint8_t system_id,
  uint8_t component_id,
  uint8_t length,
  uint8_t crc_extra)
{
  mavlink::mavlink_finalize_message(&msg, system_id, component_id, length, length, crc_extra);
  msg.magic = 253;
}

inline mavlink::mavlink_message_t pack_control_setpoint(
  const ControlSetpoint & data,
  uint8_t system_id,
  uint8_t component_id)
{
  mavlink::mavlink_message_t msg{};
  mavlink::MsgMap map(msg);
  map.reset(MSG_ID_SMART_CAR_CONTROL_SETPOINT, 27);
  map << data.time_boot_ms;
  map << data.target_speed_mps;
  map << data.target_curvature;
  map << data.target_yaw_rate_dps;
  map << data.target_accel_mps2;
  map << data.seq;
  map << data.flags;
  map << data.target_system;
  map << data.target_component;
  map << data.mode;
  finalize(msg, system_id, component_id, 27, CRC_SMART_CAR_CONTROL_SETPOINT);
  return msg;
}

inline mavlink::mavlink_message_t pack_actuator_test(
  const ActuatorTest & data,
  uint8_t system_id,
  uint8_t component_id)
{
  mavlink::mavlink_message_t msg{};
  mavlink::MsgMap map(msg);
  map.reset(MSG_ID_SMART_CAR_ACTUATOR_TEST, 20);
  map << data.time_boot_ms;
  map << data.servo_angle_deg;
  map << data.test_mask;
  map << data.servo_pwm_us;
  map << data.duration_ms;
  map << data.motor1_rpm;
  map << data.motor2_rpm;
  map << data.target_system;
  map << data.target_component;
  finalize(msg, system_id, component_id, 20, CRC_SMART_CAR_ACTUATOR_TEST);
  return msg;
}

inline mavlink::mavlink_message_t pack_command(
  const Command & data,
  uint8_t system_id,
  uint8_t component_id)
{
  mavlink::mavlink_message_t msg{};
  mavlink::MsgMap map(msg);
  map.reset(MSG_ID_SMART_CAR_COMMAND, 24);
  map << data.time_boot_ms;
  map << data.param1;
  map << data.param2;
  map << data.param3;
  map << data.param4;
  map << data.command;
  map << data.target_system;
  map << data.target_component;
  finalize(msg, system_id, component_id, 24, CRC_SMART_CAR_COMMAND);
  return msg;
}

inline bool decode_status(const mavlink::mavlink_message_t & msg, Status & data)
{
  if (msg.msgid != MSG_ID_SMART_CAR_STATUS || msg.len < 23) {
    return false;
  }
  mavlink::MsgMap map(&msg);
  map >> data.time_boot_ms;
  map >> data.fault_flags;
  map >> data.warn_flags;
  map >> data.cmd_age_ms;
  map >> data.control_loop_hz;
  map >> data.mode;
  map >> data.state;
  map >> data.imu_online;
  map >> data.can_online;
  map >> data.host_online;
  map >> data.servo_online;
  map >> data.motor_online_mask;
  return true;
}

inline bool decode_motion_state(const mavlink::mavlink_message_t & msg, MotionState & data)
{
  if (msg.msgid != MSG_ID_SMART_CAR_MOTION_STATE || msg.len < 34) {
    return false;
  }
  mavlink::MsgMap map(&msg);
  map >> data.time_boot_ms;
  map >> data.speed_mps;
  map >> data.target_speed_mps;
  map >> data.yaw_rate_dps;
  map >> data.yaw_deg;
  map >> data.curvature_meas;
  map >> data.curvature_cmd;
  map >> data.steering_angle_deg;
  map >> data.steering_pwm_us;
  if (msg.len >= 35) {
    map >> data.steering_clamped;
  } else {
    data.steering_clamped = 0;
  }
  return true;
}

inline bool decode_motor_status(const mavlink::mavlink_message_t & msg, MotorStatus & data)
{
  if (msg.msgid != MSG_ID_SMART_CAR_MOTOR_STATUS || msg.len < 33) {
    return false;
  }
  mavlink::MsgMap map(&msg);
  map >> data.time_boot_ms;
  map >> data.can_tx_busy_count;
  map >> data.can_error_count;
  map >> data.target_rpm_1;
  map >> data.target_rpm_2;
  map >> data.actual_rpm_1;
  map >> data.actual_rpm_2;
  map >> data.current_cmd_1;
  map >> data.current_cmd_2;
  map >> data.feedback_current_1;
  map >> data.feedback_current_2;
  map >> data.angle_1;
  map >> data.angle_2;
  map >> data.online_mask;
  return true;
}

inline bool decode_imu_status(const mavlink::mavlink_message_t & msg, ImuStatus & data)
{
  if (msg.msgid != MSG_ID_SMART_CAR_IMU_STATUS || msg.len < 49) {
    return false;
  }
  mavlink::MsgMap map(&msg);
  map >> data.time_boot_ms;
  map >> data.sample_count;
  map >> data.overrun_count;
  map >> data.error_count;
  map >> data.gyro_x_mdps;
  map >> data.gyro_y_mdps;
  map >> data.gyro_z_mdps;
  map >> data.yaw_rate_raw_dps;
  map >> data.yaw_rate_dps;
  map >> data.gyro_bias_z_dps;
  map >> data.accel_x_mg;
  map >> data.accel_y_mg;
  map >> data.accel_z_mg;
  map >> data.temperature_c_x100;
  map >> data.calibrated;
  return true;
}

inline bool decode_calib_status(const mavlink::mavlink_message_t & msg, CalibStatus & data)
{
  if (msg.msgid != MSG_ID_SMART_CAR_CALIB_STATUS || msg.len < 47) {
    return false;
  }
  mavlink::MsgMap map(&msg);
  map >> data.time_boot_ms;
  map >> data.point_id;
  map >> data.sweep_index;
  map >> data.sweep_count;
  map >> data.valid_count;
  map >> data.invalid_count;
  map >> data.v_center_avg;
  map >> data.yaw_rate_avg;
  map >> data.kappa_avg;
  map >> data.radius_est;
  map >> data.target_rpm;
  map >> data.servo_pwm_us;
  map >> data.state;
  map >> data.sweep_enabled;
  map >> data.yaw_sign_inverted;
  return true;
}

}  // namespace smart_car_bridge::mavlink_dialect
