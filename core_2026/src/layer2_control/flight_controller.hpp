#pragma once

#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "layer1_hal/i_state_provider.hpp"
#include "layer1_hal/i_command_publisher.hpp"
#include "layer2_control/pid_controller.hpp"
#include "layer0_common/target.hpp"
#include "layer0_common/velocity.hpp"
#include "layer0_common/path.hpp"
#include "named_args.hpp"

constexpr float DEFAULT_POS_CHECK_DISTANCE = 0.25f;

/**
 * @struct PidConfig
 * @brief XYZ轴 PID，供 fly_to_target_pid() 使用。
 */
struct PidConfig {
    PidGains xy{1.0f, 0.1f, 0.2f, 0.5f, 0.0f};  // XY 轴增益，解除速度限制
    PidGains z {1.5f, 0.2f, 0.1f, 0.3f, 0.0f};  // Z  轴增益，解除速度限制
};

/**
 * @brief 飞行控制层
 * - 负责将 mission层 的goal或path转换为具体的vel指令下发到 hal层，执行核心飞行控制逻辑。
 * - 通过依赖注入接收 IStateProvider 和 ICommandPublisher，与具体的 HAL-ROS层 解耦。
 * - PID 参数通过 PidConfig 注入
 */
class FlightController {
public:
    /**
     * @param state     位姿状态提供者（DroneHAL 实现）
     * @param cmd       指令发布者（DroneHAL 实现）
     * @param logger    日志记录器（由 DroneHAL::get_logger() 传入）
     * @param clock     稳态时钟（由 DroneHAL::get_clock() 传入）
     * @param rate_hz   控制频率（Hz），默认 20
     * @param pid_cfg   PID 增益配置，默认与原版常量一致
     */
    FlightController(IStateProvider&          state,
                     ICommandPublisher&        cmd,
                     rclcpp::Logger           logger,
                     rclcpp::Clock::SharedPtr clock,
                     int                      rate_hz = 20,
                     PidConfig                pid_cfg = {});

    // ===== 飞行动作组 ======
    
    // 定点飞行
    // 定点飞行（阻塞）非具名重载
    void fly_to_target(const Target& target,
                       float timeout_sec     = 10.0f,
                       float stable_time_sec = 0.1f,
                       int   frame_rate      = 30);
    
    // 定点飞行（阻塞）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void fly_to_target(Args&&... args);

    // 定点飞行（PID 速度闭环）非具名重载
    void fly_to_target_pid(const Target& target,
                           float timeout_sec     = 10.0f,
                           float stable_time_sec = 0.1f,
                           int   frame_rate      = 30);

    /// 定点飞行（PID 速度闭环，具名参数版本）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void fly_to_target_pid(Args&&... args);

    /// 速度飞行
    // 速度飞行，单次发布（非具名重载）
    void fly_by_velocity(const Velocity& velocity);

    /// 速度飞行，单次发布（具名参数版本）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void fly_by_velocity(Args&&... args);

    // 速度飞行，保持持续 duration 秒（含高度 P 控制）（非具名重载）
    void fly_by_vel_duration(const Velocity& velocity, float duration);

    /// 速度飞行，保持持续 duration 秒（含高度 P 控制）（具名参数版本）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void fly_by_vel_duration(Args&&... args);

    /// 路径航点飞行
    // 路径航点飞行（非具名重载）
    void fly_by_path(Path& path);

    /// 路径航点飞行（具名参数版本）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void fly_by_path(Args&&... args);

    /// 运行时更新 PID 增益
    // 运行时更新 PID 增益（非具名重载）
    void set_pid_config(PidConfig cfg);

    /// 运行时更新 PID 增益（具名参数版本）
    template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int> = 0>
    void set_pid_config(Args&&... args);

private:
    IStateProvider&               state_;
    ICommandPublisher&            cmd_;
    rclcpp::Logger                logger_;
    rclcpp::Clock::SharedPtr      clock_;
    std::shared_ptr<rclcpp::Rate> rate_;
    PidConfig                     pid_cfg_;

    /**
     * @brief 球形半径到达检查。
     * @return true = 位置与偏航均在误差内
     */
    bool pos_check(const Target& target, float distance = DEFAULT_POS_CHECK_DISTANCE);

    /**
     * @brief 各轴分别设置误差阈值，严格检查    
     * @return true = 位置与偏航均在误差内
     */
    bool pos_check(const Target& target,
                   float distance_x, float distance_y, float distance_z);

    // 定点移动（阻塞）impl
    void fly_to_target_impl(const Target& target,
                            float timeout_sec,
                            float stable_time_sec,
                            int   frame_rate);

    // 定点移动（PID 速度闭环）impl
    void fly_to_target_pid_impl(const Target& target,
                                float timeout_sec,
                                float stable_time_sec,
                                int   frame_rate);

    // 速度飞行，单次发布 impl
    void fly_by_velocity_impl(const Velocity& velocity);

    // 速度飞行，保持持续 duration 秒 impl
    void fly_by_vel_duration_impl(const Velocity& velocity, float duration);

    // 路径航点飞行 impl
    void fly_by_path_impl(Path& path);

    // 运行时更新 PID 增益 impl
    void set_pid_config_impl(PidConfig cfg);
};


// ── 定点移动（阻塞） ──────────────────────────────────────────────────────
namespace fly_to_target_args {
    struct TargetTag {};
    struct TimeoutSecTag {};
    struct StableTimeSecTag {};
    struct FrameRateTag {};

    inline constexpr named_args::ArgProxy<TargetTag>        target{};
    inline constexpr named_args::ArgProxy<TimeoutSecTag>    timeout_sec{};
    inline constexpr named_args::ArgProxy<StableTimeSecTag> stable_time_sec{};
    inline constexpr named_args::ArgProxy<FrameRateTag>     frame_rate{};
}

// 通过SFINAE确保参数不和旧写法冲突
template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>> 
inline void FlightController::fly_to_target(Args&&... args) {
    using namespace fly_to_target_args;

    static_assert(named_args::has<TargetTag, Args...>(),
                  "[fly_to_target] 缺失必要参数: target");
    
    // 1) 给可选参数设置默认值：调用时不传就走默认
    float timeout_sec_val     = named_args::get<TimeoutSecTag>   (10.0f, std::forward<Args>(args)...);
    float stable_time_sec_val = named_args::get<StableTimeSecTag>(0.1f, std::forward<Args>(args)...);
    int frame_rate_val        = named_args::get<FrameRateTag>    (20, std::forward<Args>(args)...);
    
    // 2) 必选参数直接通过 get 提取；缺失会在 static_assert 处编译失败。
    ::Target target_val = named_args::get<TargetTag>(::Target{}, std::forward<Args>(args)...);

    // 3) 具名参数解析完成后，复用原有阻塞控制逻辑实现。
    fly_to_target_impl(target_val, timeout_sec_val, stable_time_sec_val, frame_rate_val);
}

// ── 定点移动（PID 速度闭环） ──────────────────────────────────────────────────────
namespace fly_to_target_pid_args {
    struct TargetTag {};
    struct TimeoutSecTag {};
    struct StableTimeSecTag {};
    struct FrameRateTag {};

    inline constexpr named_args::ArgProxy<TargetTag>        target{};
    inline constexpr named_args::ArgProxy<TimeoutSecTag>    timeout_sec{};
    inline constexpr named_args::ArgProxy<StableTimeSecTag> stable_time_sec{};
    inline constexpr named_args::ArgProxy<FrameRateTag>     frame_rate{};
}

template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>>
inline void FlightController::fly_to_target_pid(Args&&... args) {
    using namespace fly_to_target_pid_args;

    static_assert(named_args::has<TargetTag, Args...>(),
                  "[fly_to_target_pid] 缺失必要参数: target");

    float timeout_sec_val = named_args::get<TimeoutSecTag>(10.0f, std::forward<Args>(args)...);
    float stable_time_sec_val = named_args::get<StableTimeSecTag>(0.1f, std::forward<Args>(args)...);
    int frame_rate_val = named_args::get<FrameRateTag>(30, std::forward<Args>(args)...);

    ::Target target_val = named_args::get<TargetTag>(::Target{}, std::forward<Args>(args)...);

    fly_to_target_pid_impl(target_val, timeout_sec_val, stable_time_sec_val, frame_rate_val);
}

// ── 速度飞行，单次发布 ──────────────────────────────────────────────────────
namespace fly_by_velocity_args {
    struct VelocityTag {};

    inline constexpr named_args::ArgProxy<VelocityTag> velocity{};
}

template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>>
inline void FlightController::fly_by_velocity(Args&&... args) {
    using namespace fly_by_velocity_args;

    static_assert(named_args::has<VelocityTag, Args...>(),
                  "[fly_by_velocity] 缺失必要参数: velocity");

    ::Velocity velocity_val = named_args::get<VelocityTag>(::Velocity{}, std::forward<Args>(args)...);

    fly_by_velocity_impl(velocity_val);
}

// ── 速度飞行，保持持续 duration 秒（含高度 P 控制） ─────────────────────────────────────────────
namespace fly_by_vel_duration_args {
    struct VelocityTag {};
    struct DurationTag {};

    inline constexpr named_args::ArgProxy<VelocityTag> velocity{};
    inline constexpr named_args::ArgProxy<DurationTag> duration{};
}

template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>>
inline void FlightController::fly_by_vel_duration(Args&&... args) {
    using namespace fly_by_vel_duration_args;

    static_assert(named_args::has<VelocityTag, Args...>(),
                  "[fly_by_vel_duration] 缺失必要参数: velocity");
    static_assert(named_args::has<DurationTag, Args...>(),
                  "[fly_by_vel_duration] 缺失必要参数: duration");

    float duration_val = named_args::get<DurationTag>(0.0f, std::forward<Args>(args)...);
    ::Velocity velocity_val = named_args::get<VelocityTag>(::Velocity{}, std::forward<Args>(args)...);

    fly_by_vel_duration_impl(velocity_val, duration_val);
}

// ── 路径航点飞行 ──────────────────────────────────────────────────────
namespace fly_by_path_args {
    struct PathPtrTag {};

    // Path 非拷贝传参场景下使用指针，避免 ArgProxy 的 decay 语义复制路径对象。
    inline constexpr named_args::ArgProxy<PathPtrTag> path_ptr{};
}

template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>>
inline void FlightController::fly_by_path(Args&&... args) {
    using namespace fly_by_path_args;

    static_assert(named_args::has<PathPtrTag, Args...>(),
                  "[fly_by_path] 缺失必要参数: path_ptr");

    Path* path_val = named_args::get<PathPtrTag>(static_cast<Path*>(nullptr), std::forward<Args>(args)...);

    if (path_val == nullptr) {
        RCLCPP_ERROR(logger_, "[fly_by_path]: 缺少必需的path_ptr参数");
        return;
    }

    fly_by_path_impl(*path_val);
}

// ── 运行时更新 PID 增益 ─────────────────────────────────────────────────────
namespace set_pid_config_args {
    struct CfgTag {};

    inline constexpr named_args::ArgProxy<CfgTag> cfg{};
}

template<typename... Args, std::enable_if_t<(named_args::is_arg_v<Args> && ...), int>>
inline void FlightController::set_pid_config(Args&&... args) {
    using namespace set_pid_config_args;

    static_assert(named_args::has<CfgTag, Args...>(),
                  "[set_pid_config] 缺失必要参数: cfg");

    ::PidConfig cfg_val = named_args::get<CfgTag>(::PidConfig{}, std::forward<Args>(args)...);

    set_pid_config_impl(cfg_val);
}
