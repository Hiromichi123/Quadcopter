// fsm/states/approach_barcode.rs
// 阶段三：根据视觉距离推算杆坐标，飞行到二维码扫描点
#![allow(dead_code)]

use async_trait::async_trait;
use std::sync::Arc;

use crate::fsm::state::State;
use crate::fsm::context::MissionContext;
use crate::trajectory::Target;

pub struct ApproachBarcodeState {
    ctx: Arc<MissionContext>,
    complete: bool,
}

impl ApproachBarcodeState {
    pub fn new(ctx: Arc<MissionContext>) -> Self {
        ApproachBarcodeState { ctx, complete: false }
    }
}

#[async_trait(?Send)]
impl State for ApproachBarcodeState {
    async fn enter(&mut self) {
        println!("[ApproachBarcode] === 阶段三: 飞行至二维码扫描点 ===");

        // 读取当前位姿与视觉距离，推算杆坐标
        let (pole_x, pole_y) = {
            let pos      = self.ctx.self_pos.lock().unwrap();
            let vision   = self.ctx.vision_msg.lock().unwrap();
            let px = pos.x + vision.distance * pos.yaw.cos();
            let py = pos.y + vision.distance * pos.yaw.sin();
            (px, py)
        };
        println!("[ApproachBarcode] 估算杆坐标: ({:.3}, {:.3})", pole_x, pole_y);

        let mut fc = self.ctx.flight_ctrl.lock().unwrap();

        // 飞到扫描点的侧前方（偏移 0.5 m），高度 1.5 m，朝向 90°
        let mut scan_approach = Target::new(pole_x, pole_y + 0.5, 1.5, 1.57);
        match fc.fly_to_target_sync(&mut scan_approach) {
            Ok(_)  => println!("[ApproachBarcode] 到达扫描准备点"),
            Err(e) => {
                println!("[ApproachBarcode] 飞行失败: {}", e);
                return;
            }
        }

        // 下降到扫描高度 1.0 m
        let mut scan_height = Target::new(pole_x, pole_y + 0.5, 1.0, 1.57);
        match fc.fly_to_target_sync(&mut scan_height) {
            Ok(_)  => println!("[ApproachBarcode] 到达扫描高度 1.0 m"),
            Err(e) => println!("[ApproachBarcode] 下降失败: {}", e),
        }

        self.complete = true;
    }

    fn is_complete(&self) -> bool { self.complete }
    fn name(&self) -> &'static str { "ApproachBarcode" }
}
