#pragma once

/**
 * @brief 单轴通用 PID控制器
 * 
 * - 支持积分限幅、输出限幅、积分重置。
 * - 无 ROS 依赖，可独立单元测试。
 *
 * @code
 *   PidGains g{1.0f, 0.1f, 0.2f, 0.5f, 1.0f};
 *   PidController pid_x(g);
 *   float vx = pid_x.update(err_x, dt);
 * @endcode
 */
struct PidGains {
    float kp{1.0f};        // 比例增益
    float ki{0.1f};        // 积分增益
    float kd{0.2f};        // 微分增益
    float i_limit{0.5f};   // 积分限幅（±）
    float out_limit{1.0f}; // 输出限幅（±），0 = 不限幅
};

class PidController {
public:
    explicit PidController(PidGains gains);

    /**
     * @brief 以误差和时间步长更新 PID 输出。
     * @param error  当前误差 (setpoint - feedback)
     * @param dt     距上次调用的时间 (秒)，需 > 0
     * @return 控制输出（已限幅）
     */
    float update(float error, double dt);

    /// 重置积分项与历史误差（切换目标点时调用）
    void reset();

    /// 运行时更新增益（支持参数服务器热更新）
    void set_gains(PidGains gains);

    [[nodiscard]] PidGains get_gains() const;

private:
    PidGains gains_;
    float    integral_{0.0f};
    float    prev_error_{0.0f};
};
