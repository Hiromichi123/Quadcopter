#include "smart_car_bridge/smart_car_mavlink.hpp"

#include <mavros_msgs/mavlink_convert.hpp>
#include <mavros_msgs/msg/mavlink.hpp>

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <messages/msg/platform_target.hpp>
#include <messages/msg/smart_car_calib_status.hpp>
#include <messages/msg/smart_car_control_setpoint.hpp>
#include <messages/msg/smart_car_imu_status.hpp>
#include <messages/msg/smart_car_motion_state.hpp>
#include <messages/msg/smart_car_motor_status.hpp>
#include <messages/msg/smart_car_status.hpp>
#include <messages/srv/smart_car_actuator_test.hpp>
#include <messages/srv/smart_car_command.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <mutex>
#include <string>

namespace
{

using namespace std::chrono_literals;
namespace sc = smart_car_bridge::mavlink_dialect;

constexpr mavlink::msgid_t kMsgIdHeartbeat = 0;
constexpr mavlink::msgid_t kMsgIdStatusText = 253;

float clamp_float(const float value, const float limit)
{
  return std::clamp(value, -std::abs(limit), std::abs(limit));
}

uint32_t time_boot_ms(rclcpp::Clock & clock, const rclcpp::Time & start_time)
{
  const auto elapsed = clock.now() - start_time;
  return static_cast<uint32_t>(std::max<int64_t>(0, elapsed.nanoseconds() / 1000000));
}

geometry_msgs::msg::Quaternion yaw_to_quaternion(const double yaw)
{
  geometry_msgs::msg::Quaternion q;
  q.w = std::cos(yaw * 0.5);
  q.x = 0.0;
  q.y = 0.0;
  q.z = std::sin(yaw * 0.5);
  return q;
}

std::string statustext_to_string(const mavlink::mavlink_message_t & mav_msg)
{
  mavlink::common::msg::STATUSTEXT status_text;
  mavlink::MsgMap map(&mav_msg);
  status_text.deserialize(map);
  return mavlink::to_string(status_text.text);
}

}  // namespace

class SmartCarBridgeNode final : public rclcpp::Node
{
public:
  SmartCarBridgeNode()
  : Node("smart_car_bridge_node"),
    start_time_(this->get_clock()->now())
  {
    load_parameters();
    setup_ros_interfaces();

    const auto period = std::chrono::duration<double>(1.0 / control_rate_hz_);
    control_timer_ = this->create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      std::bind(&SmartCarBridgeNode::on_control_timer, this));

    RCLCPP_INFO(
      get_logger(),
      "Smart car bridge started in %s mode, MAVLink source=%s, sink=%s",
      platform_mode_.c_str(),
      mavlink_source_topic_.c_str(),
      mavlink_sink_topic_.c_str());
  }

  ~SmartCarBridgeNode() override
  {
    if (platform_mode_ == "car") {
      publish_command(sc::SMART_CAR_COMMAND_STOP, 0.0F, 0.0F, 0.0F, 0.0F);
    }
  }

private:
  void load_parameters()
  {
    platform_mode_ = declare_parameter<std::string>("platform_mode", "car");
    mavlink_source_topic_ = declare_parameter<std::string>("mavlink_source_topic", "/uas1/mavlink_source");
    mavlink_sink_topic_ = declare_parameter<std::string>("mavlink_sink_topic", "/uas1/mavlink_sink");
    platform_target_topic_ = declare_parameter<std::string>("platform_target_topic", "/platform/target");
    flight_setpoint_topic_ =
      declare_parameter<std::string>("flight_setpoint_topic", "/mavros/setpoint_position/local");
    target_system_ = static_cast<uint8_t>(declare_parameter<int>("target_system", 1));
    target_component_ = static_cast<uint8_t>(declare_parameter<int>("target_component", 0));
    host_system_ = static_cast<uint8_t>(declare_parameter<int>("host_system", 255));
    host_component_ = static_cast<uint8_t>(declare_parameter<int>("host_component", 191));
    control_rate_hz_ = declare_parameter<double>("control_rate_hz", 20.0);
    control_input_timeout_ms_ = declare_parameter<int>("control_input_timeout_ms", 150);
    max_speed_mps_ = declare_parameter<double>("max_speed_mps", 1.5);
    max_curvature_ = declare_parameter<double>("max_curvature", 2.5);

    if (platform_mode_ != "car" && platform_mode_ != "flight") {
      RCLCPP_WARN(
        get_logger(),
        "Unknown platform_mode '%s'; falling back to car mode",
        platform_mode_.c_str());
      platform_mode_ = "car";
    }
    if (control_rate_hz_ <= 0.0) {
      RCLCPP_WARN(get_logger(), "control_rate_hz must be positive; using 20 Hz");
      control_rate_hz_ = 20.0;
    }
  }

  void setup_ros_interfaces()
  {
    const auto mavlink_qos = rclcpp::QoS(1000).best_effort().durability_volatile();

    mavlink_pub_ = create_publisher<mavros_msgs::msg::Mavlink>(mavlink_sink_topic_, mavlink_qos);
    mavlink_sub_ = create_subscription<mavros_msgs::msg::Mavlink>(
      mavlink_source_topic_,
      mavlink_qos,
      std::bind(&SmartCarBridgeNode::on_mavlink_message, this, std::placeholders::_1));

    control_sub_ = create_subscription<messages::msg::SmartCarControlSetpoint>(
      "/smart_car/control_setpoint",
      10,
      std::bind(&SmartCarBridgeNode::on_control_setpoint, this, std::placeholders::_1));
    platform_target_sub_ = create_subscription<messages::msg::PlatformTarget>(
      platform_target_topic_,
      10,
      std::bind(&SmartCarBridgeNode::on_platform_target, this, std::placeholders::_1));

    flight_setpoint_pub_ =
      create_publisher<geometry_msgs::msg::PoseStamped>(flight_setpoint_topic_, 10);

    status_pub_ = create_publisher<messages::msg::SmartCarStatus>("/smart_car/status", 10);
    motion_pub_ = create_publisher<messages::msg::SmartCarMotionState>("/smart_car/motion_state", 10);
    motor_pub_ = create_publisher<messages::msg::SmartCarMotorStatus>("/smart_car/motor_status", 10);
    imu_pub_ = create_publisher<messages::msg::SmartCarImuStatus>("/smart_car/imu_status", 10);
    calib_pub_ = create_publisher<messages::msg::SmartCarCalibStatus>("/smart_car/calib_status", 10);
    statustext_pub_ = create_publisher<std_msgs::msg::String>("/smart_car/statustext", 10);

    command_srv_ = create_service<messages::srv::SmartCarCommand>(
      "/smart_car/command",
      std::bind(
        &SmartCarBridgeNode::on_command_service,
        this,
        std::placeholders::_1,
        std::placeholders::_2));
    actuator_test_srv_ = create_service<messages::srv::SmartCarActuatorTest>(
      "/smart_car/actuator_test",
      std::bind(
        &SmartCarBridgeNode::on_actuator_test_service,
        this,
        std::placeholders::_1,
        std::placeholders::_2));
  }

  void on_control_setpoint(const messages::msg::SmartCarControlSetpoint::SharedPtr msg)
  {
    std::lock_guard<std::mutex> lock(control_mutex_);
    latest_control_ = *msg;
    latest_control_time_ = now();
    has_control_ = true;
  }

  void on_platform_target(const messages::msg::PlatformTarget::SharedPtr msg)
  {
    if (platform_mode_ == "flight") {
      publish_flight_target(*msg);
      return;
    }

    auto control = messages::msg::SmartCarControlSetpoint();
    control.mode = messages::msg::SmartCarControlSetpoint::SMART_CAR_MODE_MANUAL;
    control.flags = messages::msg::SmartCarControlSetpoint::SMART_CAR_CONTROL_FLAG_ENABLE;
    control.target_yaw_rate_dps = msg->yaw_rate_dps;
    control.target_accel_mps2 = 0.0F;

    const float planar_speed = std::hypot(msg->vx_mps, msg->vy_mps);
    const float target_distance = std::hypot(msg->x, msg->y);
    if (std::abs(msg->speed_mps) > 1.0e-4F) {
      control.target_speed_mps = msg->speed_mps;
    } else if (planar_speed > 1.0e-4F) {
      control.target_speed_mps = planar_speed;
    } else {
      control.target_speed_mps = std::min<float>(target_distance, static_cast<float>(max_speed_mps_));
    }

    if (std::abs(msg->curvature) > 1.0e-4F) {
      control.target_curvature = msg->curvature;
    } else if (target_distance > 1.0e-3F) {
      control.target_curvature = 2.0F * msg->y / (target_distance * target_distance);
    } else {
      control.target_curvature = 0.0F;
    }

    std::lock_guard<std::mutex> lock(control_mutex_);
    latest_control_ = control;
    latest_control_time_ = now();
    has_control_ = true;
  }

  void publish_flight_target(const messages::msg::PlatformTarget & target)
  {
    geometry_msgs::msg::PoseStamped pose;
    pose.header.stamp = now();
    pose.header.frame_id = "map";
    pose.pose.position.x = target.x;
    pose.pose.position.y = target.y;
    pose.pose.position.z = target.z;
    pose.pose.orientation = yaw_to_quaternion(target.yaw);
    flight_setpoint_pub_->publish(pose);
  }

  void on_control_timer()
  {
    if (platform_mode_ != "car") {
      return;
    }

    messages::msg::SmartCarControlSetpoint control;
    bool timeout = true;
    {
      std::lock_guard<std::mutex> lock(control_mutex_);
      if (has_control_) {
        const auto age_ms = (now() - latest_control_time_).nanoseconds() / 1000000;
        timeout = age_ms > control_input_timeout_ms_;
        control = latest_control_;
      }
    }

    if (!has_control_ || timeout) {
      control.mode = sc::SMART_CAR_MODE_MANUAL;
      control.flags = sc::SMART_CAR_CONTROL_FLAG_BRAKE;
      control.target_speed_mps = 0.0F;
      control.target_curvature = 0.0F;
      control.target_yaw_rate_dps = 0.0F;
      control.target_accel_mps2 = 0.0F;
    }

    sc::ControlSetpoint mav_control;
    mav_control.time_boot_ms = time_boot_ms(*get_clock(), start_time_);
    mav_control.target_speed_mps =
      clamp_float(control.target_speed_mps, static_cast<float>(max_speed_mps_));
    mav_control.target_curvature =
      clamp_float(control.target_curvature, static_cast<float>(max_curvature_));
    mav_control.target_yaw_rate_dps = control.target_yaw_rate_dps;
    mav_control.target_accel_mps2 = control.target_accel_mps2;
    mav_control.seq = control_seq_++;
    mav_control.flags = control.flags;
    mav_control.target_system = target_system_;
    mav_control.target_component = target_component_;
    mav_control.mode = control.mode;

    publish_mavlink(sc::pack_control_setpoint(mav_control, host_system_, host_component_));
  }

  void on_mavlink_message(const mavros_msgs::msg::Mavlink::SharedPtr ros_msg)
  {
    if (ros_msg->framing_status != mavros_msgs::msg::Mavlink::FRAMING_OK) {
      return;
    }

    mavlink::mavlink_message_t mav_msg{};
    if (!mavros_msgs::mavlink::convert(*ros_msg, mav_msg)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 2000, "Failed to convert raw MAVLink message");
      return;
    }

    switch (mav_msg.msgid) {
      case kMsgIdHeartbeat:
        on_heartbeat(mav_msg);
        break;
      case sc::MSG_ID_SMART_CAR_STATUS:
        publish_status(mav_msg);
        break;
      case sc::MSG_ID_SMART_CAR_MOTION_STATE:
        publish_motion_state(mav_msg);
        break;
      case sc::MSG_ID_SMART_CAR_MOTOR_STATUS:
        publish_motor_status(mav_msg);
        break;
      case sc::MSG_ID_SMART_CAR_IMU_STATUS:
        publish_imu_status(mav_msg);
        break;
      case sc::MSG_ID_SMART_CAR_CALIB_STATUS:
        publish_calib_status(mav_msg);
        break;
      case kMsgIdStatusText:
        publish_statustext(mav_msg);
        break;
      default:
        break;
    }
  }

  void on_heartbeat(const mavlink::mavlink_message_t & mav_msg)
  {
    RCLCPP_INFO_THROTTLE(
      get_logger(),
      *get_clock(),
      5000,
      "Received heartbeat from sysid=%u compid=%u",
      mav_msg.sysid,
      mav_msg.compid);
  }

  void publish_status(const mavlink::mavlink_message_t & mav_msg)
  {
    sc::Status data;
    if (!sc::decode_status(mav_msg, data)) {
      return;
    }
    messages::msg::SmartCarStatus msg;
    msg.stamp = now();
    msg.time_boot_ms = data.time_boot_ms;
    msg.mode = data.mode;
    msg.state = data.state;
    msg.fault_flags = data.fault_flags;
    msg.warn_flags = data.warn_flags;
    msg.cmd_age_ms = data.cmd_age_ms;
    msg.control_loop_hz = data.control_loop_hz;
    msg.imu_online = data.imu_online;
    msg.can_online = data.can_online;
    msg.host_online = data.host_online;
    msg.servo_online = data.servo_online;
    msg.motor_online_mask = data.motor_online_mask;
    status_pub_->publish(msg);
  }

  void publish_motion_state(const mavlink::mavlink_message_t & mav_msg)
  {
    sc::MotionState data;
    if (!sc::decode_motion_state(mav_msg, data)) {
      return;
    }
    messages::msg::SmartCarMotionState msg;
    msg.stamp = now();
    msg.time_boot_ms = data.time_boot_ms;
    msg.speed_mps = data.speed_mps;
    msg.target_speed_mps = data.target_speed_mps;
    msg.yaw_rate_dps = data.yaw_rate_dps;
    msg.yaw_deg = data.yaw_deg;
    msg.curvature_meas = data.curvature_meas;
    msg.curvature_cmd = data.curvature_cmd;
    msg.steering_angle_deg = data.steering_angle_deg;
    msg.steering_pwm_us = data.steering_pwm_us;
    msg.steering_clamped = data.steering_clamped;
    motion_pub_->publish(msg);
  }

  void publish_motor_status(const mavlink::mavlink_message_t & mav_msg)
  {
    sc::MotorStatus data;
    if (!sc::decode_motor_status(mav_msg, data)) {
      return;
    }
    messages::msg::SmartCarMotorStatus msg;
    msg.stamp = now();
    msg.time_boot_ms = data.time_boot_ms;
    msg.target_rpm_1 = data.target_rpm_1;
    msg.target_rpm_2 = data.target_rpm_2;
    msg.actual_rpm_1 = data.actual_rpm_1;
    msg.actual_rpm_2 = data.actual_rpm_2;
    msg.current_cmd_1 = data.current_cmd_1;
    msg.current_cmd_2 = data.current_cmd_2;
    msg.feedback_current_1 = data.feedback_current_1;
    msg.feedback_current_2 = data.feedback_current_2;
    msg.angle_1 = data.angle_1;
    msg.angle_2 = data.angle_2;
    msg.online_mask = data.online_mask;
    msg.can_tx_busy_count = data.can_tx_busy_count;
    msg.can_error_count = data.can_error_count;
    motor_pub_->publish(msg);
  }

  void publish_imu_status(const mavlink::mavlink_message_t & mav_msg)
  {
    sc::ImuStatus data;
    if (!sc::decode_imu_status(mav_msg, data)) {
      return;
    }
    messages::msg::SmartCarImuStatus msg;
    msg.stamp = now();
    msg.time_boot_ms = data.time_boot_ms;
    msg.sample_count = data.sample_count;
    msg.overrun_count = data.overrun_count;
    msg.error_count = data.error_count;
    msg.gyro_x_mdps = data.gyro_x_mdps;
    msg.gyro_y_mdps = data.gyro_y_mdps;
    msg.gyro_z_mdps = data.gyro_z_mdps;
    msg.yaw_rate_raw_dps = data.yaw_rate_raw_dps;
    msg.yaw_rate_dps = data.yaw_rate_dps;
    msg.gyro_bias_z_dps = data.gyro_bias_z_dps;
    msg.accel_x_mg = data.accel_x_mg;
    msg.accel_y_mg = data.accel_y_mg;
    msg.accel_z_mg = data.accel_z_mg;
    msg.temperature_c_x100 = data.temperature_c_x100;
    msg.calibrated = data.calibrated;
    imu_pub_->publish(msg);
  }

  void publish_calib_status(const mavlink::mavlink_message_t & mav_msg)
  {
    sc::CalibStatus data;
    if (!sc::decode_calib_status(mav_msg, data)) {
      return;
    }
    messages::msg::SmartCarCalibStatus msg;
    msg.stamp = now();
    msg.time_boot_ms = data.time_boot_ms;
    msg.point_id = data.point_id;
    msg.sweep_index = data.sweep_index;
    msg.sweep_count = data.sweep_count;
    msg.valid_count = data.valid_count;
    msg.invalid_count = data.invalid_count;
    msg.v_center_avg = data.v_center_avg;
    msg.yaw_rate_avg = data.yaw_rate_avg;
    msg.kappa_avg = data.kappa_avg;
    msg.radius_est = data.radius_est;
    msg.target_rpm = data.target_rpm;
    msg.servo_pwm_us = data.servo_pwm_us;
    msg.state = data.state;
    msg.sweep_enabled = data.sweep_enabled;
    msg.yaw_sign_inverted = data.yaw_sign_inverted;
    calib_pub_->publish(msg);
  }

  void publish_statustext(const mavlink::mavlink_message_t & mav_msg)
  {
    std_msgs::msg::String msg;
    msg.data = statustext_to_string(mav_msg);
    statustext_pub_->publish(msg);
  }

  void on_command_service(
    const std::shared_ptr<messages::srv::SmartCarCommand::Request> request,
    std::shared_ptr<messages::srv::SmartCarCommand::Response> response)
  {
    publish_command(
      request->command,
      request->param1,
      request->param2,
      request->param3,
      request->param4);
    response->accepted = true;
    response->message = "command sent";
  }

  void on_actuator_test_service(
    const std::shared_ptr<messages::srv::SmartCarActuatorTest::Request> request,
    std::shared_ptr<messages::srv::SmartCarActuatorTest::Response> response)
  {
    sc::ActuatorTest test;
    test.time_boot_ms = time_boot_ms(*get_clock(), start_time_);
    test.servo_angle_deg = request->servo_angle_deg;
    test.test_mask = request->test_mask;
    test.servo_pwm_us = request->servo_pwm_us;
    test.duration_ms = request->duration_ms;
    test.motor1_rpm = request->motor1_rpm;
    test.motor2_rpm = request->motor2_rpm;
    test.target_system = target_system_;
    test.target_component = target_component_;
    publish_mavlink(sc::pack_actuator_test(test, host_system_, host_component_));
    response->accepted = true;
    response->message = "actuator test sent";
  }

  void publish_command(
    const uint16_t command,
    const float param1,
    const float param2,
    const float param3,
    const float param4)
  {
    sc::Command mav_command;
    mav_command.time_boot_ms = time_boot_ms(*get_clock(), start_time_);
    mav_command.command = command;
    mav_command.param1 = param1;
    mav_command.param2 = param2;
    mav_command.param3 = param3;
    mav_command.param4 = param4;
    mav_command.target_system = target_system_;
    mav_command.target_component = target_component_;
    publish_mavlink(sc::pack_command(mav_command, host_system_, host_component_));
  }

  void publish_mavlink(const mavlink::mavlink_message_t & mav_msg)
  {
    mavros_msgs::msg::Mavlink ros_msg;
    mavros_msgs::mavlink::convert(mav_msg, ros_msg);
    ros_msg.header.stamp = now();
    mavlink_pub_->publish(ros_msg);
  }

  std::string platform_mode_;
  std::string mavlink_source_topic_;
  std::string mavlink_sink_topic_;
  std::string platform_target_topic_;
  std::string flight_setpoint_topic_;
  uint8_t target_system_{1};
  uint8_t target_component_{0};
  uint8_t host_system_{255};
  uint8_t host_component_{191};
  double control_rate_hz_{20.0};
  int control_input_timeout_ms_{150};
  double max_speed_mps_{1.5};
  double max_curvature_{2.5};

  rclcpp::Time start_time_;
  std::mutex control_mutex_;
  messages::msg::SmartCarControlSetpoint latest_control_;
  rclcpp::Time latest_control_time_{0, 0, RCL_ROS_TIME};
  bool has_control_{false};
  uint16_t control_seq_{0};

  rclcpp::Publisher<mavros_msgs::msg::Mavlink>::SharedPtr mavlink_pub_;
  rclcpp::Subscription<mavros_msgs::msg::Mavlink>::SharedPtr mavlink_sub_;
  rclcpp::Subscription<messages::msg::SmartCarControlSetpoint>::SharedPtr control_sub_;
  rclcpp::Subscription<messages::msg::PlatformTarget>::SharedPtr platform_target_sub_;
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr flight_setpoint_pub_;
  rclcpp::Publisher<messages::msg::SmartCarStatus>::SharedPtr status_pub_;
  rclcpp::Publisher<messages::msg::SmartCarMotionState>::SharedPtr motion_pub_;
  rclcpp::Publisher<messages::msg::SmartCarMotorStatus>::SharedPtr motor_pub_;
  rclcpp::Publisher<messages::msg::SmartCarImuStatus>::SharedPtr imu_pub_;
  rclcpp::Publisher<messages::msg::SmartCarCalibStatus>::SharedPtr calib_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr statustext_pub_;
  rclcpp::Service<messages::srv::SmartCarCommand>::SharedPtr command_srv_;
  rclcpp::Service<messages::srv::SmartCarActuatorTest>::SharedPtr actuator_test_srv_;
  rclcpp::TimerBase::SharedPtr control_timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SmartCarBridgeNode>());
  rclcpp::shutdown();
  return 0;
}
