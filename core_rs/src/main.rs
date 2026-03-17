#![allow(dead_code)]
#![allow(unused_imports)]
mod quadcopter;       // 无人机节点
mod flight_controller; // 飞行控制器
mod fsm;              // 有限状态机
mod trajectory;       // 轨迹库

use std::sync::{Arc, Mutex, RwLock};
use tokio::runtime::Runtime;

use crate::quadcopter::Quadcopter;
use crate::flight_controller::FlightController;
use crate::fsm::{
    MissionContext, StateMachine,
    PreFlightState, FindPoleState, ApproachBarcodeState, ScanBarcodeState,
};

// ── 主程序入口 ──────────────────────────────────────────────────────────────
fn main() {
    // 二维码检测信号通道
    let (is_barcode_tx, is_barcode_rx) = tokio::sync::watch::channel(false);
    let is_barcode_rx = Arc::new(RwLock::new(is_barcode_rx));

    // 创建飞行器节点（订阅视觉数据）
    let quad_node = Quadcopter::new(is_barcode_tx)
        .expect("quad_node: 飞行器节点创建失败");

    // 创建飞行控制器
    let flight_ctrl = Arc::new(Mutex::new(
        FlightController::new(quad_node.self_pos.clone())
            .expect("flight_ctrl_node: 创建失败"),
    ));

    // 构建任务上下文
    let ctx = Arc::new(MissionContext::new(
        flight_ctrl,
        quad_node.vision_msg.clone(),
        quad_node.self_pos.clone(),
        is_barcode_rx,
    ));

    // 运行任务 FSM
    Runtime::new()
        .expect("Tokio runtime 创建失败")
        .block_on(run_mission(ctx));

    println!("[Main] 任务流程结束");
}

// ── 任务状态机 ──────────────────────────────────────────────────────────────
async fn run_mission(ctx: Arc<MissionContext>) {
    let mut fsm = StateMachine::new();

    // 阶段一: 预飞行检查 + 起飞到巡航高度
    fsm.change_state(Box::new(PreFlightState::new(ctx.clone()))).await;

    // 阶段二: 旋转寻找红色杆 + 侧向校准
    fsm.change_state(Box::new(FindPoleState::new(ctx.clone()))).await;

    // 阶段三: 推算杆坐标并飞行到二维码扫描点
    fsm.change_state(Box::new(ApproachBarcodeState::new(ctx.clone()))).await;

    // 阶段四: 速度飞行扫描二维码
    fsm.change_state(Box::new(ScanBarcodeState::new(ctx.clone()))).await;

    println!("[Mission] 所有任务阶段完成");
}
