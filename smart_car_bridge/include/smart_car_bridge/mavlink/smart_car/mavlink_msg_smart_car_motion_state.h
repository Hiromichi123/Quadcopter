#pragma once
// MESSAGE SMART_CAR_MOTION_STATE PACKING

#define MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE 42002


typedef struct __mavlink_smart_car_motion_state_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the vehicle.*/
 float speed_mps; /*< [m/s] Estimated vehicle speed.*/
 float target_speed_mps; /*< [m/s] Current target speed.*/
 float yaw_rate_dps; /*< [deg/s] Filtered yaw rate.*/
 float yaw_deg; /*< [deg] Integrated debug yaw.*/
 float curvature_meas; /*< [1/m] Measured curvature.*/
 float curvature_cmd; /*< [1/m] Commanded curvature.*/
 float steering_angle_deg; /*< [deg] Steering servo angle.*/
 uint16_t steering_pwm_us; /*< [us] Steering PWM pulse width.*/
 uint8_t steering_clamped; /*<  Steering clamp flag.*/
} mavlink_smart_car_motion_state_t;

#define MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN 35
#define MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN 35
#define MAVLINK_MSG_ID_42002_LEN 35
#define MAVLINK_MSG_ID_42002_MIN_LEN 35

#define MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC 239
#define MAVLINK_MSG_ID_42002_CRC 239



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_MOTION_STATE { \
    42002, \
    "SMART_CAR_MOTION_STATE", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_motion_state_t, time_boot_ms) }, \
         { "speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_motion_state_t, speed_mps) }, \
         { "target_speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_motion_state_t, target_speed_mps) }, \
         { "yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_motion_state_t, yaw_rate_dps) }, \
         { "yaw_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_motion_state_t, yaw_deg) }, \
         { "curvature_meas", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_smart_car_motion_state_t, curvature_meas) }, \
         { "curvature_cmd", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_smart_car_motion_state_t, curvature_cmd) }, \
         { "steering_angle_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_motion_state_t, steering_angle_deg) }, \
         { "steering_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 32, offsetof(mavlink_smart_car_motion_state_t, steering_pwm_us) }, \
         { "steering_clamped", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_smart_car_motion_state_t, steering_clamped) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_MOTION_STATE { \
    "SMART_CAR_MOTION_STATE", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_motion_state_t, time_boot_ms) }, \
         { "speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_motion_state_t, speed_mps) }, \
         { "target_speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_motion_state_t, target_speed_mps) }, \
         { "yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_motion_state_t, yaw_rate_dps) }, \
         { "yaw_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_motion_state_t, yaw_deg) }, \
         { "curvature_meas", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_smart_car_motion_state_t, curvature_meas) }, \
         { "curvature_cmd", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_smart_car_motion_state_t, curvature_cmd) }, \
         { "steering_angle_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_motion_state_t, steering_angle_deg) }, \
         { "steering_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 32, offsetof(mavlink_smart_car_motion_state_t, steering_pwm_us) }, \
         { "steering_clamped", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_smart_car_motion_state_t, steering_clamped) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_motion_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param speed_mps [m/s] Estimated vehicle speed.
 * @param target_speed_mps [m/s] Current target speed.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param yaw_deg [deg] Integrated debug yaw.
 * @param curvature_meas [1/m] Measured curvature.
 * @param curvature_cmd [1/m] Commanded curvature.
 * @param steering_angle_deg [deg] Steering servo angle.
 * @param steering_pwm_us [us] Steering PWM pulse width.
 * @param steering_clamped  Steering clamp flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float speed_mps, float target_speed_mps, float yaw_rate_dps, float yaw_deg, float curvature_meas, float curvature_cmd, float steering_angle_deg, uint16_t steering_pwm_us, uint8_t steering_clamped)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, speed_mps);
    _mav_put_float(buf, 8, target_speed_mps);
    _mav_put_float(buf, 12, yaw_rate_dps);
    _mav_put_float(buf, 16, yaw_deg);
    _mav_put_float(buf, 20, curvature_meas);
    _mav_put_float(buf, 24, curvature_cmd);
    _mav_put_float(buf, 28, steering_angle_deg);
    _mav_put_uint16_t(buf, 32, steering_pwm_us);
    _mav_put_uint8_t(buf, 34, steering_clamped);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#else
    mavlink_smart_car_motion_state_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.speed_mps = speed_mps;
    packet.target_speed_mps = target_speed_mps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.yaw_deg = yaw_deg;
    packet.curvature_meas = curvature_meas;
    packet.curvature_cmd = curvature_cmd;
    packet.steering_angle_deg = steering_angle_deg;
    packet.steering_pwm_us = steering_pwm_us;
    packet.steering_clamped = steering_clamped;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
}

/**
 * @brief Pack a smart_car_motion_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param speed_mps [m/s] Estimated vehicle speed.
 * @param target_speed_mps [m/s] Current target speed.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param yaw_deg [deg] Integrated debug yaw.
 * @param curvature_meas [1/m] Measured curvature.
 * @param curvature_cmd [1/m] Commanded curvature.
 * @param steering_angle_deg [deg] Steering servo angle.
 * @param steering_pwm_us [us] Steering PWM pulse width.
 * @param steering_clamped  Steering clamp flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float speed_mps, float target_speed_mps, float yaw_rate_dps, float yaw_deg, float curvature_meas, float curvature_cmd, float steering_angle_deg, uint16_t steering_pwm_us, uint8_t steering_clamped)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, speed_mps);
    _mav_put_float(buf, 8, target_speed_mps);
    _mav_put_float(buf, 12, yaw_rate_dps);
    _mav_put_float(buf, 16, yaw_deg);
    _mav_put_float(buf, 20, curvature_meas);
    _mav_put_float(buf, 24, curvature_cmd);
    _mav_put_float(buf, 28, steering_angle_deg);
    _mav_put_uint16_t(buf, 32, steering_pwm_us);
    _mav_put_uint8_t(buf, 34, steering_clamped);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#else
    mavlink_smart_car_motion_state_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.speed_mps = speed_mps;
    packet.target_speed_mps = target_speed_mps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.yaw_deg = yaw_deg;
    packet.curvature_meas = curvature_meas;
    packet.curvature_cmd = curvature_cmd;
    packet.steering_angle_deg = steering_angle_deg;
    packet.steering_pwm_us = steering_pwm_us;
    packet.steering_clamped = steering_clamped;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#endif
}

/**
 * @brief Pack a smart_car_motion_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param speed_mps [m/s] Estimated vehicle speed.
 * @param target_speed_mps [m/s] Current target speed.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param yaw_deg [deg] Integrated debug yaw.
 * @param curvature_meas [1/m] Measured curvature.
 * @param curvature_cmd [1/m] Commanded curvature.
 * @param steering_angle_deg [deg] Steering servo angle.
 * @param steering_pwm_us [us] Steering PWM pulse width.
 * @param steering_clamped  Steering clamp flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,float speed_mps,float target_speed_mps,float yaw_rate_dps,float yaw_deg,float curvature_meas,float curvature_cmd,float steering_angle_deg,uint16_t steering_pwm_us,uint8_t steering_clamped)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, speed_mps);
    _mav_put_float(buf, 8, target_speed_mps);
    _mav_put_float(buf, 12, yaw_rate_dps);
    _mav_put_float(buf, 16, yaw_deg);
    _mav_put_float(buf, 20, curvature_meas);
    _mav_put_float(buf, 24, curvature_cmd);
    _mav_put_float(buf, 28, steering_angle_deg);
    _mav_put_uint16_t(buf, 32, steering_pwm_us);
    _mav_put_uint8_t(buf, 34, steering_clamped);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#else
    mavlink_smart_car_motion_state_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.speed_mps = speed_mps;
    packet.target_speed_mps = target_speed_mps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.yaw_deg = yaw_deg;
    packet.curvature_meas = curvature_meas;
    packet.curvature_cmd = curvature_cmd;
    packet.steering_angle_deg = steering_angle_deg;
    packet.steering_pwm_us = steering_pwm_us;
    packet.steering_clamped = steering_clamped;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
}

/**
 * @brief Encode a smart_car_motion_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motion_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_motion_state_t* smart_car_motion_state)
{
    return mavlink_msg_smart_car_motion_state_pack(system_id, component_id, msg, smart_car_motion_state->time_boot_ms, smart_car_motion_state->speed_mps, smart_car_motion_state->target_speed_mps, smart_car_motion_state->yaw_rate_dps, smart_car_motion_state->yaw_deg, smart_car_motion_state->curvature_meas, smart_car_motion_state->curvature_cmd, smart_car_motion_state->steering_angle_deg, smart_car_motion_state->steering_pwm_us, smart_car_motion_state->steering_clamped);
}

/**
 * @brief Encode a smart_car_motion_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motion_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_motion_state_t* smart_car_motion_state)
{
    return mavlink_msg_smart_car_motion_state_pack_chan(system_id, component_id, chan, msg, smart_car_motion_state->time_boot_ms, smart_car_motion_state->speed_mps, smart_car_motion_state->target_speed_mps, smart_car_motion_state->yaw_rate_dps, smart_car_motion_state->yaw_deg, smart_car_motion_state->curvature_meas, smart_car_motion_state->curvature_cmd, smart_car_motion_state->steering_angle_deg, smart_car_motion_state->steering_pwm_us, smart_car_motion_state->steering_clamped);
}

/**
 * @brief Encode a smart_car_motion_state struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motion_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_motion_state_t* smart_car_motion_state)
{
    return mavlink_msg_smart_car_motion_state_pack_status(system_id, component_id, _status, msg,  smart_car_motion_state->time_boot_ms, smart_car_motion_state->speed_mps, smart_car_motion_state->target_speed_mps, smart_car_motion_state->yaw_rate_dps, smart_car_motion_state->yaw_deg, smart_car_motion_state->curvature_meas, smart_car_motion_state->curvature_cmd, smart_car_motion_state->steering_angle_deg, smart_car_motion_state->steering_pwm_us, smart_car_motion_state->steering_clamped);
}

/**
 * @brief Send a smart_car_motion_state message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param speed_mps [m/s] Estimated vehicle speed.
 * @param target_speed_mps [m/s] Current target speed.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param yaw_deg [deg] Integrated debug yaw.
 * @param curvature_meas [1/m] Measured curvature.
 * @param curvature_cmd [1/m] Commanded curvature.
 * @param steering_angle_deg [deg] Steering servo angle.
 * @param steering_pwm_us [us] Steering PWM pulse width.
 * @param steering_clamped  Steering clamp flag.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_motion_state_send(mavlink_channel_t chan, uint32_t time_boot_ms, float speed_mps, float target_speed_mps, float yaw_rate_dps, float yaw_deg, float curvature_meas, float curvature_cmd, float steering_angle_deg, uint16_t steering_pwm_us, uint8_t steering_clamped)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, speed_mps);
    _mav_put_float(buf, 8, target_speed_mps);
    _mav_put_float(buf, 12, yaw_rate_dps);
    _mav_put_float(buf, 16, yaw_deg);
    _mav_put_float(buf, 20, curvature_meas);
    _mav_put_float(buf, 24, curvature_cmd);
    _mav_put_float(buf, 28, steering_angle_deg);
    _mav_put_uint16_t(buf, 32, steering_pwm_us);
    _mav_put_uint8_t(buf, 34, steering_clamped);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE, buf, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#else
    mavlink_smart_car_motion_state_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.speed_mps = speed_mps;
    packet.target_speed_mps = target_speed_mps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.yaw_deg = yaw_deg;
    packet.curvature_meas = curvature_meas;
    packet.curvature_cmd = curvature_cmd;
    packet.steering_angle_deg = steering_angle_deg;
    packet.steering_pwm_us = steering_pwm_us;
    packet.steering_clamped = steering_clamped;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#endif
}

/**
 * @brief Send a smart_car_motion_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_motion_state_send_struct(mavlink_channel_t chan, const mavlink_smart_car_motion_state_t* smart_car_motion_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_motion_state_send(chan, smart_car_motion_state->time_boot_ms, smart_car_motion_state->speed_mps, smart_car_motion_state->target_speed_mps, smart_car_motion_state->yaw_rate_dps, smart_car_motion_state->yaw_deg, smart_car_motion_state->curvature_meas, smart_car_motion_state->curvature_cmd, smart_car_motion_state->steering_angle_deg, smart_car_motion_state->steering_pwm_us, smart_car_motion_state->steering_clamped);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE, (const char *)smart_car_motion_state, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_motion_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, float speed_mps, float target_speed_mps, float yaw_rate_dps, float yaw_deg, float curvature_meas, float curvature_cmd, float steering_angle_deg, uint16_t steering_pwm_us, uint8_t steering_clamped)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, speed_mps);
    _mav_put_float(buf, 8, target_speed_mps);
    _mav_put_float(buf, 12, yaw_rate_dps);
    _mav_put_float(buf, 16, yaw_deg);
    _mav_put_float(buf, 20, curvature_meas);
    _mav_put_float(buf, 24, curvature_cmd);
    _mav_put_float(buf, 28, steering_angle_deg);
    _mav_put_uint16_t(buf, 32, steering_pwm_us);
    _mav_put_uint8_t(buf, 34, steering_clamped);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE, buf, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#else
    mavlink_smart_car_motion_state_t *packet = (mavlink_smart_car_motion_state_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->speed_mps = speed_mps;
    packet->target_speed_mps = target_speed_mps;
    packet->yaw_rate_dps = yaw_rate_dps;
    packet->yaw_deg = yaw_deg;
    packet->curvature_meas = curvature_meas;
    packet->curvature_cmd = curvature_cmd;
    packet->steering_angle_deg = steering_angle_deg;
    packet->steering_pwm_us = steering_pwm_us;
    packet->steering_clamped = steering_clamped;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_MOTION_STATE UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_motion_state message
 *
 * @return [ms] Timestamp from the vehicle.
 */
static inline uint32_t mavlink_msg_smart_car_motion_state_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field speed_mps from smart_car_motion_state message
 *
 * @return [m/s] Estimated vehicle speed.
 */
static inline float mavlink_msg_smart_car_motion_state_get_speed_mps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field target_speed_mps from smart_car_motion_state message
 *
 * @return [m/s] Current target speed.
 */
static inline float mavlink_msg_smart_car_motion_state_get_target_speed_mps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field yaw_rate_dps from smart_car_motion_state message
 *
 * @return [deg/s] Filtered yaw rate.
 */
static inline float mavlink_msg_smart_car_motion_state_get_yaw_rate_dps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field yaw_deg from smart_car_motion_state message
 *
 * @return [deg] Integrated debug yaw.
 */
static inline float mavlink_msg_smart_car_motion_state_get_yaw_deg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field curvature_meas from smart_car_motion_state message
 *
 * @return [1/m] Measured curvature.
 */
static inline float mavlink_msg_smart_car_motion_state_get_curvature_meas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field curvature_cmd from smart_car_motion_state message
 *
 * @return [1/m] Commanded curvature.
 */
static inline float mavlink_msg_smart_car_motion_state_get_curvature_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field steering_angle_deg from smart_car_motion_state message
 *
 * @return [deg] Steering servo angle.
 */
static inline float mavlink_msg_smart_car_motion_state_get_steering_angle_deg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field steering_pwm_us from smart_car_motion_state message
 *
 * @return [us] Steering PWM pulse width.
 */
static inline uint16_t mavlink_msg_smart_car_motion_state_get_steering_pwm_us(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  32);
}

/**
 * @brief Get field steering_clamped from smart_car_motion_state message
 *
 * @return  Steering clamp flag.
 */
static inline uint8_t mavlink_msg_smart_car_motion_state_get_steering_clamped(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  34);
}

/**
 * @brief Decode a smart_car_motion_state message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_motion_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_motion_state_decode(const mavlink_message_t* msg, mavlink_smart_car_motion_state_t* smart_car_motion_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_motion_state->time_boot_ms = mavlink_msg_smart_car_motion_state_get_time_boot_ms(msg);
    smart_car_motion_state->speed_mps = mavlink_msg_smart_car_motion_state_get_speed_mps(msg);
    smart_car_motion_state->target_speed_mps = mavlink_msg_smart_car_motion_state_get_target_speed_mps(msg);
    smart_car_motion_state->yaw_rate_dps = mavlink_msg_smart_car_motion_state_get_yaw_rate_dps(msg);
    smart_car_motion_state->yaw_deg = mavlink_msg_smart_car_motion_state_get_yaw_deg(msg);
    smart_car_motion_state->curvature_meas = mavlink_msg_smart_car_motion_state_get_curvature_meas(msg);
    smart_car_motion_state->curvature_cmd = mavlink_msg_smart_car_motion_state_get_curvature_cmd(msg);
    smart_car_motion_state->steering_angle_deg = mavlink_msg_smart_car_motion_state_get_steering_angle_deg(msg);
    smart_car_motion_state->steering_pwm_us = mavlink_msg_smart_car_motion_state_get_steering_pwm_us(msg);
    smart_car_motion_state->steering_clamped = mavlink_msg_smart_car_motion_state_get_steering_clamped(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN;
        memset(smart_car_motion_state, 0, MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_LEN);
    memcpy(smart_car_motion_state, _MAV_PAYLOAD(msg), len);
#endif
}
