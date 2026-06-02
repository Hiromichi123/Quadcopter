#pragma once
// MESSAGE SMART_CAR_ACTUATOR_TEST PACKING

#define MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST 42005


typedef struct __mavlink_smart_car_actuator_test_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the host.*/
 float servo_angle_deg; /*< [deg] Servo test angle.*/
 uint16_t test_mask; /*<  Test selector bitmask.*/
 uint16_t servo_pwm_us; /*< [us] Servo test pulse.*/
 uint16_t duration_ms; /*< [ms] Test duration. Zero means continuous.*/
 int16_t motor1_rpm; /*< [rpm] Motor 1 test target.*/
 int16_t motor2_rpm; /*< [rpm] Motor 2 test target.*/
 uint8_t target_system; /*<  System ID.*/
 uint8_t target_component; /*<  Component ID.*/
} mavlink_smart_car_actuator_test_t;

#define MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN 20
#define MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN 20
#define MAVLINK_MSG_ID_42005_LEN 20
#define MAVLINK_MSG_ID_42005_MIN_LEN 20

#define MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC 105
#define MAVLINK_MSG_ID_42005_CRC 105



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_ACTUATOR_TEST { \
    42005, \
    "SMART_CAR_ACTUATOR_TEST", \
    9, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_actuator_test_t, time_boot_ms) }, \
         { "servo_angle_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_actuator_test_t, servo_angle_deg) }, \
         { "test_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_smart_car_actuator_test_t, test_mask) }, \
         { "servo_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_smart_car_actuator_test_t, servo_pwm_us) }, \
         { "duration_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_smart_car_actuator_test_t, duration_ms) }, \
         { "motor1_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_smart_car_actuator_test_t, motor1_rpm) }, \
         { "motor2_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_smart_car_actuator_test_t, motor2_rpm) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_smart_car_actuator_test_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_smart_car_actuator_test_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_ACTUATOR_TEST { \
    "SMART_CAR_ACTUATOR_TEST", \
    9, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_actuator_test_t, time_boot_ms) }, \
         { "servo_angle_deg", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_smart_car_actuator_test_t, servo_angle_deg) }, \
         { "test_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_smart_car_actuator_test_t, test_mask) }, \
         { "servo_pwm_us", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_smart_car_actuator_test_t, servo_pwm_us) }, \
         { "duration_ms", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_smart_car_actuator_test_t, duration_ms) }, \
         { "motor1_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_smart_car_actuator_test_t, motor1_rpm) }, \
         { "motor2_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_smart_car_actuator_test_t, motor2_rpm) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_smart_car_actuator_test_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_smart_car_actuator_test_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_actuator_test message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param servo_angle_deg [deg] Servo test angle.
 * @param test_mask  Test selector bitmask.
 * @param servo_pwm_us [us] Servo test pulse.
 * @param duration_ms [ms] Test duration. Zero means continuous.
 * @param motor1_rpm [rpm] Motor 1 test target.
 * @param motor2_rpm [rpm] Motor 2 test target.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float servo_angle_deg, uint16_t test_mask, uint16_t servo_pwm_us, uint16_t duration_ms, int16_t motor1_rpm, int16_t motor2_rpm, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, servo_angle_deg);
    _mav_put_uint16_t(buf, 8, test_mask);
    _mav_put_uint16_t(buf, 10, servo_pwm_us);
    _mav_put_uint16_t(buf, 12, duration_ms);
    _mav_put_int16_t(buf, 14, motor1_rpm);
    _mav_put_int16_t(buf, 16, motor2_rpm);
    _mav_put_uint8_t(buf, 18, target_system);
    _mav_put_uint8_t(buf, 19, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#else
    mavlink_smart_car_actuator_test_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.servo_angle_deg = servo_angle_deg;
    packet.test_mask = test_mask;
    packet.servo_pwm_us = servo_pwm_us;
    packet.duration_ms = duration_ms;
    packet.motor1_rpm = motor1_rpm;
    packet.motor2_rpm = motor2_rpm;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
}

/**
 * @brief Pack a smart_car_actuator_test message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param servo_angle_deg [deg] Servo test angle.
 * @param test_mask  Test selector bitmask.
 * @param servo_pwm_us [us] Servo test pulse.
 * @param duration_ms [ms] Test duration. Zero means continuous.
 * @param motor1_rpm [rpm] Motor 1 test target.
 * @param motor2_rpm [rpm] Motor 2 test target.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, float servo_angle_deg, uint16_t test_mask, uint16_t servo_pwm_us, uint16_t duration_ms, int16_t motor1_rpm, int16_t motor2_rpm, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, servo_angle_deg);
    _mav_put_uint16_t(buf, 8, test_mask);
    _mav_put_uint16_t(buf, 10, servo_pwm_us);
    _mav_put_uint16_t(buf, 12, duration_ms);
    _mav_put_int16_t(buf, 14, motor1_rpm);
    _mav_put_int16_t(buf, 16, motor2_rpm);
    _mav_put_uint8_t(buf, 18, target_system);
    _mav_put_uint8_t(buf, 19, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#else
    mavlink_smart_car_actuator_test_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.servo_angle_deg = servo_angle_deg;
    packet.test_mask = test_mask;
    packet.servo_pwm_us = servo_pwm_us;
    packet.duration_ms = duration_ms;
    packet.motor1_rpm = motor1_rpm;
    packet.motor2_rpm = motor2_rpm;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#endif
}

/**
 * @brief Pack a smart_car_actuator_test message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param servo_angle_deg [deg] Servo test angle.
 * @param test_mask  Test selector bitmask.
 * @param servo_pwm_us [us] Servo test pulse.
 * @param duration_ms [ms] Test duration. Zero means continuous.
 * @param motor1_rpm [rpm] Motor 1 test target.
 * @param motor2_rpm [rpm] Motor 2 test target.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,float servo_angle_deg,uint16_t test_mask,uint16_t servo_pwm_us,uint16_t duration_ms,int16_t motor1_rpm,int16_t motor2_rpm,uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, servo_angle_deg);
    _mav_put_uint16_t(buf, 8, test_mask);
    _mav_put_uint16_t(buf, 10, servo_pwm_us);
    _mav_put_uint16_t(buf, 12, duration_ms);
    _mav_put_int16_t(buf, 14, motor1_rpm);
    _mav_put_int16_t(buf, 16, motor2_rpm);
    _mav_put_uint8_t(buf, 18, target_system);
    _mav_put_uint8_t(buf, 19, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#else
    mavlink_smart_car_actuator_test_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.servo_angle_deg = servo_angle_deg;
    packet.test_mask = test_mask;
    packet.servo_pwm_us = servo_pwm_us;
    packet.duration_ms = duration_ms;
    packet.motor1_rpm = motor1_rpm;
    packet.motor2_rpm = motor2_rpm;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
}

/**
 * @brief Encode a smart_car_actuator_test struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_actuator_test C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_actuator_test_t* smart_car_actuator_test)
{
    return mavlink_msg_smart_car_actuator_test_pack(system_id, component_id, msg, smart_car_actuator_test->time_boot_ms, smart_car_actuator_test->servo_angle_deg, smart_car_actuator_test->test_mask, smart_car_actuator_test->servo_pwm_us, smart_car_actuator_test->duration_ms, smart_car_actuator_test->motor1_rpm, smart_car_actuator_test->motor2_rpm, smart_car_actuator_test->target_system, smart_car_actuator_test->target_component);
}

/**
 * @brief Encode a smart_car_actuator_test struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_actuator_test C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_actuator_test_t* smart_car_actuator_test)
{
    return mavlink_msg_smart_car_actuator_test_pack_chan(system_id, component_id, chan, msg, smart_car_actuator_test->time_boot_ms, smart_car_actuator_test->servo_angle_deg, smart_car_actuator_test->test_mask, smart_car_actuator_test->servo_pwm_us, smart_car_actuator_test->duration_ms, smart_car_actuator_test->motor1_rpm, smart_car_actuator_test->motor2_rpm, smart_car_actuator_test->target_system, smart_car_actuator_test->target_component);
}

/**
 * @brief Encode a smart_car_actuator_test struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_actuator_test C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_actuator_test_t* smart_car_actuator_test)
{
    return mavlink_msg_smart_car_actuator_test_pack_status(system_id, component_id, _status, msg,  smart_car_actuator_test->time_boot_ms, smart_car_actuator_test->servo_angle_deg, smart_car_actuator_test->test_mask, smart_car_actuator_test->servo_pwm_us, smart_car_actuator_test->duration_ms, smart_car_actuator_test->motor1_rpm, smart_car_actuator_test->motor2_rpm, smart_car_actuator_test->target_system, smart_car_actuator_test->target_component);
}

/**
 * @brief Send a smart_car_actuator_test message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the host.
 * @param servo_angle_deg [deg] Servo test angle.
 * @param test_mask  Test selector bitmask.
 * @param servo_pwm_us [us] Servo test pulse.
 * @param duration_ms [ms] Test duration. Zero means continuous.
 * @param motor1_rpm [rpm] Motor 1 test target.
 * @param motor2_rpm [rpm] Motor 2 test target.
 * @param target_system  System ID.
 * @param target_component  Component ID.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_actuator_test_send(mavlink_channel_t chan, uint32_t time_boot_ms, float servo_angle_deg, uint16_t test_mask, uint16_t servo_pwm_us, uint16_t duration_ms, int16_t motor1_rpm, int16_t motor2_rpm, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, servo_angle_deg);
    _mav_put_uint16_t(buf, 8, test_mask);
    _mav_put_uint16_t(buf, 10, servo_pwm_us);
    _mav_put_uint16_t(buf, 12, duration_ms);
    _mav_put_int16_t(buf, 14, motor1_rpm);
    _mav_put_int16_t(buf, 16, motor2_rpm);
    _mav_put_uint8_t(buf, 18, target_system);
    _mav_put_uint8_t(buf, 19, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST, buf, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#else
    mavlink_smart_car_actuator_test_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.servo_angle_deg = servo_angle_deg;
    packet.test_mask = test_mask;
    packet.servo_pwm_us = servo_pwm_us;
    packet.duration_ms = duration_ms;
    packet.motor1_rpm = motor1_rpm;
    packet.motor2_rpm = motor2_rpm;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#endif
}

/**
 * @brief Send a smart_car_actuator_test message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_actuator_test_send_struct(mavlink_channel_t chan, const mavlink_smart_car_actuator_test_t* smart_car_actuator_test)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_actuator_test_send(chan, smart_car_actuator_test->time_boot_ms, smart_car_actuator_test->servo_angle_deg, smart_car_actuator_test->test_mask, smart_car_actuator_test->servo_pwm_us, smart_car_actuator_test->duration_ms, smart_car_actuator_test->motor1_rpm, smart_car_actuator_test->motor2_rpm, smart_car_actuator_test->target_system, smart_car_actuator_test->target_component);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST, (const char *)smart_car_actuator_test, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_actuator_test_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, float servo_angle_deg, uint16_t test_mask, uint16_t servo_pwm_us, uint16_t duration_ms, int16_t motor1_rpm, int16_t motor2_rpm, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, servo_angle_deg);
    _mav_put_uint16_t(buf, 8, test_mask);
    _mav_put_uint16_t(buf, 10, servo_pwm_us);
    _mav_put_uint16_t(buf, 12, duration_ms);
    _mav_put_int16_t(buf, 14, motor1_rpm);
    _mav_put_int16_t(buf, 16, motor2_rpm);
    _mav_put_uint8_t(buf, 18, target_system);
    _mav_put_uint8_t(buf, 19, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST, buf, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#else
    mavlink_smart_car_actuator_test_t *packet = (mavlink_smart_car_actuator_test_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->servo_angle_deg = servo_angle_deg;
    packet->test_mask = test_mask;
    packet->servo_pwm_us = servo_pwm_us;
    packet->duration_ms = duration_ms;
    packet->motor1_rpm = motor1_rpm;
    packet->motor2_rpm = motor2_rpm;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_ACTUATOR_TEST UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_actuator_test message
 *
 * @return [ms] Timestamp from the host.
 */
static inline uint32_t mavlink_msg_smart_car_actuator_test_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field servo_angle_deg from smart_car_actuator_test message
 *
 * @return [deg] Servo test angle.
 */
static inline float mavlink_msg_smart_car_actuator_test_get_servo_angle_deg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field test_mask from smart_car_actuator_test message
 *
 * @return  Test selector bitmask.
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_get_test_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field servo_pwm_us from smart_car_actuator_test message
 *
 * @return [us] Servo test pulse.
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_get_servo_pwm_us(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field duration_ms from smart_car_actuator_test message
 *
 * @return [ms] Test duration. Zero means continuous.
 */
static inline uint16_t mavlink_msg_smart_car_actuator_test_get_duration_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field motor1_rpm from smart_car_actuator_test message
 *
 * @return [rpm] Motor 1 test target.
 */
static inline int16_t mavlink_msg_smart_car_actuator_test_get_motor1_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field motor2_rpm from smart_car_actuator_test message
 *
 * @return [rpm] Motor 2 test target.
 */
static inline int16_t mavlink_msg_smart_car_actuator_test_get_motor2_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field target_system from smart_car_actuator_test message
 *
 * @return  System ID.
 */
static inline uint8_t mavlink_msg_smart_car_actuator_test_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field target_component from smart_car_actuator_test message
 *
 * @return  Component ID.
 */
static inline uint8_t mavlink_msg_smart_car_actuator_test_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Decode a smart_car_actuator_test message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_actuator_test C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_actuator_test_decode(const mavlink_message_t* msg, mavlink_smart_car_actuator_test_t* smart_car_actuator_test)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_actuator_test->time_boot_ms = mavlink_msg_smart_car_actuator_test_get_time_boot_ms(msg);
    smart_car_actuator_test->servo_angle_deg = mavlink_msg_smart_car_actuator_test_get_servo_angle_deg(msg);
    smart_car_actuator_test->test_mask = mavlink_msg_smart_car_actuator_test_get_test_mask(msg);
    smart_car_actuator_test->servo_pwm_us = mavlink_msg_smart_car_actuator_test_get_servo_pwm_us(msg);
    smart_car_actuator_test->duration_ms = mavlink_msg_smart_car_actuator_test_get_duration_ms(msg);
    smart_car_actuator_test->motor1_rpm = mavlink_msg_smart_car_actuator_test_get_motor1_rpm(msg);
    smart_car_actuator_test->motor2_rpm = mavlink_msg_smart_car_actuator_test_get_motor2_rpm(msg);
    smart_car_actuator_test->target_system = mavlink_msg_smart_car_actuator_test_get_target_system(msg);
    smart_car_actuator_test->target_component = mavlink_msg_smart_car_actuator_test_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN;
        memset(smart_car_actuator_test, 0, MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_LEN);
    memcpy(smart_car_actuator_test, _MAV_PAYLOAD(msg), len);
#endif
}
