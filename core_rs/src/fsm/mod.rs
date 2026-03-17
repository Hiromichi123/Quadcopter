// fsm/mod.rs
pub mod state;         // State trait
pub mod state_machine; // StateMachine 驱动
pub mod context;       // 跨状态共享上下文
pub mod states;        // 具体任务状态实现

#[allow(unused_imports)]
pub use state::State;
#[allow(unused_imports)]
pub use state_machine::StateMachine;
#[allow(unused_imports)]
pub use context::MissionContext;
#[allow(unused_imports)]
pub use states::{PreFlightState, FindPoleState, ApproachBarcodeState, ScanBarcodeState};