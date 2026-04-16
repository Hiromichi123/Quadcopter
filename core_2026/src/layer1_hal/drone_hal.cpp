#include "layer1_hal/drone_hal.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"

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
}

void DroneHAL::publish_velocity(Velocity& velocity) {
    velocity.set_time(this->now());
    vel_pub_->publish(velocity.get_twist());
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
