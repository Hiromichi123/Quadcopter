#pragma once
// MESSAGE SMART_CAR_COMMAND PACKING

#define MAVLINK_MSG_ID_SMART_CAR_COMMAND 42007


typedef struct __mavlink_smart_car_command_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the host.*/
 float param1; /*<  Command parameter 1.*/
 float param2; /*<  Command parameter 2.*/
 float param3; /*<  Command parameter 3.*/
 float param4; /*<  Command parameter 4.*/
 uint16_t command; /*<  Command ID.*/
 uint8_t target_system; /*<  System ID.*/
 uint8_t target_component; /*<  Component ID.*/
} mavlink_smart_car_command_t;

#define MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN 24
#define MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN 24
#define MAVLINK_MSG_ID_42007_LEN 24
#define MAVLINK_MSG_ID_42007_MIN_LEN 24

#define MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC 145
#define MAVLINK_MSG_ID_42007_CRC 145



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_COMMAND { \
    42007, \
    "SMART_CAR_COMMAND", \
    8, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_command_t, time_boot_ms) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_command_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_command_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_command_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_command_t, param4) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_smart_car_command_t, command) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_smart_car_command_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_smart_car_command_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_COMMAND { \
    "SMART_CAR_COMMAND", \
    8, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_command_t, time_boot_ms) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_command_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_smart_car_command_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_smart_car_command_t, param3) }, \
         { "param4", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_smart_car_command_t, param4) }, \
         { "command", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_smart_car_command_t, command) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_smart_car_command_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_smart_car_command_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param param1  Command parameter 1.
 * @param param2  Command parameter 2.
 * @param param3  Command parameter 3.
 * @param param4  Command parameter 4.
 * @param command  Command ID.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_command_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float param1, float param2, float param3, float param4, uint16_t command, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, param1);
    _mav_put_float(buf, 8, param2);
    _mav_put_float(buf, 12, param3);
    _mav_put_float(buf, 16, param4);
    _mav_put_uint16_t(buf, 20, command);
    _mav_put_uint8_t(buf, 22, target_system);
    _mav_put_uint8_t(buf, 23, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#else
    mavlink_smart_car_command_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_COMMAND;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
}

/**
 * @brief Pack a smart_car_command message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param param1  Command parameter 1.
 * @param param2  Command parameter 2.
 * @param param3  Command parameter 3.
 * @param param4  Command parameter 4.
 * @param command  Command ID.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_command_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float param1, float param2, float param3, float param4, uint16_t command, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, param1);
    _mav_put_float(buf, 8, param2);
    _mav_put_float(buf, 12, param3);
    _mav_put_float(buf, 16, param4);
    _mav_put_uint16_t(buf, 20, command);
    _mav_put_uint8_t(buf, 22, target_system);
    _mav_put_uint8_t(buf, 23, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#else
    mavlink_smart_car_command_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_COMMAND;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#endif
}

/**
 * @brief Pack a smart_car_command message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param param1  Command parameter 1.
 * @param param2  Command parameter 2.
 * @param param3  Command parameter 3.
 * @param param4  Command parameter 4.
 * @param command  Command ID.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_command_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,float param1,float param2,float param3,float param4,uint16_t command,uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, param1);
    _mav_put_float(buf, 8, param2);
    _mav_put_float(buf, 12, param3);
    _mav_put_float(buf, 16, param4);
    _mav_put_uint16_t(buf, 20, command);
    _mav_put_uint8_t(buf, 22, target_system);
    _mav_put_uint8_t(buf, 23, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#else
    mavlink_smart_car_command_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_COMMAND;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
}

/**
 * @brief Encode a smart_car_command struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_command_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_command_t* smart_car_command)
{
    return mavlink_msg_smart_car_command_pack(system_id, component_id, msg, smart_car_command->time_boot_ms, smart_car_command->param1, smart_car_command->param2, smart_car_command->param3, smart_car_command->param4, smart_car_command->command, smart_car_command->target_system, smart_car_command->target_component);
}

/**
 * @brief Encode a smart_car_command struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_command_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_command_t* smart_car_command)
{
    return mavlink_msg_smart_car_command_pack_chan(system_id, component_id, chan, msg, smart_car_command->time_boot_ms, smart_car_command->param1, smart_car_command->param2, smart_car_command->param3, smart_car_command->param4, smart_car_command->command, smart_car_command->target_system, smart_car_command->target_component);
}

/**
 * @brief Encode a smart_car_command struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_command C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_command_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_command_t* smart_car_command)
{
    return mavlink_msg_smart_car_command_pack_status(system_id, component_id, _status, msg,  smart_car_command->time_boot_ms, smart_car_command->param1, smart_car_command->param2, smart_car_command->param3, smart_car_command->param4, smart_car_command->command, smart_car_command->target_system, smart_car_command->target_component);
}

/**
 * @brief Send a smart_car_command message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param param1  Command parameter 1.
 * @param param2  Command parameter 2.
 * @param param3  Command parameter 3.
 * @param param4  Command parameter 4.
 * @param command  Command ID.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_command_send(mavlink_channel_t chan, uint32_t time_boot_ms, float param1, float param2, float param3, float param4, uint16_t command, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, param1);
    _mav_put_float(buf, 8, param2);
    _mav_put_float(buf, 12, param3);
    _mav_put_float(buf, 16, param4);
    _mav_put_uint16_t(buf, 20, command);
    _mav_put_uint8_t(buf, 22, target_system);
    _mav_put_uint8_t(buf, 23, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND, buf, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#else
    mavlink_smart_car_command_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.param3 = param3;
    packet.param4 = param4;
    packet.command = command;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#endif
}

/**
 * @brief Send a smart_car_command message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_command_send_struct(mavlink_channel_t chan, const mavlink_smart_car_command_t* smart_car_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_command_send(chan, smart_car_command->time_boot_ms, smart_car_command->param1, smart_car_command->param2, smart_car_command->param3, smart_car_command->param4, smart_car_command->command, smart_car_command->target_system, smart_car_command->target_component);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND, (const char *)smart_car_command, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_command_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, float param1, float param2, float param3, float param4, uint16_t command, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, param1);
    _mav_put_float(buf, 8, param2);
    _mav_put_float(buf, 12, param3);
    _mav_put_float(buf, 16, param4);
    _mav_put_uint16_t(buf, 20, command);
    _mav_put_uint8_t(buf, 22, target_system);
    _mav_put_uint8_t(buf, 23, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND, buf, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#else
    mavlink_smart_car_command_t *packet = (mavlink_smart_car_command_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->param1 = param1;
    packet->param2 = param2;
    packet->param3 = param3;
    packet->param4 = param4;
    packet->command = command;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_COMMAND, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN, MAVLINK_MSG_ID_SMART_CAR_COMMAND_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_COMMAND UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_command message
 *
 * @return [ms] Timestamp from the host.
 */
static inline uint32_t mavlink_msg_smart_car_command_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field param1 from smart_car_command message
 *
 * @return  Command parameter 1.
 */
static inline float mavlink_msg_smart_car_command_get_param1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field param2 from smart_car_command message
 *
 * @return  Command parameter 2.
 */
static inline float mavlink_msg_smart_car_command_get_param2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field param3 from smart_car_command message
 *
 * @return  Command parameter 3.
 */
static inline float mavlink_msg_smart_car_command_get_param3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field param4 from smart_car_command message
 *
 * @return  Command parameter 4.
 */
static inline float mavlink_msg_smart_car_command_get_param4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field command from smart_car_command message
 *
 * @return  Command ID.
 */
static inline uint16_t mavlink_msg_smart_car_command_get_command(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  20);
}

/**
 * @brief Get field target_system from smart_car_command message
 *
 * @return  System ID.
 */
static inline uint8_t mavlink_msg_smart_car_command_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field target_component from smart_car_command message
 *
 * @return  Component ID.
 */
static inline uint8_t mavlink_msg_smart_car_command_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  23);
}

/**
 * @brief Decode a smart_car_command message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_command C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_command_decode(const mavlink_message_t* msg, mavlink_smart_car_command_t* smart_car_command)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_command->time_boot_ms = mavlink_msg_smart_car_command_get_time_boot_ms(msg);
    smart_car_command->param1 = mavlink_msg_smart_car_command_get_param1(msg);
    smart_car_command->param2 = mavlink_msg_smart_car_command_get_param2(msg);
    smart_car_command->param3 = mavlink_msg_smart_car_command_get_param3(msg);
    smart_car_command->param4 = mavlink_msg_smart_car_command_get_param4(msg);
    smart_car_command->command = mavlink_msg_smart_car_command_get_command(msg);
    smart_car_command->target_system = mavlink_msg_smart_car_command_get_target_system(msg);
    smart_car_command->target_component = mavlink_msg_smart_car_command_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN;
        memset(smart_car_command, 0, MAVLINK_MSG_ID_SMART_CAR_COMMAND_LEN);
    memcpy(smart_car_command, _MAV_PAYLOAD(msg), len);
#endif
}
