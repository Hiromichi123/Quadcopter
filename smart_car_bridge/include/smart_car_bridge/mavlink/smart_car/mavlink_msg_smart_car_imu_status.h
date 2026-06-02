#pragma once
// MESSAGE SMART_CAR_IMU_STATUS PACKING

#define MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS 42004


typedef struct __mavlink_smart_car_imu_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp from the vehicle.*/
 uint32_t sample_count; /*<  IMU sample count.*/
 uint32_t overrun_count; /*<  IMU overrun count.*/
 uint32_t error_count; /*<  IMU error count.*/
 int32_t gyro_x_mdps; /*< [mdeg/s] Raw gyro X.*/
 int32_t gyro_y_mdps; /*< [mdeg/s] Raw gyro Y.*/
 int32_t gyro_z_mdps; /*< [mdeg/s] Raw gyro Z.*/
 float yaw_rate_raw_dps; /*< [deg/s] Raw yaw rate.*/
 float yaw_rate_dps; /*< [deg/s] Filtered yaw rate.*/
 float gyro_bias_z_dps; /*< [deg/s] Yaw gyro bias.*/
 int16_t accel_x_mg; /*< [mG] Accel X.*/
 int16_t accel_y_mg; /*< [mG] Accel Y.*/
 int16_t accel_z_mg; /*< [mG] Accel Z.*/
 int16_t temperature_c_x100; /*< [cdegC] IMU temperature.*/
 uint8_t calibrated; /*<  Gyro calibrated flag.*/
} mavlink_smart_car_imu_status_t;

#define MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN 49
#define MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN 49
#define MAVLINK_MSG_ID_42004_LEN 49
#define MAVLINK_MSG_ID_42004_MIN_LEN 49

#define MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC 104
#define MAVLINK_MSG_ID_42004_CRC 104



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SMART_CAR_IMU_STATUS { \
    42004, \
    "SMART_CAR_IMU_STATUS", \
    15, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_imu_status_t, time_boot_ms) }, \
         { "sample_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_imu_status_t, sample_count) }, \
         { "overrun_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_imu_status_t, overrun_count) }, \
         { "error_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_smart_car_imu_status_t, error_count) }, \
         { "gyro_x_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_smart_car_imu_status_t, gyro_x_mdps) }, \
         { "gyro_y_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_smart_car_imu_status_t, gyro_y_mdps) }, \
         { "gyro_z_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_smart_car_imu_status_t, gyro_z_mdps) }, \
         { "yaw_rate_raw_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_imu_status_t, yaw_rate_raw_dps) }, \
         { "yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_smart_car_imu_status_t, yaw_rate_dps) }, \
         { "gyro_bias_z_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_smart_car_imu_status_t, gyro_bias_z_dps) }, \
         { "accel_x_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_smart_car_imu_status_t, accel_x_mg) }, \
         { "accel_y_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 42, offsetof(mavlink_smart_car_imu_status_t, accel_y_mg) }, \
         { "accel_z_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 44, offsetof(mavlink_smart_car_imu_status_t, accel_z_mg) }, \
         { "temperature_c_x100", NULL, MAVLINK_TYPE_INT16_T, 0, 46, offsetof(mavlink_smart_car_imu_status_t, temperature_c_x100) }, \
         { "calibrated", NULL, MAVLINK_TYPE_UINT8_T, 0, 48, offsetof(mavlink_smart_car_imu_status_t, calibrated) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SMART_CAR_IMU_STATUS { \
    "SMART_CAR_IMU_STATUS", \
    15, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_smart_car_imu_status_t, time_boot_ms) }, \
         { "sample_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_smart_car_imu_status_t, sample_count) }, \
         { "overrun_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_smart_car_imu_status_t, overrun_count) }, \
         { "error_count", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_smart_car_imu_status_t, error_count) }, \
         { "gyro_x_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_smart_car_imu_status_t, gyro_x_mdps) }, \
         { "gyro_y_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_smart_car_imu_status_t, gyro_y_mdps) }, \
         { "gyro_z_mdps", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_smart_car_imu_status_t, gyro_z_mdps) }, \
         { "yaw_rate_raw_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_smart_car_imu_status_t, yaw_rate_raw_dps) }, \
         { "yaw_rate_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_smart_car_imu_status_t, yaw_rate_dps) }, \
         { "gyro_bias_z_dps", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_smart_car_imu_status_t, gyro_bias_z_dps) }, \
         { "accel_x_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_smart_car_imu_status_t, accel_x_mg) }, \
         { "accel_y_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 42, offsetof(mavlink_smart_car_imu_status_t, accel_y_mg) }, \
         { "accel_z_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 44, offsetof(mavlink_smart_car_imu_status_t, accel_z_mg) }, \
         { "temperature_c_x100", NULL, MAVLINK_TYPE_INT16_T, 0, 46, offsetof(mavlink_smart_car_imu_status_t, temperature_c_x100) }, \
         { "calibrated", NULL, MAVLINK_TYPE_UINT8_T, 0, 48, offsetof(mavlink_smart_car_imu_status_t, calibrated) }, \
         } \
}
#endif

/**
 * @brief Pack a smart_car_imu_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param sample_count  IMU sample count.
 * @param overrun_count  IMU overrun count.
 * @param error_count  IMU error count.
 * @param gyro_x_mdps [mdeg/s] Raw gyro X.
 * @param gyro_y_mdps [mdeg/s] Raw gyro Y.
 * @param gyro_z_mdps [mdeg/s] Raw gyro Z.
 * @param yaw_rate_raw_dps [deg/s] Raw yaw rate.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param gyro_bias_z_dps [deg/s] Yaw gyro bias.
 * @param accel_x_mg [mG] Accel X.
 * @param accel_y_mg [mG] Accel Y.
 * @param accel_z_mg [mG] Accel Z.
 * @param temperature_c_x100 [cdegC] IMU temperature.
 * @param calibrated  Gyro calibrated flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t sample_count, uint32_t overrun_count, uint32_t error_count, int32_t gyro_x_mdps, int32_t gyro_y_mdps, int32_t gyro_z_mdps, float yaw_rate_raw_dps, float yaw_rate_dps, float gyro_bias_z_dps, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t temperature_c_x100, uint8_t calibrated)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, sample_count);
    _mav_put_uint32_t(buf, 8, overrun_count);
    _mav_put_uint32_t(buf, 12, error_count);
    _mav_put_int32_t(buf, 16, gyro_x_mdps);
    _mav_put_int32_t(buf, 20, gyro_y_mdps);
    _mav_put_int32_t(buf, 24, gyro_z_mdps);
    _mav_put_float(buf, 28, yaw_rate_raw_dps);
    _mav_put_float(buf, 32, yaw_rate_dps);
    _mav_put_float(buf, 36, gyro_bias_z_dps);
    _mav_put_int16_t(buf, 40, accel_x_mg);
    _mav_put_int16_t(buf, 42, accel_y_mg);
    _mav_put_int16_t(buf, 44, accel_z_mg);
    _mav_put_int16_t(buf, 46, temperature_c_x100);
    _mav_put_uint8_t(buf, 48, calibrated);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#else
    mavlink_smart_car_imu_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.sample_count = sample_count;
    packet.overrun_count = overrun_count;
    packet.error_count = error_count;
    packet.gyro_x_mdps = gyro_x_mdps;
    packet.gyro_y_mdps = gyro_y_mdps;
    packet.gyro_z_mdps = gyro_z_mdps;
    packet.yaw_rate_raw_dps = yaw_rate_raw_dps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.gyro_bias_z_dps = gyro_bias_z_dps;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.temperature_c_x100 = temperature_c_x100;
    packet.calibrated = calibrated;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
}

/**
 * @brief Pack a smart_car_imu_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param sample_count  IMU sample count.
 * @param overrun_count  IMU overrun count.
 * @param error_count  IMU error count.
 * @param gyro_x_mdps [mdeg/s] Raw gyro X.
 * @param gyro_y_mdps [mdeg/s] Raw gyro Y.
 * @param gyro_z_mdps [mdeg/s] Raw gyro Z.
 * @param yaw_rate_raw_dps [deg/s] Raw yaw rate.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param gyro_bias_z_dps [deg/s] Yaw gyro bias.
 * @param accel_x_mg [mG] Accel X.
 * @param accel_y_mg [mG] Accel Y.
 * @param accel_z_mg [mG] Accel Z.
 * @param temperature_c_x100 [cdegC] IMU temperature.
 * @param calibrated  Gyro calibrated flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t sample_count, uint32_t overrun_count, uint32_t error_count, int32_t gyro_x_mdps, int32_t gyro_y_mdps, int32_t gyro_z_mdps, float yaw_rate_raw_dps, float yaw_rate_dps, float gyro_bias_z_dps, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t temperature_c_x100, uint8_t calibrated)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, sample_count);
    _mav_put_uint32_t(buf, 8, overrun_count);
    _mav_put_uint32_t(buf, 12, error_count);
    _mav_put_int32_t(buf, 16, gyro_x_mdps);
    _mav_put_int32_t(buf, 20, gyro_y_mdps);
    _mav_put_int32_t(buf, 24, gyro_z_mdps);
    _mav_put_float(buf, 28, yaw_rate_raw_dps);
    _mav_put_float(buf, 32, yaw_rate_dps);
    _mav_put_float(buf, 36, gyro_bias_z_dps);
    _mav_put_int16_t(buf, 40, accel_x_mg);
    _mav_put_int16_t(buf, 42, accel_y_mg);
    _mav_put_int16_t(buf, 44, accel_z_mg);
    _mav_put_int16_t(buf, 46, temperature_c_x100);
    _mav_put_uint8_t(buf, 48, calibrated);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#else
    mavlink_smart_car_imu_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.sample_count = sample_count;
    packet.overrun_count = overrun_count;
    packet.error_count = error_count;
    packet.gyro_x_mdps = gyro_x_mdps;
    packet.gyro_y_mdps = gyro_y_mdps;
    packet.gyro_z_mdps = gyro_z_mdps;
    packet.yaw_rate_raw_dps = yaw_rate_raw_dps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.gyro_bias_z_dps = gyro_bias_z_dps;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.temperature_c_x100 = temperature_c_x100;
    packet.calibrated = calibrated;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#endif
}

/**
 * @brief Pack a smart_car_imu_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param sample_count  IMU sample count.
 * @param overrun_count  IMU overrun count.
 * @param error_count  IMU error count.
 * @param gyro_x_mdps [mdeg/s] Raw gyro X.
 * @param gyro_y_mdps [mdeg/s] Raw gyro Y.
 * @param gyro_z_mdps [mdeg/s] Raw gyro Z.
 * @param yaw_rate_raw_dps [deg/s] Raw yaw rate.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param gyro_bias_z_dps [deg/s] Yaw gyro bias.
 * @param accel_x_mg [mG] Accel X.
 * @param accel_y_mg [mG] Accel Y.
 * @param accel_z_mg [mG] Accel Z.
 * @param temperature_c_x100 [cdegC] IMU temperature.
 * @param calibrated  Gyro calibrated flag.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint32_t sample_count,uint32_t overrun_count,uint32_t error_count,int32_t gyro_x_mdps,int32_t gyro_y_mdps,int32_t gyro_z_mdps,float yaw_rate_raw_dps,float yaw_rate_dps,float gyro_bias_z_dps,int16_t accel_x_mg,int16_t accel_y_mg,int16_t accel_z_mg,int16_t temperature_c_x100,uint8_t calibrated)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, sample_count);
    _mav_put_uint32_t(buf, 8, overrun_count);
    _mav_put_uint32_t(buf, 12, error_count);
    _mav_put_int32_t(buf, 16, gyro_x_mdps);
    _mav_put_int32_t(buf, 20, gyro_y_mdps);
    _mav_put_int32_t(buf, 24, gyro_z_mdps);
    _mav_put_float(buf, 28, yaw_rate_raw_dps);
    _mav_put_float(buf, 32, yaw_rate_dps);
    _mav_put_float(buf, 36, gyro_bias_z_dps);
    _mav_put_int16_t(buf, 40, accel_x_mg);
    _mav_put_int16_t(buf, 42, accel_y_mg);
    _mav_put_int16_t(buf, 44, accel_z_mg);
    _mav_put_int16_t(buf, 46, temperature_c_x100);
    _mav_put_uint8_t(buf, 48, calibrated);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#else
    mavlink_smart_car_imu_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.sample_count = sample_count;
    packet.overrun_count = overrun_count;
    packet.error_count = error_count;
    packet.gyro_x_mdps = gyro_x_mdps;
    packet.gyro_y_mdps = gyro_y_mdps;
    packet.gyro_z_mdps = gyro_z_mdps;
    packet.yaw_rate_raw_dps = yaw_rate_raw_dps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.gyro_bias_z_dps = gyro_bias_z_dps;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.temperature_c_x100 = temperature_c_x100;
    packet.calibrated = calibrated;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
}

/**
 * @brief Encode a smart_car_imu_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_imu_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_smart_car_imu_status_t* smart_car_imu_status)
{
    return mavlink_msg_smart_car_imu_status_pack(system_id, component_id, msg, smart_car_imu_status->time_boot_ms, smart_car_imu_status->sample_count, smart_car_imu_status->overrun_count, smart_car_imu_status->error_count, smart_car_imu_status->gyro_x_mdps, smart_car_imu_status->gyro_y_mdps, smart_car_imu_status->gyro_z_mdps, smart_car_imu_status->yaw_rate_raw_dps, smart_car_imu_status->yaw_rate_dps, smart_car_imu_status->gyro_bias_z_dps, smart_car_imu_status->accel_x_mg, smart_car_imu_status->accel_y_mg, smart_car_imu_status->accel_z_mg, smart_car_imu_status->temperature_c_x100, smart_car_imu_status->calibrated);
}

/**
 * @brief Encode a smart_car_imu_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_imu_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_smart_car_imu_status_t* smart_car_imu_status)
{
    return mavlink_msg_smart_car_imu_status_pack_chan(system_id, component_id, chan, msg, smart_car_imu_status->time_boot_ms, smart_car_imu_status->sample_count, smart_car_imu_status->overrun_count, smart_car_imu_status->error_count, smart_car_imu_status->gyro_x_mdps, smart_car_imu_status->gyro_y_mdps, smart_car_imu_status->gyro_z_mdps, smart_car_imu_status->yaw_rate_raw_dps, smart_car_imu_status->yaw_rate_dps, smart_car_imu_status->gyro_bias_z_dps, smart_car_imu_status->accel_x_mg, smart_car_imu_status->accel_y_mg, smart_car_imu_status->accel_z_mg, smart_car_imu_status->temperature_c_x100, smart_car_imu_status->calibrated);
}

/**
 * @brief Encode a smart_car_imu_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param smart_car_imu_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_smart_car_imu_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_smart_car_imu_status_t* smart_car_imu_status)
{
    return mavlink_msg_smart_car_imu_status_pack_status(system_id, component_id, _status, msg,  smart_car_imu_status->time_boot_ms, smart_car_imu_status->sample_count, smart_car_imu_status->overrun_count, smart_car_imu_status->error_count, smart_car_imu_status->gyro_x_mdps, smart_car_imu_status->gyro_y_mdps, smart_car_imu_status->gyro_z_mdps, smart_car_imu_status->yaw_rate_raw_dps, smart_car_imu_status->yaw_rate_dps, smart_car_imu_status->gyro_bias_z_dps, smart_car_imu_status->accel_x_mg, smart_car_imu_status->accel_y_mg, smart_car_imu_status->accel_z_mg, smart_car_imu_status->temperature_c_x100, smart_car_imu_status->calibrated);
}

/**
 * @brief Send a smart_car_imu_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp from the vehicle.
 * @param sample_count  IMU sample count.
 * @param overrun_count  IMU overrun count.
 * @param error_count  IMU error count.
 * @param gyro_x_mdps [mdeg/s] Raw gyro X.
 * @param gyro_y_mdps [mdeg/s] Raw gyro Y.
 * @param gyro_z_mdps [mdeg/s] Raw gyro Z.
 * @param yaw_rate_raw_dps [deg/s] Raw yaw rate.
 * @param yaw_rate_dps [deg/s] Filtered yaw rate.
 * @param gyro_bias_z_dps [deg/s] Yaw gyro bias.
 * @param accel_x_mg [mG] Accel X.
 * @param accel_y_mg [mG] Accel Y.
 * @param accel_z_mg [mG] Accel Z.
 * @param temperature_c_x100 [cdegC] IMU temperature.
 * @param calibrated  Gyro calibrated flag.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_smart_car_imu_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint32_t sample_count, uint32_t overrun_count, uint32_t error_count, int32_t gyro_x_mdps, int32_t gyro_y_mdps, int32_t gyro_z_mdps, float yaw_rate_raw_dps, float yaw_rate_dps, float gyro_bias_z_dps, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t temperature_c_x100, uint8_t calibrated)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, sample_count);
    _mav_put_uint32_t(buf, 8, overrun_count);
    _mav_put_uint32_t(buf, 12, error_count);
    _mav_put_int32_t(buf, 16, gyro_x_mdps);
    _mav_put_int32_t(buf, 20, gyro_y_mdps);
    _mav_put_int32_t(buf, 24, gyro_z_mdps);
    _mav_put_float(buf, 28, yaw_rate_raw_dps);
    _mav_put_float(buf, 32, yaw_rate_dps);
    _mav_put_float(buf, 36, gyro_bias_z_dps);
    _mav_put_int16_t(buf, 40, accel_x_mg);
    _mav_put_int16_t(buf, 42, accel_y_mg);
    _mav_put_int16_t(buf, 44, accel_z_mg);
    _mav_put_int16_t(buf, 46, temperature_c_x100);
    _mav_put_uint8_t(buf, 48, calibrated);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#else
    mavlink_smart_car_imu_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.sample_count = sample_count;
    packet.overrun_count = overrun_count;
    packet.error_count = error_count;
    packet.gyro_x_mdps = gyro_x_mdps;
    packet.gyro_y_mdps = gyro_y_mdps;
    packet.gyro_z_mdps = gyro_z_mdps;
    packet.yaw_rate_raw_dps = yaw_rate_raw_dps;
    packet.yaw_rate_dps = yaw_rate_dps;
    packet.gyro_bias_z_dps = gyro_bias_z_dps;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.temperature_c_x100 = temperature_c_x100;
    packet.calibrated = calibrated;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS, (const char *)&packet, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#endif
}

/**
 * @brief Send a smart_car_imu_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_smart_car_imu_status_send_struct(mavlink_channel_t chan, const mavlink_smart_car_imu_status_t* smart_car_imu_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_smart_car_imu_status_send(chan, smart_car_imu_status->time_boot_ms, smart_car_imu_status->sample_count, smart_car_imu_status->overrun_count, smart_car_imu_status->error_count, smart_car_imu_status->gyro_x_mdps, smart_car_imu_status->gyro_y_mdps, smart_car_imu_status->gyro_z_mdps, smart_car_imu_status->yaw_rate_raw_dps, smart_car_imu_status->yaw_rate_dps, smart_car_imu_status->gyro_bias_z_dps, smart_car_imu_status->accel_x_mg, smart_car_imu_status->accel_y_mg, smart_car_imu_status->accel_z_mg, smart_car_imu_status->temperature_c_x100, smart_car_imu_status->calibrated);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS, (const char *)smart_car_imu_status, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_smart_car_imu_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint32_t sample_count, uint32_t overrun_count, uint32_t error_count, int32_t gyro_x_mdps, int32_t gyro_y_mdps, int32_t gyro_z_mdps, float yaw_rate_raw_dps, float yaw_rate_dps, float gyro_bias_z_dps, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t temperature_c_x100, uint8_t calibrated)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, sample_count);
    _mav_put_uint32_t(buf, 8, overrun_count);
    _mav_put_uint32_t(buf, 12, error_count);
    _mav_put_int32_t(buf, 16, gyro_x_mdps);
    _mav_put_int32_t(buf, 20, gyro_y_mdps);
    _mav_put_int32_t(buf, 24, gyro_z_mdps);
    _mav_put_float(buf, 28, yaw_rate_raw_dps);
    _mav_put_float(buf, 32, yaw_rate_dps);
    _mav_put_float(buf, 36, gyro_bias_z_dps);
    _mav_put_int16_t(buf, 40, accel_x_mg);
    _mav_put_int16_t(buf, 42, accel_y_mg);
    _mav_put_int16_t(buf, 44, accel_z_mg);
    _mav_put_int16_t(buf, 46, temperature_c_x100);
    _mav_put_uint8_t(buf, 48, calibrated);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS, buf, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#else
    mavlink_smart_car_imu_status_t *packet = (mavlink_smart_car_imu_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->sample_count = sample_count;
    packet->overrun_count = overrun_count;
    packet->error_count = error_count;
    packet->gyro_x_mdps = gyro_x_mdps;
    packet->gyro_y_mdps = gyro_y_mdps;
    packet->gyro_z_mdps = gyro_z_mdps;
    packet->yaw_rate_raw_dps = yaw_rate_raw_dps;
    packet->yaw_rate_dps = yaw_rate_dps;
    packet->gyro_bias_z_dps = gyro_bias_z_dps;
    packet->accel_x_mg = accel_x_mg;
    packet->accel_y_mg = accel_y_mg;
    packet->accel_z_mg = accel_z_mg;
    packet->temperature_c_x100 = temperature_c_x100;
    packet->calibrated = calibrated;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS, (const char *)packet, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE SMART_CAR_IMU_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from smart_car_imu_status message
 *
 * @return [ms] Timestamp from the vehicle.
 */
static inline uint32_t mavlink_msg_smart_car_imu_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field sample_count from smart_car_imu_status message
 *
 * @return  IMU sample count.
 */
static inline uint32_t mavlink_msg_smart_car_imu_status_get_sample_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field overrun_count from smart_car_imu_status message
 *
 * @return  IMU overrun count.
 */
static inline uint32_t mavlink_msg_smart_car_imu_status_get_overrun_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field error_count from smart_car_imu_status message
 *
 * @return  IMU error count.
 */
static inline uint32_t mavlink_msg_smart_car_imu_status_get_error_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  12);
}

/**
 * @brief Get field gyro_x_mdps from smart_car_imu_status message
 *
 * @return [mdeg/s] Raw gyro X.
 */
static inline int32_t mavlink_msg_smart_car_imu_status_get_gyro_x_mdps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field gyro_y_mdps from smart_car_imu_status message
 *
 * @return [mdeg/s] Raw gyro Y.
 */
static inline int32_t mavlink_msg_smart_car_imu_status_get_gyro_y_mdps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Get field gyro_z_mdps from smart_car_imu_status message
 *
 * @return [mdeg/s] Raw gyro Z.
 */
static inline int32_t mavlink_msg_smart_car_imu_status_get_gyro_z_mdps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  24);
}

/**
 * @brief Get field yaw_rate_raw_dps from smart_car_imu_status message
 *
 * @return [deg/s] Raw yaw rate.
 */
static inline float mavlink_msg_smart_car_imu_status_get_yaw_rate_raw_dps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field yaw_rate_dps from smart_car_imu_status message
 *
 * @return [deg/s] Filtered yaw rate.
 */
static inline float mavlink_msg_smart_car_imu_status_get_yaw_rate_dps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field gyro_bias_z_dps from smart_car_imu_status message
 *
 * @return [deg/s] Yaw gyro bias.
 */
static inline float mavlink_msg_smart_car_imu_status_get_gyro_bias_z_dps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field accel_x_mg from smart_car_imu_status message
 *
 * @return [mG] Accel X.
 */
static inline int16_t mavlink_msg_smart_car_imu_status_get_accel_x_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  40);
}

/**
 * @brief Get field accel_y_mg from smart_car_imu_status message
 *
 * @return [mG] Accel Y.
 */
static inline int16_t mavlink_msg_smart_car_imu_status_get_accel_y_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  42);
}

/**
 * @brief Get field accel_z_mg from smart_car_imu_status message
 *
 * @return [mG] Accel Z.
 */
static inline int16_t mavlink_msg_smart_car_imu_status_get_accel_z_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  44);
}

/**
 * @brief Get field temperature_c_x100 from smart_car_imu_status message
 *
 * @return [cdegC] IMU temperature.
 */
static inline int16_t mavlink_msg_smart_car_imu_status_get_temperature_c_x100(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  46);
}

/**
 * @brief Get field calibrated from smart_car_imu_status message
 *
 * @return  Gyro calibrated flag.
 */
static inline uint8_t mavlink_msg_smart_car_imu_status_get_calibrated(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  48);
}

/**
 * @brief Decode a smart_car_imu_status message into a struct
 *
 * @param msg The message to decode
 * @param smart_car_imu_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_smart_car_imu_status_decode(const mavlink_message_t* msg, mavlink_smart_car_imu_status_t* smart_car_imu_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    smart_car_imu_status->time_boot_ms = mavlink_msg_smart_car_imu_status_get_time_boot_ms(msg);
    smart_car_imu_status->sample_count = mavlink_msg_smart_car_imu_status_get_sample_count(msg);
    smart_car_imu_status->overrun_count = mavlink_msg_smart_car_imu_status_get_overrun_count(msg);
    smart_car_imu_status->error_count = mavlink_msg_smart_car_imu_status_get_error_count(msg);
    smart_car_imu_status->gyro_x_mdps = mavlink_msg_smart_car_imu_status_get_gyro_x_mdps(msg);
    smart_car_imu_status->gyro_y_mdps = mavlink_msg_smart_car_imu_status_get_gyro_y_mdps(msg);
    smart_car_imu_status->gyro_z_mdps = mavlink_msg_smart_car_imu_status_get_gyro_z_mdps(msg);
    smart_car_imu_status->yaw_rate_raw_dps = mavlink_msg_smart_car_imu_status_get_yaw_rate_raw_dps(msg);
    smart_car_imu_status->yaw_rate_dps = mavlink_msg_smart_car_imu_status_get_yaw_rate_dps(msg);
    smart_car_imu_status->gyro_bias_z_dps = mavlink_msg_smart_car_imu_status_get_gyro_bias_z_dps(msg);
    smart_car_imu_status->accel_x_mg = mavlink_msg_smart_car_imu_status_get_accel_x_mg(msg);
    smart_car_imu_status->accel_y_mg = mavlink_msg_smart_car_imu_status_get_accel_y_mg(msg);
    smart_car_imu_status->accel_z_mg = mavlink_msg_smart_car_imu_status_get_accel_z_mg(msg);
    smart_car_imu_status->temperature_c_x100 = mavlink_msg_smart_car_imu_status_get_temperature_c_x100(msg);
    smart_car_imu_status->calibrated = mavlink_msg_smart_car_imu_status_get_calibrated(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN? msg->len : MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN;
        memset(smart_car_imu_status, 0, MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_LEN);
    memcpy(smart_car_imu_status, _MAV_PAYLOAD(msg), len);
#endif
}
