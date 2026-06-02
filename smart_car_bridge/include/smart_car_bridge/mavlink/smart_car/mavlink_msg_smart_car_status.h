#pragma once
// MESSAGE SMART_CAR_STATUS PACKING

#define MAVLINK_MSG_ID_SMART_CAR_STATUS 42001


typedef struct __mavlink_smart_car_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the vehicle.*/
 uint32_t fault_flags; /*<  Fault flags.*/
 uint32_t warn_flags; /*<  Warning flags.*/
 uint16_t cmd_age_ms; /*< [ms] Age of the last accepted control command.*/
 uint16_t control_loop_hz; /*< [Hz] Estimated control loop frequency.*/
 uint8_t mode; /*<  Current mode.*/
 uint8_t state; /*<  Current state.*/
 uint8_t imu_online; /*<  IMU online flag.*/
 uint8_t can_online; /*<  CAN online flag.*/
 uint8_t host_online; /*<  Host online flag.*/
 uint8_t servo_online; /*<  Servo online flag.*/
 uint8_t motor_online_mask; /*<  Motor online bitmask.*/
} mavlink_smart_car_status_t;

#define MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN 23
#define MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN 23
#define MAVLINK_MSG_ID_42001_LEN 23
#define MAVLINK_MSG_ID_42001_MIN_LEN 23

#define MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC 12
#define MAVLINK_MSG_ID_42001_CRC 12



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_STATUS { \
    42001, \
    "SMART_CAR_STATUS", \
    12, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_status_t, time_boot_ms) }, \
         { "fault_flags", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_status_t, fault_flags) }, \
         { "warn_flags", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_status_t, warn_flags) }, \
         { "cmd_age_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_smart_car_status_t, cmd_age_ms) }, \
         { "control_loop_hz", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_smart_car_status_t, control_loop_hz) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_smart_car_status_t, mode) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_smart_car_status_t, state) }, \
         { "imu_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_smart_car_status_t, imu_online) }, \
         { "can_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_smart_car_status_t, can_online) }, \
         { "host_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_smart_car_status_t, host_online) }, \
         { "servo_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_smart_car_status_t, servo_online) }, \
         { "motor_online_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_smart_car_status_t, motor_online_mask) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_STATUS { \
    "SMART_CAR_STATUS", \
    12, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_status_t, time_boot_ms) }, \
         { "fault_flags", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_status_t, fault_flags) }, \
         { "warn_flags", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_status_t, warn_flags) }, \
         { "cmd_age_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_smart_car_status_t, cmd_age_ms) }, \
         { "control_loop_hz", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_smart_car_status_t, control_loop_hz) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_smart_car_status_t, mode) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_smart_car_status_t, state) }, \
         { "imu_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_smart_car_status_t, imu_online) }, \
         { "can_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_smart_car_status_t, can_online) }, \
         { "host_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_smart_car_status_t, host_online) }, \
         { "servo_online", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_smart_car_status_t, servo_online) }, \
         { "motor_online_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_smart_car_status_t, motor_online_mask) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param fault_flags  Fault flags.
 * @param warn_flags  Warning flags.
 * @param cmd_age_ms [ms] Age of the last accepted control command.
 * @param control_loop_hz [Hz] Estimated control loop frequency.
 * @param mode  Current mode.
 * @param state  Current state.
 * @param imu_online  IMU online flag.
 * @param can_online  CAN online flag.
 * @param host_online  Host online flag.
 * @param servo_online  Servo online flag.
 * @param motor_online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t fault_flags, uint32_t warn_flags, uint16_t cmd_age_ms, uint16_t control_loop_hz, uint8_t mode, uint8_t state, uint8_t imu_online, uint8_t can_online, uint8_t host_online, uint8_t servo_online, uint8_t motor_online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, fault_flags);
    _mav_put_uint32_t(buf, 8, warn_flags);
    _mav_put_uint16_t(buf, 12, cmd_age_ms);
    _mav_put_uint16_t(buf, 14, control_loop_hz);
    _mav_put_uint8_t(buf, 16, mode);
    _mav_put_uint8_t(buf, 17, state);
    _mav_put_uint8_t(buf, 18, imu_online);
    _mav_put_uint8_t(buf, 19, can_online);
    _mav_put_uint8_t(buf, 20, host_online);
    _mav_put_uint8_t(buf, 21, servo_online);
    _mav_put_uint8_t(buf, 22, motor_online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#else
    mavlink_smart_car_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.fault_flags = fault_flags;
    packet.warn_flags = warn_flags;
    packet.cmd_age_ms = cmd_age_ms;
    packet.control_loop_hz = control_loop_hz;
    packet.mode = mode;
    packet.state = state;
    packet.imu_online = imu_online;
    packet.can_online = can_online;
    packet.host_online = host_online;
    packet.servo_online = servo_online;
    packet.motor_online_mask = motor_online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
}

/**
 * @brief Pack a smart_car_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param fault_flags  Fault flags.
 * @param warn_flags  Warning flags.
 * @param cmd_age_ms [ms] Age of the last accepted control command.
 * @param control_loop_hz [Hz] Estimated control loop frequency.
 * @param mode  Current mode.
 * @param state  Current state.
 * @param imu_online  IMU online flag.
 * @param can_online  CAN online flag.
 * @param host_online  Host online flag.
 * @param servo_online  Servo online flag.
 * @param motor_online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t fault_flags, uint32_t warn_flags, uint16_t cmd_age_ms, uint16_t control_loop_hz, uint8_t mode, uint8_t state, uint8_t imu_online, uint8_t can_online, uint8_t host_online, uint8_t servo_online, uint8_t motor_online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, fault_flags);
    _mav_put_uint32_t(buf, 8, warn_flags);
    _mav_put_uint16_t(buf, 12, cmd_age_ms);
    _mav_put_uint16_t(buf, 14, control_loop_hz);
    _mav_put_uint8_t(buf, 16, mode);
    _mav_put_uint8_t(buf, 17, state);
    _mav_put_uint8_t(buf, 18, imu_online);
    _mav_put_uint8_t(buf, 19, can_online);
    _mav_put_uint8_t(buf, 20, host_online);
    _mav_put_uint8_t(buf, 21, servo_online);
    _mav_put_uint8_t(buf, 22, motor_online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#else
    mavlink_smart_car_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.fault_flags = fault_flags;
    packet.warn_flags = warn_flags;
    packet.cmd_age_ms = cmd_age_ms;
    packet.control_loop_hz = control_loop_hz;
    packet.mode = mode;
    packet.state = state;
    packet.imu_online = imu_online;
    packet.can_online = can_online;
    packet.host_online = host_online;
    packet.servo_online = servo_online;
    packet.motor_online_mask = motor_online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#endif
}

/**
 * @brief Pack a smart_car_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param fault_flags  Fault flags.
 * @param warn_flags  Warning flags.
 * @param cmd_age_ms [ms] Age of the last accepted control command.
 * @param control_loop_hz [Hz] Estimated control loop frequency.
 * @param mode  Current mode.
 * @param state  Current state.
 * @param imu_online  IMU online flag.
 * @param can_online  CAN online flag.
 * @param host_online  Host online flag.
 * @param servo_online  Servo online flag.
 * @param motor_online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint32_t fault_flags,uint32_t warn_flags,uint16_t cmd_age_ms,uint16_t control_loop_hz,uint8_t mode,uint8_t state,uint8_t imu_online,uint8_t can_online,uint8_t host_online,uint8_t servo_online,uint8_t motor_online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, fault_flags);
    _mav_put_uint32_t(buf, 8, warn_flags);
    _mav_put_uint16_t(buf, 12, cmd_age_ms);
    _mav_put_uint16_t(buf, 14, control_loop_hz);
    _mav_put_uint8_t(buf, 16, mode);
    _mav_put_uint8_t(buf, 17, state);
    _mav_put_uint8_t(buf, 18, imu_online);
    _mav_put_uint8_t(buf, 19, can_online);
    _mav_put_uint8_t(buf, 20, host_online);
    _mav_put_uint8_t(buf, 21, servo_online);
    _mav_put_uint8_t(buf, 22, motor_online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#else
    mavlink_smart_car_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.fault_flags = fault_flags;
    packet.warn_flags = warn_flags;
    packet.cmd_age_ms = cmd_age_ms;
    packet.control_loop_hz = control_loop_hz;
    packet.mode = mode;
    packet.state = state;
    packet.imu_online = imu_online;
    packet.can_online = can_online;
    packet.host_online = host_online;
    packet.servo_online = servo_online;
    packet.motor_online_mask = motor_online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
}

/**
 * @brief Encode a smart_car_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_status_t* smart_car_status)
{
    return mavlink_msg_smart_car_status_pack(system_id, component_id, msg, smart_car_status->time_boot_ms, smart_car_status->fault_flags, smart_car_status->warn_flags, smart_car_status->cmd_age_ms, smart_car_status->control_loop_hz, smart_car_status->mode, smart_car_status->state, smart_car_status->imu_online, smart_car_status->can_online, smart_car_status->host_online, smart_car_status->servo_online, smart_car_status->motor_online_mask);
}

/**
 * @brief Encode a smart_car_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_status_t* smart_car_status)
{
    return mavlink_msg_smart_car_status_pack_chan(system_id, component_id, chan, msg, smart_car_status->time_boot_ms, smart_car_status->fault_flags, smart_car_status->warn_flags, smart_car_status->cmd_age_ms, smart_car_status->control_loop_hz, smart_car_status->mode, smart_car_status->state, smart_car_status->imu_online, smart_car_status->can_online, smart_car_status->host_online, smart_car_status->servo_online, smart_car_status->motor_online_mask);
}

/**
 * @brief Encode a smart_car_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_status_t* smart_car_status)
{
    return mavlink_msg_smart_car_status_pack_status(system_id, component_id, _status, msg,  smart_car_status->time_boot_ms, smart_car_status->fault_flags, smart_car_status->warn_flags, smart_car_status->cmd_age_ms, smart_car_status->control_loop_hz, smart_car_status->mode, smart_car_status->state, smart_car_status->imu_online, smart_car_status->can_online, smart_car_status->host_online, smart_car_status->servo_online, smart_car_status->motor_online_mask);
}

/**
 * @brief Send a smart_car_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param fault_flags  Fault flags.
 * @param warn_flags  Warning flags.
 * @param cmd_age_ms [ms] Age of the last accepted control command.
 * @param control_loop_hz [Hz] Estimated control loop frequency.
 * @param mode  Current mode.
 * @param state  Current state.
 * @param imu_online  IMU online flag.
 * @param can_online  CAN online flag.
 * @param host_online  Host online flag.
 * @param servo_online  Servo online flag.
 * @param motor_online_mask  Motor online bitmask.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint32_t fault_flags, uint32_t warn_flags, uint16_t cmd_age_ms, uint16_t control_loop_hz, uint8_t mode, uint8_t state, uint8_t imu_online, uint8_t can_online, uint8_t host_online, uint8_t servo_online, uint8_t motor_online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, fault_flags);
    _mav_put_uint32_t(buf, 8, warn_flags);
    _mav_put_uint16_t(buf, 12, cmd_age_ms);
    _mav_put_uint16_t(buf, 14, control_loop_hz);
    _mav_put_uint8_t(buf, 16, mode);
    _mav_put_uint8_t(buf, 17, state);
    _mav_put_uint8_t(buf, 18, imu_online);
    _mav_put_uint8_t(buf, 19, can_online);
    _mav_put_uint8_t(buf, 20, host_online);
    _mav_put_uint8_t(buf, 21, servo_online);
    _mav_put_uint8_t(buf, 22, motor_online_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#else
    mavlink_smart_car_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.fault_flags = fault_flags;
    packet.warn_flags = warn_flags;
    packet.cmd_age_ms = cmd_age_ms;
    packet.control_loop_hz = control_loop_hz;
    packet.mode = mode;
    packet.state = state;
    packet.imu_online = imu_online;
    packet.can_online = can_online;
    packet.host_online = host_online;
    packet.servo_online = servo_online;
    packet.motor_online_mask = motor_online_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#endif
}

/**
 * @brief Send a smart_car_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_status_send_struct(mavlink_channel_t chan, const mavlink_smart_car_status_t* smart_car_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_status_send(chan, smart_car_status->time_boot_ms, smart_car_status->fault_flags, smart_car_status->warn_flags, smart_car_status->cmd_age_ms, smart_car_status->control_loop_hz, smart_car_status->mode, smart_car_status->state, smart_car_status->imu_online, smart_car_status->can_online, smart_car_status->host_online, smart_car_status->servo_online, smart_car_status->motor_online_mask);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_STATUS, (const char *)smart_car_status, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint32_t fault_flags, uint32_t warn_flags, uint16_t cmd_age_ms, uint16_t control_loop_hz, uint8_t mode, uint8_t state, uint8_t imu_online, uint8_t can_online, uint8_t host_online, uint8_t servo_online, uint8_t motor_online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, fault_flags);
    _mav_put_uint32_t(buf, 8, warn_flags);
    _mav_put_uint16_t(buf, 12, cmd_age_ms);
    _mav_put_uint16_t(buf, 14, control_loop_hz);
    _mav_put_uint8_t(buf, 16, mode);
    _mav_put_uint8_t(buf, 17, state);
    _mav_put_uint8_t(buf, 18, imu_online);
    _mav_put_uint8_t(buf, 19, can_online);
    _mav_put_uint8_t(buf, 20, host_online);
    _mav_put_uint8_t(buf, 21, servo_online);
    _mav_put_uint8_t(buf, 22, motor_online_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#else
    mavlink_smart_car_status_t *packet = (mavlink_smart_car_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->fault_flags = fault_flags;
    packet->warn_flags = warn_flags;
    packet->cmd_age_ms = cmd_age_ms;
    packet->control_loop_hz = control_loop_hz;
    packet->mode = mode;
    packet->state = state;
    packet->imu_online = imu_online;
    packet->can_online = can_online;
    packet->host_online = host_online;
    packet->servo_online = servo_online;
    packet->motor_online_mask = motor_online_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_STATUS, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_status message
 *
 * @return [ms] Timestamp from the vehicle.
 */
static inline uint32_t mavlink_msg_smart_car_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field fault_flags from smart_car_status message
 *
 * @return  Fault flags.
 */
static inline uint32_t mavlink_msg_smart_car_status_get_fault_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field warn_flags from smart_car_status message
 *
 * @return  Warning flags.
 */
static inline uint32_t mavlink_msg_smart_car_status_get_warn_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field cmd_age_ms from smart_car_status message
 *
 * @return [ms] Age of the last accepted control command.
 */
static inline uint16_t mavlink_msg_smart_car_status_get_cmd_age_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field control_loop_hz from smart_car_status message
 *
 * @return [Hz] Estimated control loop frequency.
 */
static inline uint16_t mavlink_msg_smart_car_status_get_control_loop_hz(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  14);
}

/**
 * @brief Get field mode from smart_car_status message
 *
 * @return  Current mode.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field state from smart_car_status message
 *
 * @return  Current state.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field imu_online from smart_car_status message
 *
 * @return  IMU online flag.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_imu_online(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field can_online from smart_car_status message
 *
 * @return  CAN online flag.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_can_online(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field host_online from smart_car_status message
 *
 * @return  Host online flag.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_host_online(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field servo_online from smart_car_status message
 *
 * @return  Servo online flag.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_servo_online(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Get field motor_online_mask from smart_car_status message
 *
 * @return  Motor online bitmask.
 */
static inline uint8_t mavlink_msg_smart_car_status_get_motor_online_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Decode a smart_car_status message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_status_decode(const mavlink_message_t* msg, mavlink_smart_car_status_t* smart_car_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_status->time_boot_ms = mavlink_msg_smart_car_status_get_time_boot_ms(msg);
    smart_car_status->fault_flags = mavlink_msg_smart_car_status_get_fault_flags(msg);
    smart_car_status->warn_flags = mavlink_msg_smart_car_status_get_warn_flags(msg);
    smart_car_status->cmd_age_ms = mavlink_msg_smart_car_status_get_cmd_age_ms(msg);
    smart_car_status->control_loop_hz = mavlink_msg_smart_car_status_get_control_loop_hz(msg);
    smart_car_status->mode = mavlink_msg_smart_car_status_get_mode(msg);
    smart_car_status->state = mavlink_msg_smart_car_status_get_state(msg);
    smart_car_status->imu_online = mavlink_msg_smart_car_status_get_imu_online(msg);
    smart_car_status->can_online = mavlink_msg_smart_car_status_get_can_online(msg);
    smart_car_status->host_online = mavlink_msg_smart_car_status_get_host_online(msg);
    smart_car_status->servo_online = mavlink_msg_smart_car_status_get_servo_online(msg);
    smart_car_status->motor_online_mask = mavlink_msg_smart_car_status_get_motor_online_mask(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN;
        memset(smart_car_status, 0, MAVLINK_MSG_ID_SMART_CAR_STATUS_LEN);
    memcpy(smart_car_status, _MAV_PAYLOAD(msg), len);
#endif
}
