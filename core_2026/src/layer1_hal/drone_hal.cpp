#include "layer1_hal/drone_hal.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

#include <cctype>
#include <cmath>

DroneHAL::DroneHAL() : Node("drone_hal_node") {
    // 发布组
    pos_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
        "/mavros/setpoint_position/local", 10);
    vel_pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
        "/mavros/setpoint_velocity/cmd_vel", 10);

    // 订阅组
    lidar_sub_ = this->create_subscription<ros2_tools::msg::LidarPose>(
        "lidar_data", 10,
        [this](const ros2_tools::msg::LidarPose::SharedPtr msg){ DroneHAL::lidar_cb(msg);});
    state_sub_ = this->create_subscription<mavros_msgs::msg::State>(
        "/mavros/state", 10,
        [this](const mavros_msgs::msg::State::SharedPtr msg){ DroneHAL::state_cb(msg);});
    vision_sub_ = this->create_subscription<vision_py::msg::Vision>(
        "vision", 10,
        [this](const vision_py::msg::Vision::SharedPtr msg){ DroneHAL::vision_cb(msg);});

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

    RCLCPP_INFO(this->get_logger(), "[DroneHAL] 硬件抽象层 初始化完成");
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
    target.set_time(this->now());
    pos_pub_->publish(target.get_pose());
    log_dvs_pipeline_latency_if_applicable("position");
}

void DroneHAL::publish_velocity(Velocity& velocity) {
    velocity.set_time(this->now());
    vel_pub_->publish(velocity.get_twist());
    log_dvs_pipeline_latency_if_applicable("velocity");
}

// 视觉结果提供接口 IVisionProvider
vision_py::msg::Vision DroneHAL::get_vision() const {
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
    if (!arming_client_->service_is_ready()) return false;
    auto req = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
    req->value = arm;
    return arming_client_->async_send_request(req).valid(); // valid()有效 = 请求成功发出
}

bool DroneHAL::request_set_mode(const std::string& mode) {
    if (!set_mode_client_->service_is_ready()) return false;
    auto req = std::make_shared<mavros_msgs::srv::SetMode::Request>();
    req->custom_mode = mode;
    return set_mode_client_->async_send_request(req).valid();
}

mavros_msgs::msg::State DroneHAL::get_mavros_state() const {
    std::lock_guard<std::mutex> lock(mavros_mutex_);
    return mavros_state_;
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

void DroneHAL::vision_cb(const vision_py::msg::Vision::SharedPtr msg) {
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
