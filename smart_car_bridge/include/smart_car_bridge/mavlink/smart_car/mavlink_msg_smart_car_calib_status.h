#pragma once
// MESSAGE SMART_CAR_CALIB_STATUS PACKING

#define MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS 42006


typedef struct __mavlink_smart_car_calib_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the vehicle.*/
 uint32_t point_id; /*<  Calibration point ID.*/
 uint32_t sweep_index; /*<  Current sweep index.*/
 uint32_t sweep_count; /*<  Sweep point count.*/
 uint32_t valid_count; /*<  Valid sample count.*/
 uint32_t invalid_count; /*<  Invalid sample count.*/
 float v_center_avg; /*< [m/s] Average center speed.*/
 float yaw_rate_avg; /*< [deg/s] Average yaw rate.*/
 float kappa_avg; /*< [1/m] Average curvature.*/
 float radius_est; /*< [m] Estimated radius.*/
 int16_t target_rpm; /*< [rpm] Calibration target RPM.*/
 uint16_t servo_pwm_us; /*< [us] Calibration servo pulse.*/
 uint8_t state; /*<  Calibration state.*/
 uint8_t sweep_enabled; /*<  Sweep enabled flag.*/
 uint8_t yaw_sign_inverted; /*<  Yaw sign inverted flag.*/
} mavlink_smart_car_calib_status_t;

#define MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN 47
#define MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN 47
#define MAVLINK_MSG_ID_42006_LEN 47
#define MAVLINK_MSG_ID_42006_MIN_LEN 47

#define MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC 30
#define MAVLINK_MSG_ID_42006_CRC 30



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_CALIB_STATUS { \
    42006, \
    "SMART_CAR_CALIB_STATUS", \
    15, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_calib_status_t, time_boot_ms) }, \
         { "point_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_calib_status_t, point_id) }, \
         { "sweep_index", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_calib_status_t, sweep_index) }, \
         { "sweep_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_smart_car_calib_status_t, sweep_count) }, \
         { "valid_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_smart_car_calib_status_t, valid_count) }, \
         { "invalid_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 20, offsetof(mavlink_smart_car_calib_status_t, invalid_count) }, \
         { "v_center_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_smart_car_calib_status_t, v_center_avg) }, \
         { "yaw_rate_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_calib_status_t, yaw_rate_avg) }, \
         { "kappa_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_smart_car_calib_status_t, kappa_avg) }, \
         { "radius_est", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_smart_car_calib_status_t, radius_est) }, \
         { "target_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_smart_car_calib_status_t, target_rpm) }, \
         { "servo_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 42, offsetof(mavlink_smart_car_calib_status_t, servo_pwm_us) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_smart_car_calib_status_t, state) }, \
         { "sweep_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_smart_car_calib_status_t, sweep_enabled) }, \
         { "yaw_sign_inverted", NULL, MAVLINK_TYPE_UINT8_T, 0, 46, offsetof(mavlink_smart_car_calib_status_t, yaw_sign_inverted) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_CALIB_STATUS { \
    "SMART_CAR_CALIB_STATUS", \
    15, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_calib_status_t, time_boot_ms) }, \
         { "point_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_calib_status_t, point_id) }, \
         { "sweep_index", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_calib_status_t, sweep_index) }, \
         { "sweep_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_smart_car_calib_status_t, sweep_count) }, \
         { "valid_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_smart_car_calib_status_t, valid_count) }, \
         { "invalid_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 20, offsetof(mavlink_smart_car_calib_status_t, invalid_count) }, \
         { "v_center_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_smart_car_calib_status_t, v_center_avg) }, \
         { "yaw_rate_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_calib_status_t, yaw_rate_avg) }, \
         { "kappa_avg", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_smart_car_calib_status_t, kappa_avg) }, \
         { "radius_est", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_smart_car_calib_status_t, radius_est) }, \
         { "target_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_smart_car_calib_status_t, target_rpm) }, \
         { "servo_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 42, offsetof(mavlink_smart_car_calib_status_t, servo_pwm_us) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_smart_car_calib_status_t, state) }, \
         { "sweep_enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_smart_car_calib_status_t, sweep_enabled) }, \
         { "yaw_sign_inverted", NULL, MAVLINK_TYPE_UINT8_T, 0, 46, offsetof(mavlink_smart_car_calib_status_t, yaw_sign_inverted) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_calib_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param point_id  Calibration point ID.
 * @param sweep_index  Current sweep index.
 * @param sweep_count  Sweep point count.
 * @param valid_count  Valid sample count.
 * @param invalid_count  Invalid sample count.
 * @param v_center_avg [m/s] Average center speed.
 * @param yaw_rate_avg [deg/s] Average yaw rate.
 * @param kappa_avg [1/m] Average curvature.
 * @param radius_est [m] Estimated radius.
 * @param target_rpm [rpm] Calibration target RPM.
 * @param servo_pwm_us [us] Calibration servo pulse.
 * @param state  Calibration state.
 * @param sweep_enabled  Sweep enabled flag.
 * @param yaw_sign_inverted  Yaw sign inverted flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t point_id, uint32_t sweep_index, uint32_t sweep_count, uint32_t valid_count, uint32_t invalid_count, float v_center_avg, float yaw_rate_avg, float kappa_avg, float radius_est, int16_t target_rpm, uint16_t servo_pwm_us, uint8_t state, uint8_t sweep_enabled, uint8_t yaw_sign_inverted)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, point_id);
    _mav_put_uint32_t(buf, 8, sweep_index);
    _mav_put_uint32_t(buf, 12, sweep_count);
    _mav_put_uint32_t(buf, 16, valid_count);
    _mav_put_uint32_t(buf, 20, invalid_count);
    _mav_put_float(buf, 24, v_center_avg);
    _mav_put_float(buf, 28, yaw_rate_avg);
    _mav_put_float(buf, 32, kappa_avg);
    _mav_put_float(buf, 36, radius_est);
    _mav_put_int16_t(buf, 40, target_rpm);
    _mav_put_uint16_t(buf, 42, servo_pwm_us);
    _mav_put_uint8_t(buf, 44, state);
    _mav_put_uint8_t(buf, 45, sweep_enabled);
    _mav_put_uint8_t(buf, 46, yaw_sign_inverted);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#else
    mavlink_smart_car_calib_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.point_id = point_id;
    packet.sweep_index = sweep_index;
    packet.sweep_count = sweep_count;
    packet.valid_count = valid_count;
    packet.invalid_count = invalid_count;
    packet.v_center_avg = v_center_avg;
    packet.yaw_rate_avg = yaw_rate_avg;
    packet.kappa_avg = kappa_avg;
    packet.radius_est = radius_est;
    packet.target_rpm = target_rpm;
    packet.servo_pwm_us = servo_pwm_us;
    packet.state = state;
    packet.sweep_enabled = sweep_enabled;
    packet.yaw_sign_inverted = yaw_sign_inverted;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
}

/**
 * @brief Pack a smart_car_calib_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param point_id  Calibration point ID.
 * @param sweep_index  Current sweep index.
 * @param sweep_count  Sweep point count.
 * @param valid_count  Valid sample count.
 * @param invalid_count  Invalid sample count.
 * @param v_center_avg [m/s] Average center speed.
 * @param yaw_rate_avg [deg/s] Average yaw rate.
 * @param kappa_avg [1/m] Average curvature.
 * @param radius_est [m] Estimated radius.
 * @param target_rpm [rpm] Calibration target RPM.
 * @param servo_pwm_us [us] Calibration servo pulse.
 * @param state  Calibration state.
 * @param sweep_enabled  Sweep enabled flag.
 * @param yaw_sign_inverted  Yaw sign inverted flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t point_id, uint32_t sweep_index, uint32_t sweep_count, uint32_t valid_count, uint32_t invalid_count, float v_center_avg, float yaw_rate_avg, float kappa_avg, float radius_est, int16_t target_rpm, uint16_t servo_pwm_us, uint8_t state, uint8_t sweep_enabled, uint8_t yaw_sign_inverted)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, point_id);
    _mav_put_uint32_t(buf, 8, sweep_index);
    _mav_put_uint32_t(buf, 12, sweep_count);
    _mav_put_uint32_t(buf, 16, valid_count);
    _mav_put_uint32_t(buf, 20, invalid_count);
    _mav_put_float(buf, 24, v_center_avg);
    _mav_put_float(buf, 28, yaw_rate_avg);
    _mav_put_float(buf, 32, kappa_avg);
    _mav_put_float(buf, 36, radius_est);
    _mav_put_int16_t(buf, 40, target_rpm);
    _mav_put_uint16_t(buf, 42, servo_pwm_us);
    _mav_put_uint8_t(buf, 44, state);
    _mav_put_uint8_t(buf, 45, sweep_enabled);
    _mav_put_uint8_t(buf, 46, yaw_sign_inverted);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#else
    mavlink_smart_car_calib_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.point_id = point_id;
    packet.sweep_index = sweep_index;
    packet.sweep_count = sweep_count;
    packet.valid_count = valid_count;
    packet.invalid_count = invalid_count;
    packet.v_center_avg = v_center_avg;
    packet.yaw_rate_avg = yaw_rate_avg;
    packet.kappa_avg = kappa_avg;
    packet.radius_est = radius_est;
    packet.target_rpm = target_rpm;
    packet.servo_pwm_us = servo_pwm_us;
    packet.state = state;
    packet.sweep_enabled = sweep_enabled;
    packet.yaw_sign_inverted = yaw_sign_inverted;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#endif
}

/**
 * @brief Pack a smart_car_calib_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param point_id  Calibration point ID.
 * @param sweep_index  Current sweep index.
 * @param sweep_count  Sweep point count.
 * @param valid_count  Valid sample count.
 * @param invalid_count  Invalid sample count.
 * @param v_center_avg [m/s] Average center speed.
 * @param yaw_rate_avg [deg/s] Average yaw rate.
 * @param kappa_avg [1/m] Average curvature.
 * @param radius_est [m] Estimated radius.
 * @param target_rpm [rpm] Calibration target RPM.
 * @param servo_pwm_us [us] Calibration servo pulse.
 * @param state  Calibration state.
 * @param sweep_enabled  Sweep enabled flag.
 * @param yaw_sign_inverted  Yaw sign inverted flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint32_t point_id,uint32_t sweep_index,uint32_t sweep_count,uint32_t valid_count,uint32_t invalid_count,float v_center_avg,float yaw_rate_avg,float kappa_avg,float radius_est,int16_t target_rpm,uint16_t servo_pwm_us,uint8_t state,uint8_t sweep_enabled,uint8_t yaw_sign_inverted)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, point_id);
    _mav_put_uint32_t(buf, 8, sweep_index);
    _mav_put_uint32_t(buf, 12, sweep_count);
    _mav_put_uint32_t(buf, 16, valid_count);
    _mav_put_uint32_t(buf, 20, invalid_count);
    _mav_put_float(buf, 24, v_center_avg);
    _mav_put_float(buf, 28, yaw_rate_avg);
    _mav_put_float(buf, 32, kappa_avg);
    _mav_put_float(buf, 36, radius_est);
    _mav_put_int16_t(buf, 40, target_rpm);
    _mav_put_uint16_t(buf, 42, servo_pwm_us);
    _mav_put_uint8_t(buf, 44, state);
    _mav_put_uint8_t(buf, 45, sweep_enabled);
    _mav_put_uint8_t(buf, 46, yaw_sign_inverted);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#else
    mavlink_smart_car_calib_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.point_id = point_id;
    packet.sweep_index = sweep_index;
    packet.sweep_count = sweep_count;
    packet.valid_count = valid_count;
    packet.invalid_count = invalid_count;
    packet.v_center_avg = v_center_avg;
    packet.yaw_rate_avg = yaw_rate_avg;
    packet.kappa_avg = kappa_avg;
    packet.radius_est = radius_est;
    packet.target_rpm = target_rpm;
    packet.servo_pwm_us = servo_pwm_us;
    packet.state = state;
    packet.sweep_enabled = sweep_enabled;
    packet.yaw_sign_inverted = yaw_sign_inverted;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
}

/**
 * @brief Encode a smart_car_calib_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_calib_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_calib_status_t* smart_car_calib_status)
{
    return mavlink_msg_smart_car_calib_status_pack(system_id, component_id, msg, smart_car_calib_status->time_boot_ms, smart_car_calib_status->point_id, smart_car_calib_status->sweep_index, smart_car_calib_status->sweep_count, smart_car_calib_status->valid_count, smart_car_calib_status->invalid_count, smart_car_calib_status->v_center_avg, smart_car_calib_status->yaw_rate_avg, smart_car_calib_status->kappa_avg, smart_car_calib_status->radius_est, smart_car_calib_status->target_rpm, smart_car_calib_status->servo_pwm_us, smart_car_calib_status->state, smart_car_calib_status->sweep_enabled, smart_car_calib_status->yaw_sign_inverted);
}

/**
 * @brief Encode a smart_car_calib_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_calib_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_calib_status_t* smart_car_calib_status)
{
    return mavlink_msg_smart_car_calib_status_pack_chan(system_id, component_id, chan, msg, smart_car_calib_status->time_boot_ms, smart_car_calib_status->point_id, smart_car_calib_status->sweep_index, smart_car_calib_status->sweep_count, smart_car_calib_status->valid_count, smart_car_calib_status->invalid_count, smart_car_calib_status->v_center_avg, smart_car_calib_status->yaw_rate_avg, smart_car_calib_status->kappa_avg, smart_car_calib_status->radius_est, smart_car_calib_status->target_rpm, smart_car_calib_status->servo_pwm_us, smart_car_calib_status->state, smart_car_calib_status->sweep_enabled, smart_car_calib_status->yaw_sign_inverted);
}

/**
 * @brief Encode a smart_car_calib_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_calib_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_calib_status_t* smart_car_calib_status)
{
    return mavlink_msg_smart_car_calib_status_pack_status(system_id, component_id, _status, msg,  smart_car_calib_status->time_boot_ms, smart_car_calib_status->point_id, smart_car_calib_status->sweep_index, smart_car_calib_status->sweep_count, smart_car_calib_status->valid_count, smart_car_calib_status->invalid_count, smart_car_calib_status->v_center_avg, smart_car_calib_status->yaw_rate_avg, smart_car_calib_status->kappa_avg, smart_car_calib_status->radius_est, smart_car_calib_status->target_rpm, smart_car_calib_status->servo_pwm_us, smart_car_calib_status->state, smart_car_calib_status->sweep_enabled, smart_car_calib_status->yaw_sign_inverted);
}

/**
 * @brief Send a smart_car_calib_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param point_id  Calibration point ID.
 * @param sweep_index  Current sweep index.
 * @param sweep_count  Sweep point count.
 * @param valid_count  Valid sample count.
 * @param invalid_count  Invalid sample count.
 * @param v_center_avg [m/s] Average center speed.
 * @param yaw_rate_avg [deg/s] Average yaw rate.
 * @param kappa_avg [1/m] Average curvature.
 * @param radius_est [m] Estimated radius.
 * @param target_rpm [rpm] Calibration target RPM.
 * @param servo_pwm_us [us] Calibration servo pulse.
 * @param state  Calibration state.
 * @param sweep_enabled  Sweep enabled flag.
 * @param yaw_sign_inverted  Yaw sign inverted flag.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_calib_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint32_t point_id, uint32_t sweep_index, uint32_t sweep_count, uint32_t valid_count, uint32_t invalid_count, float v_center_avg, float yaw_rate_avg, float kappa_avg, float radius_est, int16_t target_rpm, uint16_t servo_pwm_us, uint8_t state, uint8_t sweep_enabled, uint8_t yaw_sign_inverted)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, point_id);
    _mav_put_uint32_t(buf, 8, sweep_index);
    _mav_put_uint32_t(buf, 12, sweep_count);
    _mav_put_uint32_t(buf, 16, valid_count);
    _mav_put_uint32_t(buf, 20, invalid_count);
    _mav_put_float(buf, 24, v_center_avg);
    _mav_put_float(buf, 28, yaw_rate_avg);
    _mav_put_float(buf, 32, kappa_avg);
    _mav_put_float(buf, 36, radius_est);
    _mav_put_int16_t(buf, 40, target_rpm);
    _mav_put_uint16_t(buf, 42, servo_pwm_us);
    _mav_put_uint8_t(buf, 44, state);
    _mav_put_uint8_t(buf, 45, sweep_enabled);
    _mav_put_uint8_t(buf, 46, yaw_sign_inverted);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#else
    mavlink_smart_car_calib_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.point_id = point_id;
    packet.sweep_index = sweep_index;
    packet.sweep_count = sweep_count;
    packet.valid_count = valid_count;
    packet.invalid_count = invalid_count;
    packet.v_center_avg = v_center_avg;
    packet.yaw_rate_avg = yaw_rate_avg;
    packet.kappa_avg = kappa_avg;
    packet.radius_est = radius_est;
    packet.target_rpm = target_rpm;
    packet.servo_pwm_us = servo_pwm_us;
    packet.state = state;
    packet.sweep_enabled = sweep_enabled;
    packet.yaw_sign_inverted = yaw_sign_inverted;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#endif
}

/**
 * @brief Send a smart_car_calib_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_calib_status_send_struct(mavlink_channel_t chan, const mavlink_smart_car_calib_status_t* smart_car_calib_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_calib_status_send(chan, smart_car_calib_status->time_boot_ms, smart_car_calib_status->point_id, smart_car_calib_status->sweep_index, smart_car_calib_status->sweep_count, smart_car_calib_status->valid_count, smart_car_calib_status->invalid_count, smart_car_calib_status->v_center_avg, smart_car_calib_status->yaw_rate_avg, smart_car_calib_status->kappa_avg, smart_car_calib_status->radius_est, smart_car_calib_status->target_rpm, smart_car_calib_status->servo_pwm_us, smart_car_calib_status->state, smart_car_calib_status->sweep_enabled, smart_car_calib_status->yaw_sign_inverted);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS, (const char *)smart_car_calib_status, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_calib_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint32_t point_id, uint32_t sweep_index, uint32_t sweep_count, uint32_t valid_count, uint32_t invalid_count, float v_center_avg, float yaw_rate_avg, float kappa_avg, float radius_est, int16_t target_rpm, uint16_t servo_pwm_us, uint8_t state, uint8_t sweep_enabled, uint8_t yaw_sign_inverted)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, point_id);
    _mav_put_uint32_t(buf, 8, sweep_index);
    _mav_put_uint32_t(buf, 12, sweep_count);
    _mav_put_uint32_t(buf, 16, valid_count);
    _mav_put_uint32_t(buf, 20, invalid_count);
    _mav_put_float(buf, 24, v_center_avg);
    _mav_put_float(buf, 28, yaw_rate_avg);
    _mav_put_float(buf, 32, kappa_avg);
    _mav_put_float(buf, 36, radius_est);
    _mav_put_int16_t(buf, 40, target_rpm);
    _mav_put_uint16_t(buf, 42, servo_pwm_us);
    _mav_put_uint8_t(buf, 44, state);
    _mav_put_uint8_t(buf, 45, sweep_enabled);
    _mav_put_uint8_t(buf, 46, yaw_sign_inverted);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#else
    mavlink_smart_car_calib_status_t *packet = (mavlink_smart_car_calib_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->point_id = point_id;
    packet->sweep_index = sweep_index;
    packet->sweep_count = sweep_count;
    packet->valid_count = valid_count;
    packet->invalid_count = invalid_count;
    packet->v_center_avg = v_center_avg;
    packet->yaw_rate_avg = yaw_rate_avg;
    packet->kappa_avg = kappa_avg;
    packet->radius_est = radius_est;
    packet->target_rpm = target_rpm;
    packet->servo_pwm_us = servo_pwm_us;
    packet->state = state;
    packet->sweep_enabled = sweep_enabled;
    packet->yaw_sign_inverted = yaw_sign_inverted;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_CALIB_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_calib_status message
 *
 * @return [ms] Timestamp from the vehicle.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field point_id from smart_car_calib_status message
 *
 * @return  Calibration point ID.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_point_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field sweep_index from smart_car_calib_status message
 *
 * @return  Current sweep index.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_sweep_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field sweep_count from smart_car_calib_status message
 *
 * @return  Sweep point count.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_sweep_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  12);
}

/**
 * @brief Get field valid_count from smart_car_calib_status message
 *
 * @return  Valid sample count.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_valid_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  16);
}

/**
 * @brief Get field invalid_count from smart_car_calib_status message
 *
 * @return  Invalid sample count.
 */
static inline uint32_t mavlink_msg_smart_car_calib_status_get_invalid_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  20);
}

/**
 * @brief Get field v_center_avg from smart_car_calib_status message
 *
 * @return [m/s] Average center speed.
 */
static inline float mavlink_msg_smart_car_calib_status_get_v_center_avg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yaw_rate_avg from smart_car_calib_status message
 *
 * @return [deg/s] Average yaw rate.
 */
static inline float mavlink_msg_smart_car_calib_status_get_yaw_rate_avg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field kappa_avg from smart_car_calib_status message
 *
 * @return [1/m] Average curvature.
 */
static inline float mavlink_msg_smart_car_calib_status_get_kappa_avg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field radius_est from smart_car_calib_status message
 *
 * @return [m] Estimated radius.
 */
static inline float mavlink_msg_smart_car_calib_status_get_radius_est(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field target_rpm from smart_car_calib_status message
 *
 * @return [rpm] Calibration target RPM.
 */
static inline int16_t mavlink_msg_smart_car_calib_status_get_target_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  40);
}

/**
 * @brief Get field servo_pwm_us from smart_car_calib_status message
 *
 * @return [us] Calibration servo pulse.
 */
static inline uint16_t mavlink_msg_smart_car_calib_status_get_servo_pwm_us(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  42);
}

/**
 * @brief Get field state from smart_car_calib_status message
 *
 * @return  Calibration state.
 */
static inline uint8_t mavlink_msg_smart_car_calib_status_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  44);
}

/**
 * @brief Get field sweep_enabled from smart_car_calib_status message
 *
 * @return  Sweep enabled flag.
 */
static inline uint8_t mavlink_msg_smart_car_calib_status_get_sweep_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  45);
}

/**
 * @brief Get field yaw_sign_inverted from smart_car_calib_status message
 *
 * @return  Yaw sign inverted flag.
 */
static inline uint8_t mavlink_msg_smart_car_calib_status_get_yaw_sign_inverted(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  46);
}

/**
 * @brief Decode a smart_car_calib_status message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_calib_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_calib_status_decode(const mavlink_message_t* msg, mavlink_smart_car_calib_status_t* smart_car_calib_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_calib_status->time_boot_ms = mavlink_msg_smart_car_calib_status_get_time_boot_ms(msg);
    smart_car_calib_status->point_id = mavlink_msg_smart_car_calib_status_get_point_id(msg);
    smart_car_calib_status->sweep_index = mavlink_msg_smart_car_calib_status_get_sweep_index(msg);
    smart_car_calib_status->sweep_count = mavlink_msg_smart_car_calib_status_get_sweep_count(msg);
    smart_car_calib_status->valid_count = mavlink_msg_smart_car_calib_status_get_valid_count(msg);
    smart_car_calib_status->invalid_count = mavlink_msg_smart_car_calib_status_get_invalid_count(msg);
    smart_car_calib_status->v_center_avg = mavlink_msg_smart_car_calib_status_get_v_center_avg(msg);
    smart_car_calib_status->yaw_rate_avg = mavlink_msg_smart_car_calib_status_get_yaw_rate_avg(msg);
    smart_car_calib_status->kappa_avg = mavlink_msg_smart_car_calib_status_get_kappa_avg(msg);
    smart_car_calib_status->radius_est = mavlink_msg_smart_car_calib_status_get_radius_est(msg);
    smart_car_calib_status->target_rpm = mavlink_msg_smart_car_calib_status_get_target_rpm(msg);
    smart_car_calib_status->servo_pwm_us = mavlink_msg_smart_car_calib_status_get_servo_pwm_us(msg);
    smart_car_calib_status->state = mavlink_msg_smart_car_calib_status_get_state(msg);
    smart_car_calib_status->sweep_enabled = mavlink_msg_smart_car_calib_status_get_sweep_enabled(msg);
    smart_car_calib_status->yaw_sign_inverted = mavlink_msg_smart_car_calib_status_get_yaw_sign_inverted(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN;
        memset(smart_car_calib_status, 0, MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_LEN);
    memcpy(smart_car_calib_status, _MAV_PAYLOAD(msg), len);
#endif
}
