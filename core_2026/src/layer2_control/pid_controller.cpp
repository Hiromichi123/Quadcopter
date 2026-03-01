#include "layer2_control/pid_controller.hpp"
#include <algorithm>

PidController::PidController(PidGains gains) : gains_(gains) {}

float PidController::update(float error, double dt) {
    if (dt <= 0.0) dt = 0.05; // 防止除零（与原版保持一致）

    // 积分项（含抗饱和限幅）
    integral_ += error * static_cast<float>(dt);
    integral_  = std::clamp(integral_, -gains_.i_limit, gains_.i_limit);

    // 微分项
    float deriv = (error - prev_error_) / static_cast<float>(dt);
    prev_error_ = error;

    float output = gains_.kp * error + gains_.ki * integral_ + gains_.kd * deriv;

    // 输出限幅（out_limit == 0 表示不限）
    if (gains_.out_limit > 0.0f) {
        output = std::clamp(output, -gains_.out_limit, gains_.out_limit);
    }
    return output;
}

void PidController::reset() {
    integral_   = 0.0f;
    prev_error_ = 0.0f;
}

void PidController::set_gains(PidGains gains) { gains_ = gains; }

PidGains PidController::get_gains() const { return gains_; }
