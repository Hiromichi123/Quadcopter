/** @file
 *    @brief MAVLink comm protocol testsuite generated from smart_car.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef SMART_CAR_TESTSUITE_H
#define SMART_CAR_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_smart_car(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_smart_car(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_smart_car_control_setpoint(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_control_setpoint_t packet_in = {
        963497464,45.0,73.0,101.0,129.0,18275,18379,77,144,211
    };
    mavlink_smart_car_control_setpoint_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.target_speed_mps = packet_in.target_speed_mps;
        packet1.target_curvature = packet_in.target_curvature;
        packet1.target_yaw_rate_dps = packet_in.target_yaw_rate_dps;
        packet1.target_accel_mps2 = packet_in.target_accel_mps2;
        packet1.seq = packet_in.seq;
        packet1.flags = packet_in.flags;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.mode = packet_in.mode;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_control_setpoint_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_control_setpoint_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_control_setpoint_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.target_speed_mps , packet1.target_curvature , packet1.target_yaw_rate_dps , packet1.target_accel_mps2 , packet1.seq , packet1.flags , packet1.target_system , packet1.target_component , packet1.mode );
    mavlink_msg_smart_car_control_setpoint_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_control_setpoint_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.target_speed_mps , packet1.target_curvature , packet1.target_yaw_rate_dps , packet1.target_accel_mps2 , packet1.seq , packet1.flags , packet1.target_system , packet1.target_component , packet1.mode );
    mavlink_msg_smart_car_control_setpoint_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_control_setpoint_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_control_setpoint_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.target_speed_mps , packet1.target_curvature , packet1.target_yaw_rate_dps , packet1.target_accel_mps2 , packet1.seq , packet1.flags , packet1.target_system , packet1.target_component , packet1.mode );
    mavlink_msg_smart_car_control_setpoint_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_CONTROL_SETPOINT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_CONTROL_SETPOINT) != NULL);
#endif
}

static void mavlink_test_smart_car_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_status_t packet_in = {
        963497464,963497672,963497880,17859,17963,53,120,187,254,65,132,199
    };
    mavlink_smart_car_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.fault_flags = packet_in.fault_flags;
        packet1.warn_flags = packet_in.warn_flags;
        packet1.cmd_age_ms = packet_in.cmd_age_ms;
        packet1.control_loop_hz = packet_in.control_loop_hz;
        packet1.mode = packet_in.mode;
        packet1.state = packet_in.state;
        packet1.imu_online = packet_in.imu_online;
        packet1.can_online = packet_in.can_online;
        packet1.host_online = packet_in.host_online;
        packet1.servo_online = packet_in.servo_online;
        packet1.motor_online_mask = packet_in.motor_online_mask;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.fault_flags , packet1.warn_flags , packet1.cmd_age_ms , packet1.control_loop_hz , packet1.mode , packet1.state , packet1.imu_online , packet1.can_online , packet1.host_online , packet1.servo_online , packet1.motor_online_mask );
    mavlink_msg_smart_car_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.fault_flags , packet1.warn_flags , packet1.cmd_age_ms , packet1.control_loop_hz , packet1.mode , packet1.state , packet1.imu_online , packet1.can_online , packet1.host_online , packet1.servo_online , packet1.motor_online_mask );
    mavlink_msg_smart_car_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.fault_flags , packet1.warn_flags , packet1.cmd_age_ms , packet1.control_loop_hz , packet1.mode , packet1.state , packet1.imu_online , packet1.can_online , packet1.host_online , packet1.servo_online , packet1.motor_online_mask );
    mavlink_msg_smart_car_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_STATUS) != NULL);
#endif
}

static void mavlink_test_smart_car_motion_state(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_motion_state_t packet_in = {
        963497464,45.0,73.0,101.0,129.0,157.0,185.0,213.0,18899,235
    };
    mavlink_smart_car_motion_state_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.speed_mps = packet_in.speed_mps;
        packet1.target_speed_mps = packet_in.target_speed_mps;
        packet1.yaw_rate_dps = packet_in.yaw_rate_dps;
        packet1.yaw_deg = packet_in.yaw_deg;
        packet1.curvature_meas = packet_in.curvature_meas;
        packet1.curvature_cmd = packet_in.curvature_cmd;
        packet1.steering_angle_deg = packet_in.steering_angle_deg;
        packet1.steering_pwm_us = packet_in.steering_pwm_us;
        packet1.steering_clamped = packet_in.steering_clamped;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motion_state_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_motion_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motion_state_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.speed_mps , packet1.target_speed_mps , packet1.yaw_rate_dps , packet1.yaw_deg , packet1.curvature_meas , packet1.curvature_cmd , packet1.steering_angle_deg , packet1.steering_pwm_us , packet1.steering_clamped );
    mavlink_msg_smart_car_motion_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motion_state_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.speed_mps , packet1.target_speed_mps , packet1.yaw_rate_dps , packet1.yaw_deg , packet1.curvature_meas , packet1.curvature_cmd , packet1.steering_angle_deg , packet1.steering_pwm_us , packet1.steering_clamped );
    mavlink_msg_smart_car_motion_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_motion_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motion_state_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.speed_mps , packet1.target_speed_mps , packet1.yaw_rate_dps , packet1.yaw_deg , packet1.curvature_meas , packet1.curvature_cmd , packet1.steering_angle_deg , packet1.steering_pwm_us , packet1.steering_clamped );
    mavlink_msg_smart_car_motion_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_MOTION_STATE") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_MOTION_STATE) != NULL);
#endif
}

static void mavlink_test_smart_car_motor_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_motor_status_t packet_in = {
        963497464,963497672,963497880,17859,17963,18067,18171,18275,18379,18483,18587,18691,18795,101
    };
    mavlink_smart_car_motor_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.can_tx_busy_count = packet_in.can_tx_busy_count;
        packet1.can_error_count = packet_in.can_error_count;
        packet1.target_rpm_1 = packet_in.target_rpm_1;
        packet1.target_rpm_2 = packet_in.target_rpm_2;
        packet1.actual_rpm_1 = packet_in.actual_rpm_1;
        packet1.actual_rpm_2 = packet_in.actual_rpm_2;
        packet1.current_cmd_1 = packet_in.current_cmd_1;
        packet1.current_cmd_2 = packet_in.current_cmd_2;
        packet1.feedback_current_1 = packet_in.feedback_current_1;
        packet1.feedback_current_2 = packet_in.feedback_current_2;
        packet1.angle_1 = packet_in.angle_1;
        packet1.angle_2 = packet_in.angle_2;
        packet1.online_mask = packet_in.online_mask;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motor_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_motor_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motor_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.can_tx_busy_count , packet1.can_error_count , packet1.target_rpm_1 , packet1.target_rpm_2 , packet1.actual_rpm_1 , packet1.actual_rpm_2 , packet1.current_cmd_1 , packet1.current_cmd_2 , packet1.feedback_current_1 , packet1.feedback_current_2 , packet1.angle_1 , packet1.angle_2 , packet1.online_mask );
    mavlink_msg_smart_car_motor_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motor_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.can_tx_busy_count , packet1.can_error_count , packet1.target_rpm_1 , packet1.target_rpm_2 , packet1.actual_rpm_1 , packet1.actual_rpm_2 , packet1.current_cmd_1 , packet1.current_cmd_2 , packet1.feedback_current_1 , packet1.feedback_current_2 , packet1.angle_1 , packet1.angle_2 , packet1.online_mask );
    mavlink_msg_smart_car_motor_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_motor_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_motor_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.can_tx_busy_count , packet1.can_error_count , packet1.target_rpm_1 , packet1.target_rpm_2 , packet1.actual_rpm_1 , packet1.actual_rpm_2 , packet1.current_cmd_1 , packet1.current_cmd_2 , packet1.feedback_current_1 , packet1.feedback_current_2 , packet1.angle_1 , packet1.angle_2 , packet1.online_mask );
    mavlink_msg_smart_car_motor_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_MOTOR_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_MOTOR_STATUS) != NULL);
#endif
}

static void mavlink_test_smart_car_imu_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_imu_status_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,963498712,213.0,241.0,269.0,19315,19419,19523,19627,149
    };
    mavlink_smart_car_imu_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.sample_count = packet_in.sample_count;
        packet1.overrun_count = packet_in.overrun_count;
        packet1.error_count = packet_in.error_count;
        packet1.gyro_x_mdps = packet_in.gyro_x_mdps;
        packet1.gyro_y_mdps = packet_in.gyro_y_mdps;
        packet1.gyro_z_mdps = packet_in.gyro_z_mdps;
        packet1.yaw_rate_raw_dps = packet_in.yaw_rate_raw_dps;
        packet1.yaw_rate_dps = packet_in.yaw_rate_dps;
        packet1.gyro_bias_z_dps = packet_in.gyro_bias_z_dps;
        packet1.accel_x_mg = packet_in.accel_x_mg;
        packet1.accel_y_mg = packet_in.accel_y_mg;
        packet1.accel_z_mg = packet_in.accel_z_mg;
        packet1.temperature_c_x100 = packet_in.temperature_c_x100;
        packet1.calibrated = packet_in.calibrated;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_imu_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_imu_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_imu_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.sample_count , packet1.overrun_count , packet1.error_count , packet1.gyro_x_mdps , packet1.gyro_y_mdps , packet1.gyro_z_mdps , packet1.yaw_rate_raw_dps , packet1.yaw_rate_dps , packet1.gyro_bias_z_dps , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.temperature_c_x100 , packet1.calibrated );
    mavlink_msg_smart_car_imu_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_imu_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.sample_count , packet1.overrun_count , packet1.error_count , packet1.gyro_x_mdps , packet1.gyro_y_mdps , packet1.gyro_z_mdps , packet1.yaw_rate_raw_dps , packet1.yaw_rate_dps , packet1.gyro_bias_z_dps , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.temperature_c_x100 , packet1.calibrated );
    mavlink_msg_smart_car_imu_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_imu_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_imu_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.sample_count , packet1.overrun_count , packet1.error_count , packet1.gyro_x_mdps , packet1.gyro_y_mdps , packet1.gyro_z_mdps , packet1.yaw_rate_raw_dps , packet1.yaw_rate_dps , packet1.gyro_bias_z_dps , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.temperature_c_x100 , packet1.calibrated );
    mavlink_msg_smart_car_imu_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_IMU_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_IMU_STATUS) != NULL);
#endif
}

static void mavlink_test_smart_car_actuator_test(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_actuator_test_t packet_in = {
        963497464,45.0,17651,17755,17859,17963,18067,187,254
    };
    mavlink_smart_car_actuator_test_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.servo_angle_deg = packet_in.servo_angle_deg;
        packet1.test_mask = packet_in.test_mask;
        packet1.servo_pwm_us = packet_in.servo_pwm_us;
        packet1.duration_ms = packet_in.duration_ms;
        packet1.motor1_rpm = packet_in.motor1_rpm;
        packet1.motor2_rpm = packet_in.motor2_rpm;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_actuator_test_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_actuator_test_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_actuator_test_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.servo_angle_deg , packet1.test_mask , packet1.servo_pwm_us , packet1.duration_ms , packet1.motor1_rpm , packet1.motor2_rpm , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_actuator_test_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_actuator_test_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.servo_angle_deg , packet1.test_mask , packet1.servo_pwm_us , packet1.duration_ms , packet1.motor1_rpm , packet1.motor2_rpm , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_actuator_test_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_actuator_test_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_actuator_test_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.servo_angle_deg , packet1.test_mask , packet1.servo_pwm_us , packet1.duration_ms , packet1.motor1_rpm , packet1.motor2_rpm , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_actuator_test_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_ACTUATOR_TEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_ACTUATOR_TEST) != NULL);
#endif
}

static void mavlink_test_smart_car_calib_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_calib_status_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,185.0,213.0,241.0,269.0,19315,19419,137,204,15
    };
    mavlink_smart_car_calib_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.point_id = packet_in.point_id;
        packet1.sweep_index = packet_in.sweep_index;
        packet1.sweep_count = packet_in.sweep_count;
        packet1.valid_count = packet_in.valid_count;
        packet1.invalid_count = packet_in.invalid_count;
        packet1.v_center_avg = packet_in.v_center_avg;
        packet1.yaw_rate_avg = packet_in.yaw_rate_avg;
        packet1.kappa_avg = packet_in.kappa_avg;
        packet1.radius_est = packet_in.radius_est;
        packet1.target_rpm = packet_in.target_rpm;
        packet1.servo_pwm_us = packet_in.servo_pwm_us;
        packet1.state = packet_in.state;
        packet1.sweep_enabled = packet_in.sweep_enabled;
        packet1.yaw_sign_inverted = packet_in.yaw_sign_inverted;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_calib_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_calib_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_calib_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.point_id , packet1.sweep_index , packet1.sweep_count , packet1.valid_count , packet1.invalid_count , packet1.v_center_avg , packet1.yaw_rate_avg , packet1.kappa_avg , packet1.radius_est , packet1.target_rpm , packet1.servo_pwm_us , packet1.state , packet1.sweep_enabled , packet1.yaw_sign_inverted );
    mavlink_msg_smart_car_calib_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_calib_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.point_id , packet1.sweep_index , packet1.sweep_count , packet1.valid_count , packet1.invalid_count , packet1.v_center_avg , packet1.yaw_rate_avg , packet1.kappa_avg , packet1.radius_est , packet1.target_rpm , packet1.servo_pwm_us , packet1.state , packet1.sweep_enabled , packet1.yaw_sign_inverted );
    mavlink_msg_smart_car_calib_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_calib_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_calib_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.point_id , packet1.sweep_index , packet1.sweep_count , packet1.valid_count , packet1.invalid_count , packet1.v_center_avg , packet1.yaw_rate_avg , packet1.kappa_avg , packet1.radius_est , packet1.target_rpm , packet1.servo_pwm_us , packet1.state , packet1.sweep_enabled , packet1.yaw_sign_inverted );
    mavlink_msg_smart_car_calib_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_CALIB_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_CALIB_STATUS) != NULL);
#endif
}

static void mavlink_test_smart_car_command(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SMART_CAR_COMMAND >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_smart_car_command_t packet_in = {
        963497464,45.0,73.0,101.0,129.0,18275,199,10
    };
    mavlink_smart_car_command_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.param1 = packet_in.param1;
        packet1.param2 = packet_in.param2;
        packet1.param3 = packet_in.param3;
        packet1.param4 = packet_in.param4;
        packet1.command = packet_in.command;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SMART_CAR_COMMAND_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_command_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_smart_car_command_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_command_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.command , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_command_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_command_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.command , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_command_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_smart_car_command_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_smart_car_command_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.param1 , packet1.param2 , packet1.param3 , packet1.param4 , packet1.command , packet1.target_system , packet1.target_component );
    mavlink_msg_smart_car_command_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SMART_CAR_COMMAND") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SMART_CAR_COMMAND) != NULL);
#endif
}

static void mavlink_test_smart_car(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_smart_car_control_setpoint(system_id, component_id, last_msg);
    mavlink_test_smart_car_status(system_id, component_id, last_msg);
    mavlink_test_smart_car_motion_state(system_id, component_id, last_msg);
    mavlink_test_smart_car_motor_status(system_id, component_id, last_msg);
    mavlink_test_smart_car_imu_status(system_id, component_id, last_msg);
    mavlink_test_smart_car_actuator_test(system_id, component_id, last_msg);
    mavlink_test_smart_car_calib_status(system_id, component_id, last_msg);
    mavlink_test_smart_car_command(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SMART_CAR_TESTSUITE_H
