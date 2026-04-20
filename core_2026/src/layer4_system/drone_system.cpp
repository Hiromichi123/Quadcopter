#include "layer4_system/drone_system.hpp"
#include <chrono>

using namespace fly_to_target_args;

DroneSystem::DroneSystem() {
    // Layer 1: HAL-ROS2节点，提供接口实现
    hal_ = std::make_shared<DroneHAL>();

    // Layer 2: 飞行控制器（注入接口引用 + 日志 + 稳态时钟）
    fc_ = std::make_unique<FlightController>(
        *hal_,              // IStateProvider&
        *hal_,              // ICommandPublisher&
        hal_->get_logger(), // HALの 日志接口
        hal_->get_clock()   // HALの 稳态或ROS时钟
    );

    // Layer 3: 任务执行器
    mission_ = std::make_unique<MissionExecutor>(
        *fc_,               // FlightController&
        *hal_,              // IStateProvider&
        *hal_,              // IVisionProvider&
        *hal_,              // IDvsAvoidProvider&
        *hal_,              // ICommandPublisher&
        hal_->get_logger()
    );

    // 独立 spin 线程，持续处理 HAL-ROS 回调
    spin_thread_ = std::make_shared<std::thread>([this]() {
        while (rclcpp::ok()) {
            rclcpp::spin_some(hal_);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    });

    // 注册 shutdown 回调，确保 spin 线程析构时 join
    rclcpp::on_shutdown([this]() {
        if (spin_thread_ && spin_thread_->joinable()) {
            spin_thread_->join();
        }
    });

    RCLCPP_INFO(hal_->get_logger(), "[DroneSystem] 系统初始化完成");
}

DroneSystem::~DroneSystem() {
    if (spin_thread_ && spin_thread_->joinable()) {
        spin_thread_->join();
    }
}

// main运行入口
void DroneSystem::run() {
    pre_flight_checks();
    mission_->run();
}

// 起飞前检查
void DroneSystem::pre_flight_checks() {
    RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 等待 FCU 连接...");
    while (rclcpp::ok() && !hal_->get_mavros_state().connected) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 等待定位状态 lidar_data...");
    while (rclcpp::ok() && !hal_->has_state()) {
        RCLCPP_WARN_THROTTLE(
            hal_->get_logger(), *hal_->get_clock(), 5000,
            "[PreFlight] 未收到 lidar_data 或外部定位里程计数据，不满足 OFFBOARD 安全条件");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // 预发布 setpoint
    // PX4 要求进入 OFFBOARD 前持续发送至少 2Hz，持续 0.5s+
    Target hold(0.0f, 0.0f, 0.5f, 0.0f);
    rclcpp::Rate rate(20);

    rclcpp::Time last_request = hal_->now();
    RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 开始请求 OFFBOARD 与解锁...");

    while (rclcpp::ok()) {
        hal_->publish_position(hold); // 持续send

        const auto ms = hal_->get_mavros_state(); // 获取mavros状态句柄
        const bool timeout = (hal_->now() - last_request) > rclcpp::Duration::from_seconds(1.0); // 超时位

        /*
            === 重要自检信息 ===
            mode：当前mavros状态
            armed：是否arm
            connected：是否连接fcu
            has_odom：定位数据打通
        */
        RCLCPP_INFO_THROTTLE(hal_->get_logger(), *hal_->get_clock(), 2000,
                            "[PreFlight] 状态: mode='%s', armed=%d, connected=%d, has_odom=%d",
                            ms.mode.c_str(), ms.armed, ms.connected, hal_->has_state());

        if (ms.mode != "OFFBOARD" && timeout) { // 先请求 OFFBOARD
            bool success = hal_->request_set_mode("OFFBOARD");
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 请求 OFFBOARD 模式... %s", success ? "FCU已接受" : "FCU请求失败");
            last_request = hal_->now();
        } else if (!ms.armed && ms.mode == "OFFBOARD" && timeout) { // 再arm
            const bool success = hal_->request_arm(true);
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] arming... %s", success ? "FCU已接受" : "FCU请求失败");
            last_request = hal_->now();
        } else if (ms.armed && ms.mode == "OFFBOARD") {
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] Armed + OFFBOARD 成功！");
            fc_->fly_to_target(target = hold); // 起飞至初始点hold（阻塞）
            break;
        }
        
        rate.sleep();
    }
}
