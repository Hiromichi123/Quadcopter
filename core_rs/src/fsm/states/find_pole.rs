// fsm/states/find_pole.rs
// 阶段二：旋转寻找红色杆 + 侧向校准
#![allow(dead_code)]

use async_trait::async_trait;
use std::sync::Arc;

use crate::fsm::state::State;
use crate::fsm::context::MissionContext;
use crate::trajectory::Velocity;

pub struct FindPoleState {
    ctx: Arc<MissionContext>,
    complete: bool,
}

impl FindPoleState {
    pub fn new(ctx: Arc<MissionContext>) -> Self {
        FindPoleState { ctx, complete: false }
    }
}

#[async_trait(?Send)]
impl State for FindPoleState {
    async fn enter(&mut self) {
        println!("[FindPole] === 阶段二: 旋转寻找红色杆 ===");
        let fc = self.ctx.flight_ctrl.lock().unwrap();

        // 绕 yaw 轴慢速旋转，直到视觉模块检测到红色杆
        let mut spin = Velocity::new(0.0, 0.0, 0.0, 0.1, 0.0, 0.0);
        while !self.ctx.vision_msg.lock().unwrap().is_red_detected {
            let _ = fc.fly_by_velocity(&mut spin);
        }
        println!("[FindPole] 检测到红色杆，停止旋转");

        // 侧向校准：将横向误差控制在 50 pixel 以内
        let mut align = Velocity::new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        loop {
            let later_err = self.ctx.vision_msg.lock().unwrap().later_error;
            if later_err.abs() <= 50 { break; }
            align.set_vy(later_err as f64 / 100.0);
            let _ = fc.fly_by_velocity(&mut align);
        }
        println!("[FindPole] 侧向校准完成，横向误差 ≤ 50 px");

        self.complete = true;
    }

    fn is_complete(&self) -> bool { self.complete }
    fn name(&self) -> &'static str { "FindPole" }
}
