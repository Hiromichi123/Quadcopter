#include "layer1_hal/drone_hal.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

#include <chrono>
#include <algorithm>
#include <cctype>
#include <cmath>

DroneHAL::DroneHAL() : Node("drone_hal_node") {
    platform_mode_name_ = this->declare_parameter<std::string>("platform_mode", "px4_drone");
    position_setpoint_topic_ = this->declare_parameter<std::string>(
        "position_setpoint_topic", "/mavros/setpoint_position/local");
    velocity_setpoint_topic_ = this->declare_parameter<std::string>(
        "velocity_setpoint_topic", "/mavros/setpoint_velocity/cmd_vel");
    platform_target_topic_ = this->declare_parameter<std::string>(
        "platform_target_topic", "/platform/target");
    car_position_kp_speed_ = static_cast<float>(
        this->declare_parameter<double>("car_position_kp_speed", 0.8));
    car_yaw_kp_ = static_cast<float>(
        this->declare_parameter<double>("car_yaw_kp", 1.5));
    car_max_speed_mps_ = static_cast<float>(
        this->declare_parameter<double>("car_max_speed_mps", 0.6));
    car_max_yaw_rate_radps_ = static_cast<float>(
        this->declare_parameter<double>("car_max_yaw_rate_radps", 1.0));
    car_xy_tolerance_m_ = static_cast<float>(
        this->declare_parameter<double>("car_xy_tolerance_m", 0.08));
    platform_mode_ = parse_platform_mode(platform_mode_name_);

    // 发布组
    pos_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
        position_setpoint_topic_, 10);
    vel_pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
        velocity_setpoint_topic_, 10);
    platform_target_pub_ = this->create_publisher<messages::msg::PlatformTarget>(
        platform_target_topic_, 10);

    // 订阅组
    lidar_sub_ = this->create_subscription<ros2_tools::msg::LidarPose>(
        "lidar_data", 10,
        [this](const ros2_tools::msg::LidarPose::SharedPtr msg){ DroneHAL::lidar_cb(msg);});
    state_sub_ = this->create_subscription<mavros_msgs::msg::State>(
        "/mavros/state", 10,
        [this](const mavros_msgs::msg::State::SharedPtr msg){ DroneHAL::state_cb(msg);});
    vision_sub_ = this->create_subscription<messages::msg::Vision>(
        "vision", 10,
        [this](const messages::msg::Vision::SharedPtr msg){ DroneHAL::vision_cb(msg);});

    const auto dvs_qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort().durability_volatile();
    dvs_detection_sub_ = this->create_subscription<std_msgs::msg::String>(
        "/dvs/ball_detection", dvs_qos,
        [this](const std_msgs::msg::String::SharedPtr msg){ DroneHAL::dvs_detection_cb(msg); });
    dvs_avoid_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "/dvs/avoid_cmd", dvs_qos,
        [this](const geometry_msgs::msg::Twist::SharedPtr msg){ DroneHAL::dvs_avoid_cb(msg); });

    // 客户端
    arming_client_  = this->create_client<mavros_msgs::srv::CommandBool>("mavros/cmd/arming");
    command_client_ = this->create_client<mavros_msgs::srv::CommandLong>("mavros/cmd/command");
    set_mode_client_= this->create_client<mavros_msgs::srv::SetMode>("mavros/set_mode");

    RCLCPP_INFO(
        this->get_logger(),
        "[DroneHAL] 硬件抽象层初始化完成: platform_mode=%s, pos_topic=%s, vel_topic=%s, platform_target_topic=%s",
        platform_mode_name_.c_str(),
        position_setpoint_topic_.c_str(),
        velocity_setpoint_topic_.c_str(),
        platform_target_topic_.c_str());
}

// ===== 接口组 =====
// 状态提供接口 IStateProvider
DroneState DroneHAL::get_state() const {
    std::lock_guard<std::mutex> lock(state_mutex_);
    return state_;
}

bool DroneHAL::has_state() const {
    std::lock_guard<std::mutex> lock(state_mutex_);
    return has_state_;
}

// 指令发布接口 ICommandPublisher
void DroneHAL::publish_position(Target& target) {
    switch (platform_mode_) {
        case PlatformMode::Px4MavrosDrone:
            publish_px4_drone_position(target);
            break;
        case PlatformMode::Px4MavrosDiffCar:
            publish_car_position_target(target, false);
            break;
        case PlatformMode::CustomAckermannCar:
            publish_car_position_target(target, true);
            break;
    }
    log_dvs_pipeline_latency_if_applicable("position");
}

void DroneHAL::publish_velocity(Velocity& velocity) {
    switch (platform_mode_) {
        case PlatformMode::Px4MavrosDrone:
            publish_px4_drone_velocity(velocity);
            break;
        case PlatformMode::Px4MavrosDiffCar:
            publish_car_velocity_target(velocity, false);
            break;
        case PlatformMode::CustomAckermannCar:
            publish_car_velocity_target(velocity, true);
            break;
    }
    log_dvs_pipeline_latency_if_applicable("velocity");
}

bool DroneHAL::uses_planar_position_control() const {
    return platform_mode_ != PlatformMode::Px4MavrosDrone;
}

// 视觉结果提供接口 IVisionProvider
messages::msg::Vision DroneHAL::get_vision() const {
    std::lock_guard<std::mutex> lock(vision_mutex_);
    return vision_;
}

bool DroneHAL::has_vision() const {
    std::lock_guard<std::mutex> lock(vision_mutex_);
    return has_vision_;
}

geometry_msgs::msg::Twist DroneHAL::get_dvs_avoid_cmd() const {
    std::lock_guard<std::mutex> lock(dvs_mutex_);
    return dvs_avoid_cmd_;
}

bool DroneHAL::has_recent_dvs_avoid(double max_age_sec) const {
    std::lock_guard<std::mutex> lock(dvs_mutex_);
    if (!has_dvs_avoid_) {
        return false;
    }

    const double age_sec = (this->now() - dvs_avoid_rx_time_).seconds();
    return age_sec >= 0.0 && age_sec <= max_age_sec;
}

int64_t DroneHAL::get_last_dvs_detect_time_ns() const {
    std::lock_guard<std::mutex> lock(dvs_mutex_);
    return last_dvs_detect_time_ns_;
}

// MAVRos 服务接口
bool DroneHAL::request_arm(bool arm) {
    if (!arming_client_->wait_for_service(std::chrono::milliseconds(500))) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] arming 服务未就绪");
        return false;
    }

    auto req = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
    req->value = arm;
    auto future = arming_client_->async_send_request(req);
    if (!future.valid()) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] arming 请求发送失败");
        return false;
    }

    if (future.wait_for(std::chrono::milliseconds(800)) != std::future_status::ready) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] arming 响应超时");
        return false;
    }

    const auto resp = future.get();
    if (!resp) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] arming 响应为空");
        return false;
    }

    RCLCPP_INFO(this->get_logger(), "[PreFlight] arming 响应: success=%d, result=%u",
        static_cast<int>(resp->success), static_cast<unsigned int>(resp->result));
    return resp->success;
}

bool DroneHAL::request_set_mode(const std::string& mode) {
    if (!set_mode_client_->wait_for_service(std::chrono::milliseconds(500))) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] set_mode 服务未就绪");
        return false;
    }

    auto req = std::make_shared<mavros_msgs::srv::SetMode::Request>();
    req->custom_mode = mode;
    auto future = set_mode_client_->async_send_request(req);
    if (!future.valid()) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] set_mode 请求发送失败: mode=%s", mode.c_str());
        return false;
    }

    if (future.wait_for(std::chrono::milliseconds(800)) != std::future_status::ready) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] set_mode 响应超时: mode=%s", mode.c_str());
        return false;
    }

    const auto resp = future.get();
    if (!resp) {
        RCLCPP_WARN(this->get_logger(), "[PreFlight] set_mode 响应为空: mode=%s", mode.c_str());
        return false;
    }

    RCLCPP_INFO(this->get_logger(), "[PreFlight] set_mode 响应: mode=%s, mode_sent=%d",
        mode.c_str(), static_cast<int>(resp->mode_sent));
    return resp->mode_sent;
}

mavros_msgs::msg::State DroneHAL::get_mavros_state() const {
    std::lock_guard<std::mutex> lock(mavros_mutex_);
    return mavros_state_;
}

bool DroneHAL::requires_mavros_preflight() const {
    return platform_mode_ != PlatformMode::CustomAckermannCar;
}

std::string DroneHAL::get_platform_mode_name() const {
    return platform_mode_name_;
}

DroneHAL::PlatformMode DroneHAL::parse_platform_mode(const std::string& mode_name) {
    std::string normalized;
    normalized.reserve(mode_name.size());
    for (const char c : mode_name) {
        normalized.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }

    if (normalized == "px4_diff_car" || normalized == "px4_rover" ||
        normalized == "diff_car" || normalized == "rover" || normalized == "car_px4") {
        return PlatformMode::Px4MavrosDiffCar;
    }
    if (normalized == "custom_ackermann" || normalized == "ackermann" ||
        normalized == "smart_car" || normalized == "custom_car") {
        return PlatformMode::CustomAckermannCar;
    }
    return PlatformMode::Px4MavrosDrone;
}

float DroneHAL::normalize_angle(float angle_rad) {
    return std::remainder(angle_rad, 2.0f * static_cast<float>(M_PI));
}

void DroneHAL::publish_px4_drone_position(Target& target) {
    target.set_time(this->now());
    pos_pub_->publish(target.get_pose());
}

void DroneHAL::publish_px4_drone_velocity(Velocity& velocity) {
    velocity.set_time(this->now());
    vel_pub_->publish(velocity.get_twist());
}

void DroneHAL::publish_car_position_target(const Target& target, bool use_custom_ackermann) {
    const auto platform_target = make_platform_target_from_position(target);

    if (use_custom_ackermann) {
        platform_target_pub_->publish(platform_target);
        return;
    }

    geometry_msgs::msg::TwistStamped twist;
    twist.header.stamp = this->now();
    twist.header.frame_id = "base_link";
    twist.twist.linear.x = platform_target.speed_mps;
    twist.twist.angular.z = std::clamp(
        platform_target.curvature * platform_target.speed_mps +
            platform_target.yaw_rate_dps * static_cast<float>(M_PI) / 180.0f,
        -car_max_yaw_rate_radps_,
        car_max_yaw_rate_radps_);
    vel_pub_->publish(twist);
}

void DroneHAL::publish_car_velocity_target(Velocity& velocity, bool use_custom_ackermann) {
    if (use_custom_ackermann) {
        platform_target_pub_->publish(make_platform_target_from_velocity(velocity));
        return;
    }

    geometry_msgs::msg::TwistStamped twist;
    twist.header.stamp = this->now();
    twist.header.frame_id = "base_link";
    twist.twist.linear.x = velocity.get_vx();
    twist.twist.angular.z = std::clamp(
        velocity.get_vyaw(),
        -car_max_yaw_rate_radps_,
        car_max_yaw_rate_radps_);
    vel_pub_->publish(twist);
}

messages::msg::PlatformTarget DroneHAL::make_platform_target_from_position(const Target& target) const {
    const DroneState s = get_state();
    const float dx = target.get_x() - s.x;
    const float dy = target.get_y() - s.y;
    const float yaw = s.yaw;
    const float body_x = std::cos(yaw) * dx + std::sin(yaw) * dy;
    const float body_y = -std::sin(yaw) * dx + std::cos(yaw) * dy;
    const float distance = std::hypot(body_x, body_y);
    const float yaw_error = normalize_angle(target.get_yaw() - s.yaw);

    messages::msg::PlatformTarget msg;
    msg.platform = messages::msg::PlatformTarget::PLATFORM_CAR;
    msg.x = body_x;
    msg.y = body_y;
    msg.z = 0.0f;
    msg.yaw = yaw_error;
    msg.vx_mps = 0.0f;
    msg.vy_mps = 0.0f;
    msg.vz_mps = 0.0f;

    if (distance <= car_xy_tolerance_m_) {
        msg.speed_mps = 0.0f;
        msg.curvature = 0.0f;
    } else {
        const float direction = body_x < -car_xy_tolerance_m_ ? -1.0f : 1.0f;
        msg.speed_mps = direction * std::clamp(
            distance * car_position_kp_speed_,
            0.0f,
            car_max_speed_mps_);
        msg.curvature = 2.0f * body_y / std::max(distance * distance, 1.0e-3f);
    }
    msg.yaw_rate_dps = std::clamp(
        yaw_error * car_yaw_kp_,
        -car_max_yaw_rate_radps_,
        car_max_yaw_rate_radps_) * 180.0f / static_cast<float>(M_PI);
    return msg;
}

messages::msg::PlatformTarget DroneHAL::make_platform_target_from_velocity(const Velocity& velocity) const {
    messages::msg::PlatformTarget msg;
    msg.platform = messages::msg::PlatformTarget::PLATFORM_CAR;
    msg.x = 0.0f;
    msg.y = 0.0f;
    msg.z = 0.0f;
    msg.yaw = 0.0f;
    msg.vx_mps = velocity.get_vx();
    msg.vy_mps = 0.0f;
    msg.vz_mps = 0.0f;
    msg.speed_mps = velocity.get_vx();
    msg.curvature = 0.0f;
    msg.yaw_rate_dps = std::clamp(
        velocity.get_vyaw(),
        -car_max_yaw_rate_radps_,
        car_max_yaw_rate_radps_) * 180.0f / static_cast<float>(M_PI);
    return msg;
}

// ===== 回调组 ======
void DroneHAL::lidar_cb(const ros2_tools::msg::LidarPose::SharedPtr msg) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    state_.x   = msg->x;
    state_.y   = msg->y;
    state_.z   = msg->z;
    state_.yaw = msg->yaw;
    has_state_ = true;
}

void DroneHAL::state_cb(const mavros_msgs::msg::State::SharedPtr msg) {
    std::lock_guard<std::mutex> lock(mavros_mutex_);
    mavros_state_ = *msg;
}

void DroneHAL::vision_cb(const messages::msg::Vision::SharedPtr msg) {
    std::lock_guard<std::mutex> lock(vision_mutex_);
    vision_    = *msg;
    has_vision_= true;
}

void DroneHAL::dvs_detection_cb(const std_msgs::msg::String::SharedPtr msg) {
    bool detected = false;
    int64_t detect_time_ns = 0;

    extract_json_bool(msg->data, "detected", detected);
    extract_json_int64(msg->data, "detect_time_ns", detect_time_ns);

    if (detected && detect_time_ns <= 0) {
        detect_time_ns = this->now().nanoseconds();
    }

    if (!detected || detect_time_ns <= 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(dvs_mutex_);
    last_dvs_detect_time_ns_ = detect_time_ns;
}

void DroneHAL::dvs_avoid_cb(const geometry_msgs::msg::Twist::SharedPtr msg) {
    std::lock_guard<std::mutex> lock(dvs_mutex_);
    dvs_avoid_cmd_ = *msg;
    dvs_avoid_rx_time_ = this->now();
    has_dvs_avoid_ = true;
}

bool DroneHAL::extract_json_int64(const std::string& json, const std::string& key, int64_t& out) {
    const std::string token = "\"" + key + "\"";
    const size_t key_pos = json.find(token);
    if (key_pos == std::string::npos) {
        return false;
    }

    const size_t colon_pos = json.find(':', key_pos + token.size());
    if (colon_pos == std::string::npos) {
        return false;
    }

    size_t i = colon_pos + 1;
    while (i < json.size() && std::isspace(static_cast<unsigned char>(json[i]))) {
        ++i;
    }
    if (i >= json.size()) {
        return false;
    }

    size_t j = i;
    if (json[j] == '-') {
        ++j;
    }
    const size_t digits_begin = j;
    while (j < json.size() && std::isdigit(static_cast<unsigned char>(json[j]))) {
        ++j;
    }
    if (j == digits_begin) {
        return false;
    }

    try {
        out = std::stoll(json.substr(i, j - i));
        return true;
    } catch (...) {
        return false;
    }
}

bool DroneHAL::extract_json_bool(const std::string& json, const std::string& key, bool& out) {
    const std::string token = "\"" + key + "\"";
    const size_t key_pos = json.find(token);
    if (key_pos == std::string::npos) {
        return false;
    }

    const size_t colon_pos = json.find(':', key_pos + token.size());
    if (colon_pos == std::string::npos) {
        return false;
    }

    size_t i = colon_pos + 1;
    while (i < json.size() && std::isspace(static_cast<unsigned char>(json[i]))) {
        ++i;
    }
    if (i >= json.size()) {
        return false;
    }

    if (json.compare(i, 4, "true") == 0) {
        out = true;
        return true;
    }
    if (json.compare(i, 5, "false") == 0) {
        out = false;
        return true;
    }
    return false;
}

void DroneHAL::log_dvs_pipeline_latency_if_applicable(const char* command_type) {
    int64_t detect_ns = 0;
    int64_t last_logged_ns = 0;
    {
        std::lock_guard<std::mutex> lock(dvs_mutex_);
        detect_ns = last_dvs_detect_time_ns_;
        last_logged_ns = last_latency_logged_detect_ns_;
    }

    if (detect_ns <= 0 || detect_ns == last_logged_ns) {
        return;
    }

    const int64_t now_ns = this->now().nanoseconds();
    if (now_ns < detect_ns) {
        return;
    }

    const double latency_ms = static_cast<double>(now_ns - detect_ns) / 1e6;
    if (latency_ms < 0.0 || latency_ms > 2000.0) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(dvs_mutex_);
        if (last_latency_logged_detect_ns_ == detect_ns) {
            return;
        }
        last_latency_logged_detect_ns_ = detect_ns;
    }

    RCLCPP_INFO(
        this->get_logger(),
        "[DVS_LATENCY] detect->%s_cmd total_latency=%.2f ms",
        command_type,
        latency_ms);
}
