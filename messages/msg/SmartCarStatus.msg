uint8 SMART_CAR_MODE_IDLE=0
uint8 SMART_CAR_MODE_MANUAL=1
uint8 SMART_CAR_MODE_AUTO=2
uint8 SMART_CAR_MODE_CALIB=3

uint8 SMART_CAR_STATE_IDLE=0
uint8 SMART_CAR_STATE_ARMED=1
uint8 SMART_CAR_STATE_RUNNING=2
uint8 SMART_CAR_STATE_FAULT=3
uint8 SMART_CAR_STATE_CALIB=4

uint32 SMART_CAR_FAULT_CMD_TIMEOUT=1
uint32 SMART_CAR_FAULT_MOTOR1_OFFLINE=2
uint32 SMART_CAR_FAULT_MOTOR2_OFFLINE=4
uint32 SMART_CAR_FAULT_IMU_NOT_READY=8
uint32 SMART_CAR_FAULT_CAN_ERROR=16
uint32 SMART_CAR_FAULT_SERVO_CLAMPED=32

builtin_interfaces/Time stamp
uint32 time_boot_ms
uint8 mode
uint8 state
uint32 fault_flags
uint32 warn_flags
uint16 cmd_age_ms
uint16 control_loop_hz
uint8 imu_online
uint8 can_online
uint8 host_online
uint8 servo_online
uint8 motor_online_mask
