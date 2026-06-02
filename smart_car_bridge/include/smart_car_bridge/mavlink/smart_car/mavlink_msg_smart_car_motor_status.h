#pragma once
// MESSAGE SMART_CAR_MOTOR_STATUS PACKING

#define MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS 42003


typedef struct __mavlink_smart_car_motor_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the vehicle.*/
 uint32_t can_tx_busy_count; /*<  CAN TX busy count.*/
 uint32_t can_error_count; /*<  CAN error count.*/
 int16_t target_rpm_1; /*< [rpm] Motor 1 target RPM.*/
 int16_t target_rpm_2; /*< [rpm] Motor 2 target RPM.*/
 int16_t actual_rpm_1; /*< [rpm] Motor 1 actual RPM.*/
 int16_t actual_rpm_2; /*< [rpm] Motor 2 actual RPM.*/
 int16_t current_cmd_1; /*<  Motor 1 command current.*/
 int16_t current_cmd_2; /*<  Motor 2 command current.*/
 int16_t feedback_current_1; /*<  Motor 1 feedback current.*/
 int16_t feedback_current_2; /*<  Motor 2 feedback current.*/
 uint16_t angle_1; /*<  Motor 1 encoder angle.*/
 uint16_t angle_2; /*<  Motor 2 encoder angle.*/
 uint8_t online_mask; /*<  Motor online bitmask.*/
} mavlink_smart_car_motor_status_t;

#define MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN 33
#define MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN 33
#define MAVLINK_MSG_ID_42003_LEN 33
#define MAVLINK_MSG_ID_42003_MIN_LEN 33

#define MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC 37
#define MAVLINK_MSG_ID_42003_CRC 37



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_MOTOR_STATUS { \
    42003, \
    "SMART_CAR_MOTOR_STATUS", \
    14, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_motor_status_t, time_boot_ms) }, \
         { "can_tx_busy_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_motor_status_t, can_tx_busy_count) }, \
         { "can_error_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_motor_status_t, can_error_count) }, \
         { "target_rpm_1", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_smart_car_motor_status_t, target_rpm_1) }, \
         { "target_rpm_2", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_smart_car_motor_status_t, target_rpm_2) }, \
         { "actual_rpm_1", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_smart_car_motor_status_t, actual_rpm_1) }, \
         { "actual_rpm_2", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_smart_car_motor_status_t, actual_rpm_2) }, \
         { "current_cmd_1", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_smart_car_motor_status_t, current_cmd_1) }, \
         { "current_cmd_2", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_smart_car_motor_status_t, current_cmd_2) }, \
         { "feedback_current_1", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_smart_car_motor_status_t, feedback_current_1) }, \
         { "feedback_current_2", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_smart_car_motor_status_t, feedback_current_2) }, \
         { "angle_1", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_smart_car_motor_status_t, angle_1) }, \
         { "angle_2", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_smart_car_motor_status_t, angle_2) }, \
         { "online_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_smart_car_motor_status_t, online_mask) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_MOTOR_STATUS { \
    "SMART_CAR_MOTOR_STATUS", \
    14, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_motor_status_t, time_boot_ms) }, \
         { "can_tx_busy_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_motor_status_t, can_tx_busy_count) }, \
         { "can_error_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_motor_status_t, can_error_count) }, \
         { "target_rpm_1", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_smart_car_motor_status_t, target_rpm_1) }, \
         { "target_rpm_2", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_smart_car_motor_status_t, target_rpm_2) }, \
         { "actual_rpm_1", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_smart_car_motor_status_t, actual_rpm_1) }, \
         { "actual_rpm_2", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_smart_car_motor_status_t, actual_rpm_2) }, \
         { "current_cmd_1", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_smart_car_motor_status_t, current_cmd_1) }, \
         { "current_cmd_2", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_smart_car_motor_status_t, current_cmd_2) }, \
         { "feedback_current_1", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_smart_car_motor_status_t, feedback_current_1) }, \
         { "feedback_current_2", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_smart_car_motor_status_t, feedback_current_2) }, \
         { "angle_1", NULL, MAVLINK_TYPE_UINT16_T, 0, 28, offsetof(mavlink_smart_car_motor_status_t, angle_1) }, \
         { "angle_2", NULL, MAVLINK_TYPE_UINT16_T, 0, 30, offsetof(mavlink_smart_car_motor_status_t, angle_2) }, \
         { "online_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_smart_car_motor_status_t, online_mask) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_motor_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param can_tx_busy_count  CAN TX busy count.
 * @param can_error_count  CAN error count.
 * @param target_rpm_1 [rpm] Motor 1 target RPM.
 * @param target_rpm_2 [rpm] Motor 2 target RPM.
 * @param actual_rpm_1 [rpm] Motor 1 actual RPM.
 * @param actual_rpm_2 [rpm] Motor 2 actual RPM.
 * @param current_cmd_1  Motor 1 command current.
 * @param current_cmd_2  Motor 2 command current.
 * @param feedback_current_1  Motor 1 feedback current.
 * @param feedback_current_2  Motor 2 feedback current.
 * @param angle_1  Motor 1 encoder angle.
 * @param angle_2  Motor 2 encoder angle.
 * @param online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t can_tx_busy_count, uint32_t can_error_count, int16_t target_rpm_1, int16_t target_rpm_2, int16_t actual_rpm_1, int16_t actual_rpm_2, int16_t current_cmd_1, int16_t current_cmd_2, int16_t feedback_current_1, int16_t feedback_current_2, uint16_t angle_1, uint16_t angle_2, uint8_t online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, can_tx_busy_count);
    _mav_put_uint32_t(buf, 8, can_error_count);
    _mav_put_int16_t(buf, 12, target_rpm_1);
    _mav_put_int16_t(buf, 14, target_rpm_2);
    _mav_put_int16_t(buf, 16, actual_rpm_1);
    _mav_put_int16_t(buf, 18, actual_rpm_2);
    _mav_put_int16_t(buf, 20, current_cmd_1);
    _mav_put_int16_t(buf, 22, current_cmd_2);
    _mav_put_int16_t(buf, 24, feedback_current_1);
    _mav_put_int16_t(buf, 26, feedback_current_2);
    _mav_put_uint16_t(buf, 28, angle_1);
    _mav_put_uint16_t(buf, 30, angle_2);
    _mav_put_uint8_t(buf, 32, online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#else
    mavlink_smart_car_motor_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.can_tx_busy_count = can_tx_busy_count;
    packet.can_error_count = can_error_count;
    packet.target_rpm_1 = target_rpm_1;
    packet.target_rpm_2 = target_rpm_2;
    packet.actual_rpm_1 = actual_rpm_1;
    packet.actual_rpm_2 = actual_rpm_2;
    packet.current_cmd_1 = current_cmd_1;
    packet.current_cmd_2 = current_cmd_2;
    packet.feedback_current_1 = feedback_current_1;
    packet.feedback_current_2 = feedback_current_2;
    packet.angle_1 = angle_1;
    packet.angle_2 = angle_2;
    packet.online_mask = online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
}

/**
 * @brief Pack a smart_car_motor_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param can_tx_busy_count  CAN TX busy count.
 * @param can_error_count  CAN error count.
 * @param target_rpm_1 [rpm] Motor 1 target RPM.
 * @param target_rpm_2 [rpm] Motor 2 target RPM.
 * @param actual_rpm_1 [rpm] Motor 1 actual RPM.
 * @param actual_rpm_2 [rpm] Motor 2 actual RPM.
 * @param current_cmd_1  Motor 1 command current.
 * @param current_cmd_2  Motor 2 command current.
 * @param feedback_current_1  Motor 1 feedback current.
 * @param feedback_current_2  Motor 2 feedback current.
 * @param angle_1  Motor 1 encoder angle.
 * @param angle_2  Motor 2 encoder angle.
 * @param online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t can_tx_busy_count, uint32_t can_error_count, int16_t target_rpm_1, int16_t target_rpm_2, int16_t actual_rpm_1, int16_t actual_rpm_2, int16_t current_cmd_1, int16_t current_cmd_2, int16_t feedback_current_1, int16_t feedback_current_2, uint16_t angle_1, uint16_t angle_2, uint8_t online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, can_tx_busy_count);
    _mav_put_uint32_t(buf, 8, can_error_count);
    _mav_put_int16_t(buf, 12, target_rpm_1);
    _mav_put_int16_t(buf, 14, target_rpm_2);
    _mav_put_int16_t(buf, 16, actual_rpm_1);
    _mav_put_int16_t(buf, 18, actual_rpm_2);
    _mav_put_int16_t(buf, 20, current_cmd_1);
    _mav_put_int16_t(buf, 22, current_cmd_2);
    _mav_put_int16_t(buf, 24, feedback_current_1);
    _mav_put_int16_t(buf, 26, feedback_current_2);
    _mav_put_uint16_t(buf, 28, angle_1);
    _mav_put_uint16_t(buf, 30, angle_2);
    _mav_put_uint8_t(buf, 32, online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#else
    mavlink_smart_car_motor_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.can_tx_busy_count = can_tx_busy_count;
    packet.can_error_count = can_error_count;
    packet.target_rpm_1 = target_rpm_1;
    packet.target_rpm_2 = target_rpm_2;
    packet.actual_rpm_1 = actual_rpm_1;
    packet.actual_rpm_2 = actual_rpm_2;
    packet.current_cmd_1 = current_cmd_1;
    packet.current_cmd_2 = current_cmd_2;
    packet.feedback_current_1 = feedback_current_1;
    packet.feedback_current_2 = feedback_current_2;
    packet.angle_1 = angle_1;
    packet.angle_2 = angle_2;
    packet.online_mask = online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#endif
}

/**
 * @brief Pack a smart_car_motor_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param can_tx_busy_count  CAN TX busy count.
 * @param can_error_count  CAN error count.
 * @param target_rpm_1 [rpm] Motor 1 target RPM.
 * @param target_rpm_2 [rpm] Motor 2 target RPM.
 * @param actual_rpm_1 [rpm] Motor 1 actual RPM.
 * @param actual_rpm_2 [rpm] Motor 2 actual RPM.
 * @param current_cmd_1  Motor 1 command current.
 * @param current_cmd_2  Motor 2 command current.
 * @param feedback_current_1  Motor 1 feedback current.
 * @param feedback_current_2  Motor 2 feedback current.
 * @param angle_1  Motor 1 encoder angle.
 * @param angle_2  Motor 2 encoder angle.
 * @param online_mask  Motor online bitmask.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint32_t can_tx_busy_count,uint32_t can_error_count,int16_t target_rpm_1,int16_t target_rpm_2,int16_t actual_rpm_1,int16_t actual_rpm_2,int16_t current_cmd_1,int16_t current_cmd_2,int16_t feedback_current_1,int16_t feedback_current_2,uint16_t angle_1,uint16_t angle_2,uint8_t online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, can_tx_busy_count);
    _mav_put_uint32_t(buf, 8, can_error_count);
    _mav_put_int16_t(buf, 12, target_rpm_1);
    _mav_put_int16_t(buf, 14, target_rpm_2);
    _mav_put_int16_t(buf, 16, actual_rpm_1);
    _mav_put_int16_t(buf, 18, actual_rpm_2);
    _mav_put_int16_t(buf, 20, current_cmd_1);
    _mav_put_int16_t(buf, 22, current_cmd_2);
    _mav_put_int16_t(buf, 24, feedback_current_1);
    _mav_put_int16_t(buf, 26, feedback_current_2);
    _mav_put_uint16_t(buf, 28, angle_1);
    _mav_put_uint16_t(buf, 30, angle_2);
    _mav_put_uint8_t(buf, 32, online_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#else
    mavlink_smart_car_motor_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.can_tx_busy_count = can_tx_busy_count;
    packet.can_error_count = can_error_count;
    packet.target_rpm_1 = target_rpm_1;
    packet.target_rpm_2 = target_rpm_2;
    packet.actual_rpm_1 = actual_rpm_1;
    packet.actual_rpm_2 = actual_rpm_2;
    packet.current_cmd_1 = current_cmd_1;
    packet.current_cmd_2 = current_cmd_2;
    packet.feedback_current_1 = feedback_current_1;
    packet.feedback_current_2 = feedback_current_2;
    packet.angle_1 = angle_1;
    packet.angle_2 = angle_2;
    packet.online_mask = online_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
}

/**
 * @brief Encode a smart_car_motor_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motor_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_motor_status_t* smart_car_motor_status)
{
    return mavlink_msg_smart_car_motor_status_pack(system_id, component_id, msg, smart_car_motor_status->time_boot_ms, smart_car_motor_status->can_tx_busy_count, smart_car_motor_status->can_error_count, smart_car_motor_status->target_rpm_1, smart_car_motor_status->target_rpm_2, smart_car_motor_status->actual_rpm_1, smart_car_motor_status->actual_rpm_2, smart_car_motor_status->current_cmd_1, smart_car_motor_status->current_cmd_2, smart_car_motor_status->feedback_current_1, smart_car_motor_status->feedback_current_2, smart_car_motor_status->angle_1, smart_car_motor_status->angle_2, smart_car_motor_status->online_mask);
}

/**
 * @brief Encode a smart_car_motor_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motor_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_motor_status_t* smart_car_motor_status)
{
    return mavlink_msg_smart_car_motor_status_pack_chan(system_id, component_id, chan, msg, smart_car_motor_status->time_boot_ms, smart_car_motor_status->can_tx_busy_count, smart_car_motor_status->can_error_count, smart_car_motor_status->target_rpm_1, smart_car_motor_status->target_rpm_2, smart_car_motor_status->actual_rpm_1, smart_car_motor_status->actual_rpm_2, smart_car_motor_status->current_cmd_1, smart_car_motor_status->current_cmd_2, smart_car_motor_status->feedback_current_1, smart_car_motor_status->feedback_current_2, smart_car_motor_status->angle_1, smart_car_motor_status->angle_2, smart_car_motor_status->online_mask);
}

/**
 * @brief Encode a smart_car_motor_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_motor_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_motor_status_t* smart_car_motor_status)
{
    return mavlink_msg_smart_car_motor_status_pack_status(system_id, component_id, _status, msg,  smart_car_motor_status->time_boot_ms, smart_car_motor_status->can_tx_busy_count, smart_car_motor_status->can_error_count, smart_car_motor_status->target_rpm_1, smart_car_motor_status->target_rpm_2, smart_car_motor_status->actual_rpm_1, smart_car_motor_status->actual_rpm_2, smart_car_motor_status->current_cmd_1, smart_car_motor_status->current_cmd_2, smart_car_motor_status->feedback_current_1, smart_car_motor_status->feedback_current_2, smart_car_motor_status->angle_1, smart_car_motor_status->angle_2, smart_car_motor_status->online_mask);
}

/**
 * @brief Send a smart_car_motor_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param can_tx_busy_count  CAN TX busy count.
 * @param can_error_count  CAN error count.
 * @param target_rpm_1 [rpm] Motor 1 target RPM.
 * @param target_rpm_2 [rpm] Motor 2 target RPM.
 * @param actual_rpm_1 [rpm] Motor 1 actual RPM.
 * @param actual_rpm_2 [rpm] Motor 2 actual RPM.
 * @param current_cmd_1  Motor 1 command current.
 * @param current_cmd_2  Motor 2 command current.
 * @param feedback_current_1  Motor 1 feedback current.
 * @param feedback_current_2  Motor 2 feedback current.
 * @param angle_1  Motor 1 encoder angle.
 * @param angle_2  Motor 2 encoder angle.
 * @param online_mask  Motor online bitmask.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_motor_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint32_t can_tx_busy_count, uint32_t can_error_count, int16_t target_rpm_1, int16_t target_rpm_2, int16_t actual_rpm_1, int16_t actual_rpm_2, int16_t current_cmd_1, int16_t current_cmd_2, int16_t feedback_current_1, int16_t feedback_current_2, uint16_t angle_1, uint16_t angle_2, uint8_t online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, can_tx_busy_count);
    _mav_put_uint32_t(buf, 8, can_error_count);
    _mav_put_int16_t(buf, 12, target_rpm_1);
    _mav_put_int16_t(buf, 14, target_rpm_2);
    _mav_put_int16_t(buf, 16, actual_rpm_1);
    _mav_put_int16_t(buf, 18, actual_rpm_2);
    _mav_put_int16_t(buf, 20, current_cmd_1);
    _mav_put_int16_t(buf, 22, current_cmd_2);
    _mav_put_int16_t(buf, 24, feedback_current_1);
    _mav_put_int16_t(buf, 26, feedback_current_2);
    _mav_put_uint16_t(buf, 28, angle_1);
    _mav_put_uint16_t(buf, 30, angle_2);
    _mav_put_uint8_t(buf, 32, online_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#else
    mavlink_smart_car_motor_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.can_tx_busy_count = can_tx_busy_count;
    packet.can_error_count = can_error_count;
    packet.target_rpm_1 = target_rpm_1;
    packet.target_rpm_2 = target_rpm_2;
    packet.actual_rpm_1 = actual_rpm_1;
    packet.actual_rpm_2 = actual_rpm_2;
    packet.current_cmd_1 = current_cmd_1;
    packet.current_cmd_2 = current_cmd_2;
    packet.feedback_current_1 = feedback_current_1;
    packet.feedback_current_2 = feedback_current_2;
    packet.angle_1 = angle_1;
    packet.angle_2 = angle_2;
    packet.online_mask = online_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#endif
}

/**
 * @brief Send a smart_car_motor_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_motor_status_send_struct(mavlink_channel_t chan, const mavlink_smart_car_motor_status_t* smart_car_motor_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_motor_status_send(chan, smart_car_motor_status->time_boot_ms, smart_car_motor_status->can_tx_busy_count, smart_car_motor_status->can_error_count, smart_car_motor_status->target_rpm_1, smart_car_motor_status->target_rpm_2, smart_car_motor_status->actual_rpm_1, smart_car_motor_status->actual_rpm_2, smart_car_motor_status->current_cmd_1, smart_car_motor_status->current_cmd_2, smart_car_motor_status->feedback_current_1, smart_car_motor_status->feedback_current_2, smart_car_motor_status->angle_1, smart_car_motor_status->angle_2, smart_car_motor_status->online_mask);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS, (const char *)smart_car_motor_status, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_motor_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint32_t can_tx_busy_count, uint32_t can_error_count, int16_t target_rpm_1, int16_t target_rpm_2, int16_t actual_rpm_1, int16_t actual_rpm_2, int16_t current_cmd_1, int16_t current_cmd_2, int16_t feedback_current_1, int16_t feedback_current_2, uint16_t angle_1, uint16_t angle_2, uint8_t online_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, can_tx_busy_count);
    _mav_put_uint32_t(buf, 8, can_error_count);
    _mav_put_int16_t(buf, 12, target_rpm_1);
    _mav_put_int16_t(buf, 14, target_rpm_2);
    _mav_put_int16_t(buf, 16, actual_rpm_1);
    _mav_put_int16_t(buf, 18, actual_rpm_2);
    _mav_put_int16_t(buf, 20, current_cmd_1);
    _mav_put_int16_t(buf, 22, current_cmd_2);
    _mav_put_int16_t(buf, 24, feedback_current_1);
    _mav_put_int16_t(buf, 26, feedback_current_2);
    _mav_put_uint16_t(buf, 28, angle_1);
    _mav_put_uint16_t(buf, 30, angle_2);
    _mav_put_uint8_t(buf, 32, online_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#else
    mavlink_smart_car_motor_status_t *packet = (mavlink_smart_car_motor_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->can_tx_busy_count = can_tx_busy_count;
    packet->can_error_count = can_error_count;
    packet->target_rpm_1 = target_rpm_1;
    packet->target_rpm_2 = target_rpm_2;
    packet->actual_rpm_1 = actual_rpm_1;
    packet->actual_rpm_2 = actual_rpm_2;
    packet->current_cmd_1 = current_cmd_1;
    packet->current_cmd_2 = current_cmd_2;
    packet->feedback_current_1 = feedback_current_1;
    packet->feedback_current_2 = feedback_current_2;
    packet->angle_1 = angle_1;
    packet->angle_2 = angle_2;
    packet->online_mask = online_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_MOTOR_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_motor_status message
 *
 * @return [ms] Timestamp from the vehicle.
 */
static inline uint32_t mavlink_msg_smart_car_motor_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field can_tx_busy_count from smart_car_motor_status message
 *
 * @return  CAN TX busy count.
 */
static inline uint32_t mavlink_msg_smart_car_motor_status_get_can_tx_busy_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field can_error_count from smart_car_motor_status message
 *
 * @return  CAN error count.
 */
static inline uint32_t mavlink_msg_smart_car_motor_status_get_can_error_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field target_rpm_1 from smart_car_motor_status message
 *
 * @return [rpm] Motor 1 target RPM.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_target_rpm_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field target_rpm_2 from smart_car_motor_status message
 *
 * @return [rpm] Motor 2 target RPM.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_target_rpm_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field actual_rpm_1 from smart_car_motor_status message
 *
 * @return [rpm] Motor 1 actual RPM.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_actual_rpm_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field actual_rpm_2 from smart_car_motor_status message
 *
 * @return [rpm] Motor 2 actual RPM.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_actual_rpm_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field current_cmd_1 from smart_car_motor_status message
 *
 * @return  Motor 1 command current.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_current_cmd_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Get field current_cmd_2 from smart_car_motor_status message
 *
 * @return  Motor 2 command current.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_current_cmd_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  22);
}

/**
 * @brief Get field feedback_current_1 from smart_car_motor_status message
 *
 * @return  Motor 1 feedback current.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_feedback_current_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  24);
}

/**
 * @brief Get field feedback_current_2 from smart_car_motor_status message
 *
 * @return  Motor 2 feedback current.
 */
static inline int16_t mavlink_msg_smart_car_motor_status_get_feedback_current_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  26);
}

/**
 * @brief Get field angle_1 from smart_car_motor_status message
 *
 * @return  Motor 1 encoder angle.
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_get_angle_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  28);
}

/**
 * @brief Get field angle_2 from smart_car_motor_status message
 *
 * @return  Motor 2 encoder angle.
 */
static inline uint16_t mavlink_msg_smart_car_motor_status_get_angle_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  30);
}

/**
 * @brief Get field online_mask from smart_car_motor_status message
 *
 * @return  Motor online bitmask.
 */
static inline uint8_t mavlink_msg_smart_car_motor_status_get_online_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  32);
}

/**
 * @brief Decode a smart_car_motor_status message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_motor_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_motor_status_decode(const mavlink_message_t* msg, mavlink_smart_car_motor_status_t* smart_car_motor_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_motor_status->time_boot_ms = mavlink_msg_smart_car_motor_status_get_time_boot_ms(msg);
    smart_car_motor_status->can_tx_busy_count = mavlink_msg_smart_car_motor_status_get_can_tx_busy_count(msg);
    smart_car_motor_status->can_error_count = mavlink_msg_smart_car_motor_status_get_can_error_count(msg);
    smart_car_motor_status->target_rpm_1 = mavlink_msg_smart_car_motor_status_get_target_rpm_1(msg);
    smart_car_motor_status->target_rpm_2 = mavlink_msg_smart_car_motor_status_get_target_rpm_2(msg);
    smart_car_motor_status->actual_rpm_1 = mavlink_msg_smart_car_motor_status_get_actual_rpm_1(msg);
    smart_car_motor_status->actual_rpm_2 = mavlink_msg_smart_car_motor_status_get_actual_rpm_2(msg);
    smart_car_motor_status->current_cmd_1 = mavlink_msg_smart_car_motor_status_get_current_cmd_1(msg);
    smart_car_motor_status->current_cmd_2 = mavlink_msg_smart_car_motor_status_get_current_cmd_2(msg);
    smart_car_motor_status->feedback_current_1 = mavlink_msg_smart_car_motor_status_get_feedback_current_1(msg);
    smart_car_motor_status->feedback_current_2 = mavlink_msg_smart_car_motor_status_get_feedback_current_2(msg);
    smart_car_motor_status->angle_1 = mavlink_msg_smart_car_motor_status_get_angle_1(msg);
    smart_car_motor_status->angle_2 = mavlink_msg_smart_car_motor_status_get_angle_2(msg);
    smart_car_motor_status->online_mask = mavlink_msg_smart_car_motor_status_get_online_mask(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN;
        memset(smart_car_motor_status, 0, MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_LEN);
    memcpy(smart_car_motor_status, _MAV_PAYLOAD(msg), len);
#endif
}
