// fsm/states/scan_barcode.rs
// 阶段四：以固定速度沿 y 轴飞行，等待二维码触发信号
#![allow(dead_code)]

use async_trait::async_trait;
use std::sync::Arc;

use crate::fsm::state::State;
use crate::fsm::context::MissionContext;
use crate::trajectory::Velocity;

/// 扫描持续时长（秒）
const SCAN_DURATION_SECS: f64 = 10.0;

pub struct ScanBarcodeState {
    ctx: Arc<MissionContext>,
    complete: bool,
}

impl ScanBarcodeState {
    pub fn new(ctx: Arc<MissionContext>) -> Self {
        ScanBarcodeState { ctx, complete: false }
    }
}

#[async_trait(?Send)]
impl State for ScanBarcodeState {
    async fn enter(&mut self) {
        println!("[ScanBarcode] === 阶段四: 速度飞行扫描二维码 ===");
        let mut fc = self.ctx.flight_ctrl.lock().unwrap();

        // 以 0.2 m/s 沿 y 轴推进，最长持续 SCAN_DURATION_SECS 秒
        // 若 is_barcode_rx 触发（检测到二维码）则提前退出
        let mut scan_vel = Velocity::new(0.0, 0.2, 0.0, 0.0, 0.0, 0.0);
        match fc.fly_by_vel_duration(&mut scan_vel, SCAN_DURATION_SECS, self.ctx.is_barcode_rx.clone()).await {
            Ok(_)  => println!("[ScanBarcode] 扫描结束"),
            Err(e) => println!("[ScanBarcode] 扫描中断: {}", e),
        }

        self.complete = true;
    }

    fn is_complete(&self) -> bool { self.complete }
    fn name(&self) -> &'static str { "ScanBarcode" }
}
