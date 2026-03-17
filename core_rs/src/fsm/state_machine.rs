use super::state::State;

pub struct StateMachine {
    current_state: Option<Box<dyn State>>,
}

#[allow(dead_code)]
impl StateMachine {
    pub fn new() -> Self {
        StateMachine { current_state: None }
    }

    /// 切换到新状态：退出当前状态 → 进入新状态（enter 阻塞到本阶段完成）
    pub async fn change_state(&mut self, mut new_state: Box<dyn State>) {
        if let Some(mut current_state) = self.current_state.take() {
            println!("[FSM] 退出状态: {}", current_state.name());
            current_state.exit().await;
        }
        println!("[FSM] 进入状态: {}", new_state.name());
        new_state.enter().await;
        self.current_state = Some(new_state);
    }

    /// 轮询驱动当前状态（用于需要循环更新的状态）
    pub async fn update(&mut self) {
        if let Some(mut state) = self.current_state.take() {
            state.update().await;
            self.current_state = Some(state);
        }
    }

    /// 当前状态是否完成
    pub fn is_complete(&self) -> bool {
        self.current_state.as_ref().map_or(false, |s| s.is_complete())
    }

    /// 当前状态名称
    pub fn current_state_name(&self) -> &'static str {
        self.current_state.as_ref().map_or("None", |s| s.name())
    }
}
