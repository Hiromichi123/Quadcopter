use async_trait::async_trait;

// FSM 为单线程顺序执行，无需跨线程传递状态
#[async_trait(?Send)]
#[allow(dead_code)]
pub trait State {
    async fn enter(&mut self) {}
    async fn update(&mut self) {}
    async fn exit(&mut self) {}
    
    // 本状态是否已完成
    fn is_complete(&self) -> bool { false }
    // 状态名称（用于日志）
    fn name(&self) -> &'static str { "Unknown" }
}
