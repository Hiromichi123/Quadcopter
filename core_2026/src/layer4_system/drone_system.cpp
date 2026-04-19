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
        hal_->get_logger(),
        hal_->get_clock()   // HAL提供 稳态或ROS时钟
    );

    // Layer 3: 任务执行器
    mission_ = std::make_unique<MissionExecutor>(
        *fc_,
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
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // 注册 shutdown 回调，确保 spin 线程 join
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

    RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 等待定位状态(lidar_data)...");
    while (rclcpp::ok() && !hal_->has_state()) {
        RCLCPP_WARN_THROTTLE(
            hal_->get_logger(), *hal_->get_clock(), 2000,
            "[PreFlight] 尚未收到 lidar_data，暂不请求 OFFBOARD。请检查 real_robot_odom_topic 与 PointLIO 输出是否一致");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 预发布 setpoint（PX4 要求进入 OFFBOARD 前持续发送至少 2Hz，持续 0.5s+）
    Target hold(0.0f, 0.0f, 0.5f, 0.0f);
    rclcpp::Rate rate(20);

    rclcpp::Time last_request = hal_->now();
    RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 开始请求 OFFBOARD 与解锁...");

    while (rclcpp::ok()) {
        hal_->publish_position(hold); // 必须持续发送

        const auto ms = hal_->get_mavros_state();
        const bool timeout = (hal_->now() - last_request) > rclcpp::Duration::from_seconds(1.0);

        RCLCPP_INFO_THROTTLE(hal_->get_logger(), *hal_->get_clock(), 2000,
            "[PreFlight] 状态: mode='%s', armed=%d, connected=%d, has_lidar_state=%d",
            ms.mode.c_str(), ms.armed, ms.connected, hal_->has_state());

        if (ms.mode != "OFFBOARD" && timeout) {
            bool success = hal_->request_set_mode("OFFBOARD");
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] 请求 OFFBOARD 模式... %s",
                success ? "FCU已接受" : "FCU未接受/调用失败");
            last_request = hal_->now();
        } else if (!ms.armed && ms.mode == "OFFBOARD" && timeout) {
            const bool success = hal_->request_arm(true);
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] arming... %s",
                success ? "FCU已接受" : "FCU未接受/调用失败");
            last_request = hal_->now();
        } else if (ms.armed && ms.mode == "OFFBOARD") {
            RCLCPP_INFO(hal_->get_logger(), "[PreFlight] Armed + OFFBOARD 成功！");
            fc_->fly_to_target(target = hold); // 起飞至 hold（阻塞）
            break;
        }
        
        rate.sleep();
    }
}
