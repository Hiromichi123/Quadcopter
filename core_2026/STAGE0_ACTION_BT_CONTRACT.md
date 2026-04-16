# Stage 0: Action + BT 接口契约说明（core_2026）

## 1. 目标与边界

### 1.1 目标
- 为上位机控制建立统一任务协议，作为后续行为树（BT）和 Action 实现的输入标准。
- 明确任务优先级、状态码、反馈频率、字段语义，避免后期联调反复修改。
- 保持与现有 topic 控制链路兼容，支持平滑迁移。

### 1.2 边界
- 本阶段仅定义契约和运行规则，不实现导航层能力。
- 本阶段不强制改动控制算法（`impl` 逻辑保持现状）。

---

## 2. 任务模型（Action 维度）

建议先定义 5 个动作，覆盖当前已验证飞行能力。

1. `Takeoff.action`
2. `GoToTarget.action`
3. `TrackVelocity.action`
4. `Land.action`
5. `ExecuteMission.action`

说明：
- 前 4 个是原子动作，便于 BT 复用。
- `ExecuteMission` 是任务组合动作，后续可由 BT 驱动执行。

---

## 3. 全局优先级规则（抢占）

统一使用数值越小优先级越高。

| 优先级值 | 名称 | 典型触发 | 说明 |
|---|---|---|---|
| 0 | `EMERGENCY_STOP` | 急停按钮、硬故障 | 可抢占任何动作，必须立即执行 |
| 1 | `SAFETY_LAND` | 失联超时、定位失效 | 抢占任务动作，进入安全降落 |
| 2 | `MANUAL_OVERRIDE` | 人工接管 | 抢占自动任务，切人工 |
| 3 | `SYSTEM_COMMAND` | 返航/系统控制 | 抢占普通任务 |
| 4 | `MISSION_COMMAND` | 上位机任务队列 | 正常业务优先级 |

冲突处理规则：
1. 更高优先级请求到达时，当前 goal 进入 `CANCELED` 或 `ABORTED`。
2. 同优先级冲突时，后到请求默认拒绝（`REJECTED_BUSY`），避免抖动。
3. 可配置策略：同优先级是否允许替换（默认否）。

---

## 4. 统一状态码定义

### 4.1 Goal 生命周期状态

| 代码 | 名称 | 说明 |
|---|---|---|
| 0 | `ACCEPTED` | 请求已接收，待执行 |
| 1 | `EXECUTING` | 正在执行 |
| 2 | `SUCCEEDED` | 执行完成 |
| 3 | `CANCELED` | 被取消 |
| 4 | `ABORTED` | 异常终止 |
| 5 | `REJECTED` | 请求未被接收 |
| 6 | `PREEMPTED` | 被更高优先级抢占 |
| 7 | `TIMEOUT` | 执行超时 |

### 4.2 失败原因码（Result.error_code）

| 代码 | 名称 | 说明 |
|---|---|---|
| 0 | `OK` | 正常 |
| 1001 | `REJECTED_BUSY` | 当前忙，不接受同级任务 |
| 1002 | `INVALID_ARGUMENT` | 参数非法 |
| 1003 | `PRECONDITION_FAILED` | 前置条件不满足（未解锁/模式不对） |
| 1004 | `CONTROL_TIMEOUT` | 控制环超时 |
| 1005 | `COMMS_TIMEOUT` | 通信超时 |
| 1006 | `ODOM_TIMEOUT` | 状态输入超时 |
| 1007 | `VISION_TIMEOUT` | 视觉输入超时 |
| 1008 | `SAFETY_TRIGGERED` | 安全策略触发 |
| 1009 | `EMERGENCY_STOP` | 急停触发 |
| 1010 | `INTERNAL_ERROR` | 内部未分类错误 |

---

## 5. 反馈频率规范（Feedback）

默认反馈频率建议：
- 控制类动作（`Takeoff`、`GoToTarget`、`TrackVelocity`、`Land`）：`5 Hz`
- 组合任务（`ExecuteMission`）：`2 Hz`

统一约束：
1. 最低反馈频率不得低于 `2 Hz`。
2. 关键状态变化（进入新阶段、失败、完成）必须立即上报一次，不受节流限制。
3. 上位机若连续 `2.0 s` 未收到 feedback，标记链路告警。

---

## 6. 字段规范（统一字段语义）

以下字段建议在所有 Action 的 Feedback/Result 中尽量复用。

### 6.1 统一基础字段

- `string task_id`
  - 任务唯一标识（上位机生成），用于幂等和追踪。
- `uint8 priority`
  - 优先级值，见第 3 节。
- `builtin_interfaces/Time stamp`
  - 反馈时间戳（节点时钟）。
- `uint8 state_code`
  - 生命周期状态码，见第 4.1 节。
- `uint16 error_code`
  - 失败原因码，见第 4.2 节。
- `string message`
  - 人类可读说明，便于联调。

### 6.2 控制进度字段（Feedback）

- `float32 progress`
  - 范围 `[0.0, 1.0]`，无法估算时填 `-1.0`。
- `float32 distance_error`
  - 当前距目标位置误差（米），无目标动作填 `-1.0`。
- `float32 yaw_error`
  - 偏航误差（弧度），无目标动作填 `-1.0`。
- `geometry_msgs/Pose current_pose`
  - 当前估计位姿。
- `geometry_msgs/Twist current_twist`
  - 当前估计速度。

### 6.3 结果字段（Result）

- `bool success`
- `uint8 final_state_code`
- `uint16 error_code`
- `string message`
- `float32 elapsed_sec`

---

## 7. Action 草案（建议）

以下为契约草案，不要求本阶段完成代码。

### 7.1 Takeoff.action

Goal:
- `string task_id`
- `uint8 priority`
- `float32 target_altitude`
- `float32 timeout_sec`

Feedback:
- 统一基础字段 + `progress` + `distance_error` + `current_pose`

Result:
- 统一结果字段

### 7.2 GoToTarget.action

Goal:
- `string task_id`
- `uint8 priority`
- `float32 x`
- `float32 y`
- `float32 z`
- `float32 yaw`
- `float32 timeout_sec`
- `float32 stable_time_sec`

Feedback:
- 统一基础字段 + `progress` + `distance_error` + `yaw_error` + `current_pose`

Result:
- 统一结果字段

### 7.3 TrackVelocity.action

Goal:
- `string task_id`
- `uint8 priority`
- `float32 vx`
- `float32 vy`
- `float32 vz`
- `float32 yaw_rate`
- `float32 duration_sec`

Feedback:
- 统一基础字段 + `progress` + `current_twist` + `current_pose`

Result:
- 统一结果字段

### 7.4 Land.action

Goal:
- `string task_id`
- `uint8 priority`
- `float32 timeout_sec`

Feedback:
- 统一基础字段 + `progress` + `current_pose`

Result:
- 统一结果字段

### 7.5 ExecuteMission.action

Goal:
- `string task_id`
- `uint8 priority`
- `string mission_name`
- `string bt_xml_uri`
- `float32 timeout_sec`

Feedback:
- 统一基础字段 + `progress` + `string current_node` + `string current_stage`

Result:
- 统一结果字段 + `string failed_node`

---

## 8. 幂等与重试规范

1. `task_id` 相同且 Goal 完全相同：返回已有执行状态，不重复执行。
2. `task_id` 相同但参数不同：直接 `REJECTED` + `INVALID_ARGUMENT`。
3. 仅网络重试场景允许重复提交；业务重发应使用新 `task_id`。

---

## 9. 与旧 Topic 控制的兼容策略

阶段 0 到阶段 2 期间使用双通道：
1. Action 作为主控制入口。
2. 原 topic 接口保留，仅用于回退和对照测试。
3. 增加桥接层时，桥接层只做协议转换，不引入新控制逻辑。

切换策略：
1. 灰度比例：10% -> 30% -> 100%
2. 每个阶段记录成功率、超时率、取消率
3. 达标后关闭 topic 控制写入口

---

## 10. 阶段 0 验收标准

1. 动作清单、字段、状态码、优先级表冻结（版本号 `v0.1`）。
2. 上位机可按文档模拟构造 Goal 和解析 Feedback/Result。
3. 团队评审通过并确认后续 BT 节点与 ActionServer 按该契约实现。

---

## 11. 版本与变更规则

- 当前版本：`v0.1`
- 变更策略：
  - 增加字段：向后兼容，次版本号 +1（`v0.x`）
  - 改字段语义/删除字段：不兼容，主版本号 +1（`v1.0`）
