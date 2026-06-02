#pragma once
// MESSAGE SMART_CAR_CONTROL_SETPOINT PACKING

#define MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT 42000


typedef struct __mavlink_smart_car_control_setpoint_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the host.*/
 float target_speed_mps; /*< [m/s] Target vehicle speed. Forward is positive.*/
 float target_curvature; /*< [1/m] Target curvature. Left turn is positive.*/
 float target_yaw_rate_dps; /*< [deg/s] Optional target yaw rate.*/
 float target_accel_mps2; /*< [m/s/s] Optional acceleration limit.*/
 uint16_t seq; /*<  Host control sequence number.*/
 uint16_t flags; /*<  Control flags.*/
 uint8_t target_system; /*<  System ID.*/
 uint8_t target_component; /*<  Component ID.*/
 uint8_t mode; /*<  Requested control mode.*/
} mavlink_smart_car_control_setpoint_t;

#define MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN 27
#define MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN 27
#define MAVLINK_MSG_ID_42000_LEN 27
#define MAVLINK_MSG_ID_42000_MIN_LEN 27

#define MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC 134
#define MAVLINK_MSG_ID_42000_CRC 134



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_CONTROL_SETPOINT { \
    42000, \
    "SMART_CAR_CONTROL_SETPOINT", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_control_setpoint_t, time_boot_ms) }, \
         { "target_speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_control_setpoint_t, target_speed_mps) }, \
         { "target_curvature", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_control_setpoint_t, target_curvature) }, \
         { "target_yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_control_setpoint_t, target_yaw_rate_dps) }, \
         { "target_accel_mps2", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_control_setpoint_t, target_accel_mps2) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_smart_car_control_setpoint_t, seq) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT16_T, 0, 22, offsetof(mavlink_smart_car_control_setpoint_t, flags) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_smart_car_control_setpoint_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_smart_car_control_setpoint_t, target_component) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_smart_car_control_setpoint_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_CONTROL_SETPOINT { \
    "SMART_CAR_CONTROL_SETPOINT", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_control_setpoint_t, time_boot_ms) }, \
         { "target_speed_mps", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_control_setpoint_t, target_speed_mps) }, \
         { "target_curvature", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_control_setpoint_t, target_curvature) }, \
         { "target_yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_control_setpoint_t, target_yaw_rate_dps) }, \
         { "target_accel_mps2", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_control_setpoint_t, target_accel_mps2) }, \
         { "seq", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_smart_car_control_setpoint_t, seq) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT16_T, 0, 22, offsetof(mavlink_smart_car_control_setpoint_t, flags) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_smart_car_control_setpoint_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_smart_car_control_setpoint_t, target_component) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_smart_car_control_setpoint_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_control_setpoint message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param target_speed_mps [m/s] Target vehicle speed. Forward is positive.
 * @param target_curvature [1/m] Target curvature. Left turn is positive.
 * @param target_yaw_rate_dps [deg/s] Optional target yaw rate.
 * @param target_accel_mps2 [m/s/s] Optional acceleration limit.
 * @param seq  Host control sequence number.
 * @param flags  Control flags.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @param mode  Requested control mode.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float target_speed_mps, float target_curvature, float target_yaw_rate_dps, float target_accel_mps2, uint16_t seq, uint16_t flags, uint8_t target_system, uint8_t target_component, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, target_speed_mps);
    _mav_put_float(buf, 8, target_curvature);
    _mav_put_float(buf, 12, target_yaw_rate_dps);
    _mav_put_float(buf, 16, target_accel_mps2);
    _mav_put_uint16_t(buf, 20, seq);
    _mav_put_uint16_t(buf, 22, flags);
    _mav_put_uint8_t(buf, 24, target_system);
    _mav_put_uint8_t(buf, 25, target_component);
    _mav_put_uint8_t(buf, 26, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#else
    mavlink_smart_car_control_setpoint_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.target_speed_mps = target_speed_mps;
    packet.target_curvature = target_curvature;
    packet.target_yaw_rate_dps = target_yaw_rate_dps;
    packet.target_accel_mps2 = target_accel_mps2;
    packet.seq = seq;
    packet.flags = flags;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
}

/**
 * @brief Pack a smart_car_control_setpoint message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param target_speed_mps [m/s] Target vehicle speed. Forward is positive.
 * @param target_curvature [1/m] Target curvature. Left turn is positive.
 * @param target_yaw_rate_dps [deg/s] Optional target yaw rate.
 * @param target_accel_mps2 [m/s/s] Optional acceleration limit.
 * @param seq  Host control sequence number.
 * @param flags  Control flags.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @param mode  Requested control mode.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float target_speed_mps, float target_curvature, float target_yaw_rate_dps, float target_accel_mps2, uint16_t seq, uint16_t flags, uint8_t target_system, uint8_t target_component, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, target_speed_mps);
    _mav_put_float(buf, 8, target_curvature);
    _mav_put_float(buf, 12, target_yaw_rate_dps);
    _mav_put_float(buf, 16, target_accel_mps2);
    _mav_put_uint16_t(buf, 20, seq);
    _mav_put_uint16_t(buf, 22, flags);
    _mav_put_uint8_t(buf, 24, target_system);
    _mav_put_uint8_t(buf, 25, target_component);
    _mav_put_uint8_t(buf, 26, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#else
    mavlink_smart_car_control_setpoint_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.target_speed_mps = target_speed_mps;
    packet.target_curvature = target_curvature;
    packet.target_yaw_rate_dps = target_yaw_rate_dps;
    packet.target_accel_mps2 = target_accel_mps2;
    packet.seq = seq;
    packet.flags = flags;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#endif
}

/**
 * @brief Pack a smart_car_control_setpoint message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param target_speed_mps [m/s] Target vehicle speed. Forward is positive.
 * @param target_curvature [1/m] Target curvature. Left turn is positive.
 * @param target_yaw_rate_dps [deg/s] Optional target yaw rate.
 * @param target_accel_mps2 [m/s/s] Optional acceleration limit.
 * @param seq  Host control sequence number.
 * @param flags  Control flags.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @param mode  Requested control mode.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,float target_speed_mps,float target_curvature,float target_yaw_rate_dps,float target_accel_mps2,uint16_t seq,uint16_t flags,uint8_t target_system,uint8_t target_component,uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, target_speed_mps);
    _mav_put_float(buf, 8, target_curvature);
    _mav_put_float(buf, 12, target_yaw_rate_dps);
    _mav_put_float(buf, 16, target_accel_mps2);
    _mav_put_uint16_t(buf, 20, seq);
    _mav_put_uint16_t(buf, 22, flags);
    _mav_put_uint8_t(buf, 24, target_system);
    _mav_put_uint8_t(buf, 25, target_component);
    _mav_put_uint8_t(buf, 26, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#else
    mavlink_smart_car_control_setpoint_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.target_speed_mps = target_speed_mps;
    packet.target_curvature = target_curvature;
    packet.target_yaw_rate_dps = target_yaw_rate_dps;
    packet.target_accel_mps2 = target_accel_mps2;
    packet.seq = seq;
    packet.flags = flags;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
}

/**
 * @brief Encode a smart_car_control_setpoint struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_control_setpoint C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_control_setpoint_t* smart_car_control_setpoint)
{
    return mavlink_msg_smart_car_control_setpoint_pack(system_id, component_id, msg, smart_car_control_setpoint->time_boot_ms, smart_car_control_setpoint->target_speed_mps, smart_car_control_setpoint->target_curvature, smart_car_control_setpoint->target_yaw_rate_dps, smart_car_control_setpoint->target_accel_mps2, smart_car_control_setpoint->seq, smart_car_control_setpoint->flags, smart_car_control_setpoint->target_system, smart_car_control_setpoint->target_component, smart_car_control_setpoint->mode);
}

/**
 * @brief Encode a smart_car_control_setpoint struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_control_setpoint C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_control_setpoint_t* smart_car_control_setpoint)
{
    return mavlink_msg_smart_car_control_setpoint_pack_chan(system_id, component_id, chan, msg, smart_car_control_setpoint->time_boot_ms, smart_car_control_setpoint->target_speed_mps, smart_car_control_setpoint->target_curvature, smart_car_control_setpoint->target_yaw_rate_dps, smart_car_control_setpoint->target_accel_mps2, smart_car_control_setpoint->seq, smart_car_control_setpoint->flags, smart_car_control_setpoint->target_system, smart_car_control_setpoint->target_component, smart_car_control_setpoint->mode);
}

/**
 * @brief Encode a smart_car_control_setpoint struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_control_setpoint C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_control_setpoint_t* smart_car_control_setpoint)
{
    return mavlink_msg_smart_car_control_setpoint_pack_status(system_id, component_id, _status, msg,  smart_car_control_setpoint->time_boot_ms, smart_car_control_setpoint->target_speed_mps, smart_car_control_setpoint->target_curvature, smart_car_control_setpoint->target_yaw_rate_dps, smart_car_control_setpoint->target_accel_mps2, smart_car_control_setpoint->seq, smart_car_control_setpoint->flags, smart_car_control_setpoint->target_system, smart_car_control_setpoint->target_component, smart_car_control_setpoint->mode);
}

/**
 * @brief Send a smart_car_control_setpoint message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param target_speed_mps [m/s] Target vehicle speed. Forward is positive.
 * @param target_curvature [1/m] Target curvature. Left turn is positive.
 * @param target_yaw_rate_dps [deg/s] Optional target yaw rate.
 * @param target_accel_mps2 [m/s/s] Optional acceleration limit.
 * @param seq  Host control sequence number.
 * @param flags  Control flags.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @param mode  Requested control mode.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_control_setpoint_send(mavlink_channel_t chan, uint32_t time_boot_ms, float target_speed_mps, float target_curvature, float target_yaw_rate_dps, float target_accel_mps2, uint16_t seq, uint16_t flags, uint8_t target_system, uint8_t target_component, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, target_speed_mps);
    _mav_put_float(buf, 8, target_curvature);
    _mav_put_float(buf, 12, target_yaw_rate_dps);
    _mav_put_float(buf, 16, target_accel_mps2);
    _mav_put_uint16_t(buf, 20, seq);
    _mav_put_uint16_t(buf, 22, flags);
    _mav_put_uint8_t(buf, 24, target_system);
    _mav_put_uint8_t(buf, 25, target_component);
    _mav_put_uint8_t(buf, 26, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT, buf, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#else
    mavlink_smart_car_control_setpoint_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.target_speed_mps = target_speed_mps;
    packet.target_curvature = target_curvature;
    packet.target_yaw_rate_dps = target_yaw_rate_dps;
    packet.target_accel_mps2 = target_accel_mps2;
    packet.seq = seq;
    packet.flags = flags;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#endif
}

/**
 * @brief Send a smart_car_control_setpoint message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_control_setpoint_send_struct(mavlink_channel_t chan, const mavlink_smart_car_control_setpoint_t* smart_car_control_setpoint)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_control_setpoint_send(chan, smart_car_control_setpoint->time_boot_ms, smart_car_control_setpoint->target_speed_mps, smart_car_control_setpoint->target_curvature, smart_car_control_setpoint->target_yaw_rate_dps, smart_car_control_setpoint->target_accel_mps2, smart_car_control_setpoint->seq, smart_car_control_setpoint->flags, smart_car_control_setpoint->target_system, smart_car_control_setpoint->target_component, smart_car_control_setpoint->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT, (const char *)smart_car_control_setpoint, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_control_setpoint_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, float target_speed_mps, float target_curvature, float target_yaw_rate_dps, float target_accel_mps2, uint16_t seq, uint16_t flags, uint8_t target_system, uint8_t target_component, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, target_speed_mps);
    _mav_put_float(buf, 8, target_curvature);
    _mav_put_float(buf, 12, target_yaw_rate_dps);
    _mav_put_float(buf, 16, target_accel_mps2);
    _mav_put_uint16_t(buf, 20, seq);
    _mav_put_uint16_t(buf, 22, flags);
    _mav_put_uint8_t(buf, 24, target_system);
    _mav_put_uint8_t(buf, 25, target_component);
    _mav_put_uint8_t(buf, 26, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT, buf, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#else
    mavlink_smart_car_control_setpoint_t *packet = (mavlink_smart_car_control_setpoint_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->target_speed_mps = target_speed_mps;
    packet->target_curvature = target_curvature;
    packet->target_yaw_rate_dps = target_yaw_rate_dps;
    packet->target_accel_mps2 = target_accel_mps2;
    packet->seq = seq;
    packet->flags = flags;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_CONTROL_SETPOINT UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_control_setpoint message
 *
 * @return [ms] Timestamp from the host.
 */
static inline uint32_t mavlink_msg_smart_car_control_setpoint_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field target_speed_mps from smart_car_control_setpoint message
 *
 * @return [m/s] Target vehicle speed. Forward is positive.
 */
static inline float mavlink_msg_smart_car_control_setpoint_get_target_speed_mps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field target_curvature from smart_car_control_setpoint message
 *
 * @return [1/m] Target curvature. Left turn is positive.
 */
static inline float mavlink_msg_smart_car_control_setpoint_get_target_curvature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field target_yaw_rate_dps from smart_car_control_setpoint message
 *
 * @return [deg/s] Optional target yaw rate.
 */
static inline float mavlink_msg_smart_car_control_setpoint_get_target_yaw_rate_dps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field target_accel_mps2 from smart_car_control_setpoint message
 *
 * @return [m/s/s] Optional acceleration limit.
 */
static inline float mavlink_msg_smart_car_control_setpoint_get_target_accel_mps2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field seq from smart_car_control_setpoint message
 *
 * @return  Host control sequence number.
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_get_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  20);
}

/**
 * @brief Get field flags from smart_car_control_setpoint message
 *
 * @return  Control flags.
 */
static inline uint16_t mavlink_msg_smart_car_control_setpoint_get_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  22);
}

/**
 * @brief Get field target_system from smart_car_control_setpoint message
 *
 * @return  System ID.
 */
static inline uint8_t mavlink_msg_smart_car_control_setpoint_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field target_component from smart_car_control_setpoint message
 *
 * @return  Component ID.
 */
static inline uint8_t mavlink_msg_smart_car_control_setpoint_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  25);
}

/**
 * @brief Get field mode from smart_car_control_setpoint message
 *
 * @return  Requested control mode.
 */
static inline uint8_t mavlink_msg_smart_car_control_setpoint_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Decode a smart_car_control_setpoint message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_control_setpoint C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_control_setpoint_decode(const mavlink_message_t* msg, mavlink_smart_car_control_setpoint_t* smart_car_control_setpoint)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_control_setpoint->time_boot_ms = mavlink_msg_smart_car_control_setpoint_get_time_boot_ms(msg);
    smart_car_control_setpoint->target_speed_mps = mavlink_msg_smart_car_control_setpoint_get_target_speed_mps(msg);
    smart_car_control_setpoint->target_curvature = mavlink_msg_smart_car_control_setpoint_get_target_curvature(msg);
    smart_car_control_setpoint->target_yaw_rate_dps = mavlink_msg_smart_car_control_setpoint_get_target_yaw_rate_dps(msg);
    smart_car_control_setpoint->target_accel_mps2 = mavlink_msg_smart_car_control_setpoint_get_target_accel_mps2(msg);
    smart_car_control_setpoint->seq = mavlink_msg_smart_car_control_setpoint_get_seq(msg);
    smart_car_control_setpoint->flags = mavlink_msg_smart_car_control_setpoint_get_flags(msg);
    smart_car_control_setpoint->target_system = mavlink_msg_smart_car_control_setpoint_get_target_system(msg);
    smart_car_control_setpoint->target_component = mavlink_msg_smart_car_control_setpoint_get_target_component(msg);
    smart_car_control_setpoint->mode = mavlink_msg_smart_car_control_setpoint_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN;
        memset(smart_car_control_setpoint, 0, MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_LEN);
    memcpy(smart_car_control_setpoint, _MAV_PAYLOAD(msg), len);
#endif
}
