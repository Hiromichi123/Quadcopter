// fsm/context.rs
// 任务共享上下文，跨状态传递资源句柄
#![allow(dead_code)]

use std::sync::{Arc, Mutex, RwLock};
use tokio::sync::watch;
use cv_tools::msg::Vision;
use crate::flight_controller::FlightController;
use crate::quadcopter::SelfPos;

/// 任务上下文：持有所有跨状态共享的资源
pub struct MissionContext {
    /// 飞行控制器
    pub flight_ctrl: Arc<Mutex<FlightController>>,
    /// 视觉消息（视觉检测结果）
    pub vision_msg: Arc<Mutex<Vision>>,
    /// 飞行器自身位姿
    pub self_pos: Arc<Mutex<SelfPos>>,
    /// 二维码检测接收器（用于中断飞行）
    pub is_barcode_rx: Arc<RwLock<watch::Receiver<bool>>>,
}

impl MissionContext {
    pub fn new(
        flight_ctrl: Arc<Mutex<FlightController>>,
        vision_msg: Arc<Mutex<Vision>>,
        self_pos: Arc<Mutex<SelfPos>>,
        is_barcode_rx: Arc<RwLock<watch::Receiver<bool>>>,
    ) -> Self {
        MissionContext {
            flight_ctrl,
            vision_msg,
            self_pos,
            is_barcode_rx,
        }
    }
}
