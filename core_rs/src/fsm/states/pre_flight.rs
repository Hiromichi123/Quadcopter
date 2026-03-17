// fsm/states/pre_flight.rs
// 阶段一：预飞行检查 + 起飞到指定高度
#![allow(dead_code)]

use async_trait::async_trait;
use std::sync::Arc;

use crate::fsm::state::State;
use crate::fsm::context::MissionContext;
use crate::trajectory::Target;

pub struct PreFlightState {
    ctx: Arc<MissionContext>,
    complete: bool,
}

impl PreFlightState {
    pub fn new(ctx: Arc<MissionContext>) -> Self {
        PreFlightState { ctx, complete: false }
    }
}

#[async_trait(?Send)]
impl State for PreFlightState {
    async fn enter(&mut self) {
        println!("[PreFlight] === 阶段一: 预飞行检查 ===");
        let mut fc = self.ctx.flight_ctrl.lock().unwrap();

        match fc.pre_flight_checks_loop().await {
            Ok(_)  => println!("[PreFlight] 预检查通过, 开始起飞"),
            Err(e) => {
                println!("[PreFlight] 预检查失败: {}", e);
                return;
            }
        }

        // 起飞到任务高度 1.0 m
        let mut cruise_altitude = Target::new(0.0, 0.0, 1.0, 0.0);
        match fc.fly_to_target_sync(&mut cruise_altitude) {
            Ok(_)  => println!("[PreFlight] 到达巡航高度 1.0 m"),
            Err(e) => println!("[PreFlight] 上升到高度失败: {}", e),
        }

        self.complete = true;
    }

    fn is_complete(&self) -> bool { self.complete }
    fn name(&self) -> &'static str { "PreFlight" }
}
