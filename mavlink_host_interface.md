# Smart Car MAVLink Host Interface

本文档给上位机开发者使用，描述智能竞速小车下位机的 MAVLink 串口通信接口。

## 1. Link

```text
Physical port: P7 UART
Baudrate:      115200
Format:        8N1
Protocol:      MAVLink v2
Signing:       Disabled
Vehicle sysid: 1
Vehicle compid:MAV_COMP_ID_AUTOPILOT1
Host sysid:    255 recommended
```

协议定义文件：

```text
tools/mavlink/smart_car.xml
```

上位机应使用这份 XML 生成或加载 MAVLink dialect，不建议手写消息结构。当前自定义消息 ID 范围为 `42000..42007`。

目标系统规则：

```text
target_system = 1: 控制本车
target_system = 0: 广播，本车接受
其他 target_system: 本车丢弃

target_component = 0: 任意组件，本车接受
target_component = MAV_COMP_ID_AUTOPILOT1: 本车接受
其他 target_component: 本车丢弃
```

## 2. Main Control Message

上位机通过 `SMART_CAR_CONTROL_SETPOINT` 周期发送控制目标。

建议发送频率：

```text
Recommended: 20 Hz or higher
Minimum:     10 Hz
```

字段：

```text
uint32_t time_boot_ms
uint16_t seq
uint8_t  target_system
uint8_t  target_component
uint8_t  mode
uint16_t flags
float    target_speed_mps
float    target_curvature
float    target_yaw_rate_dps
float    target_accel_mps2
```

第一版推荐填写：

```text
target_system       = 1
target_component    = 0
mode                = SMART_CAR_MODE_MANUAL
flags               = SMART_CAR_CONTROL_FLAG_ENABLE
target_speed_mps    = desired speed in m/s
target_curvature    = desired curvature in 1/m
target_yaw_rate_dps = 0
target_accel_mps2   = 0
```

控制 flags：

```text
SMART_CAR_CONTROL_FLAG_ENABLE = 1
SMART_CAR_CONTROL_FLAG_BRAKE  = 2
```

正常控制：

```text
flags = ENABLE
```

停车或不允许控制：

```text
flags = 0
```

或：

```text
flags = BRAKE
```

## 3. Safety Behavior

控制保活使用 `SMART_CAR_CONTROL_SETPOINT`，不是 heartbeat。

若超过 `CMD_TIMEOUT_MS` 未收到有效控制帧，下位机会：

```text
1. Stop M2006 motors
2. Recenter steering servo
3. Set FAULT_CMD_TIMEOUT
4. Send one STATUSTEXT: "Host command timeout"
```

默认超时：

```text
CMD_TIMEOUT_MS = 200 ms
```

重要约定：

```text
Do not send only one control frame.
The host must continuously send SMART_CAR_CONTROL_SETPOINT while controlling the vehicle.
```

## 4. Control Units

### Speed

```text
Field: target_speed_mps
Unit:  m/s
Default limit: +/- 1.50 m/s
```

下位机会将车速换算成 M2006 电机目标 rpm：

```text
wheel_rpm = speed_mps * 60 / (PI * 0.065)
motor_rpm = wheel_rpm * 36
```

### Curvature

```text
Field: target_curvature
Unit:  1/m
Default limit: +/- 2.50 1/m
```

下位机会将曲率换算成舵机角度。当前默认标定模型：

```text
kappa = -0.056703297 * (servo_angle_deg - 92.0) + 0.028327473
```

因此上位机只需要发送目标曲率，不需要直接控制舵机角度或 PWM。

## 5. Telemetry

下位机周期发送：

```text
HEARTBEAT                 1 Hz
SMART_CAR_STATUS          10 Hz
SMART_CAR_MOTION_STATE    50 Hz
SMART_CAR_MOTOR_STATUS    20 Hz
SMART_CAR_IMU_STATUS      20 Hz
SMART_CAR_CALIB_STATUS    10 Hz while calibrating
STATUSTEXT                Event-triggered
PARAM_VALUE               PARAM protocol response
```

### HEARTBEAT

标准 MAVLink heartbeat。

下位机：

```text
type      = MAV_TYPE_GROUND_ROVER
autopilot = MAV_AUTOPILOT_GENERIC
```

### SMART_CAR_STATUS

字段：

```text
uint32_t time_boot_ms
uint8_t  mode
uint8_t  state
uint32_t fault_flags
uint32_t warn_flags
uint16_t cmd_age_ms
uint16_t control_loop_hz
uint8_t  imu_online
uint8_t  can_online
uint8_t  host_online
uint8_t  servo_online
uint8_t  motor_online_mask
```

重点关注：

```text
fault_flags
warn_flags
cmd_age_ms
motor_online_mask
```

Fault flags：

```text
FAULT_CMD_TIMEOUT    = 0x00000001
FAULT_MOTOR1_OFFLINE = 0x00000002
FAULT_MOTOR2_OFFLINE = 0x00000004
FAULT_IMU_NOT_READY  = 0x00000008
FAULT_CAN_ERROR      = 0x00000010
FAULT_SERVO_CLAMPED  = 0x00000020
```

Motor online mask：

```text
bit0 = motor 1 online
bit1 = motor 2 online
```

### SMART_CAR_MOTION_STATE

字段：

```text
uint32_t time_boot_ms
float    speed_mps
float    target_speed_mps
float    yaw_rate_dps
float    yaw_deg
float    curvature_meas
float    curvature_cmd
float    steering_angle_deg
uint16_t steering_pwm_us
uint8_t  steering_clamped
```

说明：

```text
speed_mps          Current estimated vehicle speed
target_speed_mps   Current accepted target speed
yaw_rate_dps       Filtered IMU yaw rate
yaw_deg            Debug-only integrated yaw angle
curvature_meas     yaw_rate_radps / speed_mps
curvature_cmd      Current accepted target curvature
steering_angle_deg Current servo command angle
steering_pwm_us    Current servo PWM pulse
steering_clamped   1 if curvature command reached steering limit
```

### SMART_CAR_MOTOR_STATUS

字段：

```text
uint32_t time_boot_ms
int16_t  target_rpm_1
int16_t  target_rpm_2
int16_t  actual_rpm_1
int16_t  actual_rpm_2
int16_t  current_cmd_1
int16_t  current_cmd_2
int16_t  feedback_current_1
int16_t  feedback_current_2
uint16_t angle_1
uint16_t angle_2
uint8_t  online_mask
uint32_t can_tx_busy_count
uint32_t can_error_count
```

说明：

```text
M2006 feedback rpm is rotor rpm before the 36:1 gearbox.
Wheel rpm = motor rpm / 36.
```

### SMART_CAR_IMU_STATUS

字段：

```text
uint32_t time_boot_ms
uint32_t sample_count
int16_t  accel_x_mg
int16_t  accel_y_mg
int16_t  accel_z_mg
int32_t  gyro_x_mdps
int32_t  gyro_y_mdps
int32_t  gyro_z_mdps
float    yaw_rate_raw_dps
float    yaw_rate_dps
float    gyro_bias_z_dps
int16_t  temperature_c_x100
uint8_t  calibrated
uint32_t overrun_count
uint32_t error_count
```

重点关注：

```text
sample_count
yaw_rate_dps
gyro_bias_z_dps
calibrated
error_count
```

### SMART_CAR_CALIB_STATUS

曲率标定过程中发送。

字段：

```text
uint32_t time_boot_ms
uint8_t  state
uint8_t  sweep_enabled
uint8_t  yaw_sign_inverted
uint32_t point_id
uint16_t servo_pwm_us
int16_t  target_rpm
uint32_t sweep_index
uint32_t sweep_count
uint32_t valid_count
uint32_t invalid_count
float    v_center_avg
float    yaw_rate_avg
float    kappa_avg
float    radius_est
```

## 6. Commands

离散命令使用 `SMART_CAR_COMMAND`。

字段：

```text
uint32_t time_boot_ms
uint8_t  target_system
uint8_t  target_component
uint16_t command
float    param1
float    param2
float    param3
float    param4
```

命令枚举：

```text
1  SMART_CAR_COMMAND_ENABLE
2  SMART_CAR_COMMAND_DISABLE
3  SMART_CAR_COMMAND_STOP
4  SMART_CAR_COMMAND_RECENTER_SERVO
5  SMART_CAR_COMMAND_GYRO_CAL
6  SMART_CAR_COMMAND_CLEAR_FAULTS
7  SMART_CAR_COMMAND_START_CURVATURE_CAL_POINT   param1=pwm_us, param2=rpm
8  SMART_CAR_COMMAND_START_CURVATURE_CAL_LEFT    param1=rpm
9  SMART_CAR_COMMAND_START_CURVATURE_CAL_RIGHT   param1=rpm
10 SMART_CAR_COMMAND_STOP_CURVATURE_CAL
11 SMART_CAR_COMMAND_FIREWATER_OFF
```

常用命令：

```text
STOP
RECENTER_SERVO
GYRO_CAL
CLEAR_FAULTS
```

建议上位机在退出控制前发送 `SMART_CAR_COMMAND_STOP`。

## 7. Actuator Test

执行器测试使用 `SMART_CAR_ACTUATOR_TEST`。

字段：

```text
uint32_t time_boot_ms
uint8_t  target_system
uint8_t  target_component
uint16_t test_mask
float    servo_angle_deg
uint16_t servo_pwm_us
int16_t  motor1_rpm
int16_t  motor2_rpm
uint16_t duration_ms
```

test mask：

```text
0x0001: servo angle test
0x0002: servo PWM test
0x0004: motor RPM test
```

示例：

```text
test_mask       = 0x0001
servo_angle_deg = 92.0
duration_ms     = 1000
```

安全建议：

```text
Use low motor rpm first.
Use duration_ms to avoid continuous actuator output during early tests.
```

## 8. Parameters

支持标准 MAVLink 参数协议：

```text
PARAM_REQUEST_LIST
PARAM_REQUEST_READ
PARAM_SET
PARAM_VALUE
```

当前参数表：

```text
CMD_TIMEOUT_MS       default 200
MAX_SPEED_MPS        default 1.50
MAX_ACCEL_MPS2       default 1.00
MAX_CURVATURE        default 2.50
STEER_CENTER_DEG     default 92.0
STEER_K_SLOPE        default -0.056703297
STEER_K_OFFSET       default 0.028327473
MOTOR_MAX_RPM        default 16000
GYRO_YAW_SIGN        default 1
```

注意：

```text
Parameters are runtime-only in the current firmware.
They are not saved to non-volatile memory.
```

`STEER_CENTER_DEG`、`STEER_K_SLOPE`、`STEER_K_OFFSET` 会立即影响曲率到舵机角度的转换。

## 9. Host Development Flow

上位机最小控制流程：

```text
1. Open UART at 115200 8N1.
2. Load smart_car.xml as MAVLink v2 dialect.
3. Wait for HEARTBEAT from sysid 1.
4. Start sending SMART_CAR_CONTROL_SETPOINT at 20 Hz.
5. Monitor SMART_CAR_STATUS.fault_flags.
6. Display MOTION/MOTOR/IMU telemetry.
7. Send SMART_CAR_COMMAND_STOP before exiting control.
```

如果 `fault_flags != 0`，建议上位机：

```text
1. Stop sending non-zero speed.
2. Send STOP command.
3. Show the fault to the operator.
4. Send CLEAR_FAULTS only after the cause is resolved.
```

## 10. Test Tool

工程内提供测试工具：

```text
tools/host_mavlink_smoke.py
```

示例：

```powershell
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 listen --duration 10
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 control --speed 0.2 --curvature 0 --duration 3
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 timeout --speed 0.2
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 params list
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 actuator --servo-angle 92 --duration-ms 1000
E:\anaconda_envs\mavlink-dev\python.exe tools\host_mavlink_smoke.py --port COM8 stop
```

子命令：

```text
listen    Receive and print telemetry
control   Send periodic speed/curvature setpoints
timeout   Stop sending setpoints and observe timeout behavior
stop      Send STOP command
params    Test PARAM list/read/set
actuator  Test servo/motor actuator outputs
calib     Test curvature calibration commands
command   Send discrete smart-car commands
stress    High-rate setpoint stress test
```

## 11. Important Notes

```text
The control interface is speed + curvature.
The host should not directly control motor rpm for normal driving.
The host should not directly control servo PWM for normal driving.
Actuator test messages are for debug and calibration only.
Heartbeat is not the control keepalive.
SMART_CAR_CONTROL_SETPOINT is the control keepalive.
```

For safe first tests:

```text
1. Keep the vehicle lifted or wheels off the ground.
2. Use speed <= 0.2 m/s.
3. Use curvature = 0 first.
4. Verify STOP and timeout behavior before ground tests.
```
