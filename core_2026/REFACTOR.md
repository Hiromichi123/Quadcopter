# core_2026 重构说明文档

> 作者：Hiromichi123  
> 重构日期：2026-03-01  
> 原包：`core`  新包：`core_2026`

---

## 目录

1. [背景与动机](#1-背景与动机)
2. [原包问题清单](#2-原包问题清单)
3. [新架构设计](#3-新架构设计)
4. [目录结构对比](#4-目录结构对比)
5. [逐文件改进说明](#5-逐文件改进说明)
6. [设计原则对照](#6-设计原则对照)
7. [编译与运行](#7-编译与运行)
8. [后续扩展建议](#8-后续扩展建议)

---

## 1. 新架构设计

### 1.1 分层总览

```
┌─────────────────────────────────────────────────┐
│  Layer 3 · Mission                              │
│  MissionExecutor  (状态机，枚举 State 枚举)      │
├─────────────────────────────────────────────────┤
│  Layer 2 · Control                              │
│  FlightController  +  PidController             │
├─────────────────────────────────────────────────┤
│  Layer 1 · HAL Interfaces（纯虚接口）            │
│  IStateProvider  ICommandPublisher  IVisionProvider │
├─────────────────────────────────────────────────┤
│  Layer 1 · HAL Concrete（唯一 ROS2 Node）        │
│  DroneHAL  (订阅/发布/服务客户端，mutex 保护)    │
├─────────────────────────────────────────────────┤
│  Layer 0 · Common Data Types（纯数据，无 ROS 依赖）│
│  DroneState  Target  Velocity  Path              │
└─────────────────────────────────────────────────┘

System Orchestration：DroneSystem（组合所有层，管理生命周期）
Entry Point：main.cpp（3 行有效代码）
```

### 1.2 依赖关系

```
MissionExecutor  →  FlightController  →  IStateProvider
                 →  IStateProvider        (实现：DroneHAL)
                 →  IVisionProvider    →  ICommandPublisher
                                          (实现：DroneHAL)

FlightController →  IStateProvider
                 →  ICommandPublisher
                 →  PidController

DroneHAL         →  实现 IStateProvider, ICommandPublisher, IVisionProvider

DroneSystem      →  DroneHAL (shared_ptr)
                 →  FlightController (unique_ptr)
                 →  MissionExecutor (unique_ptr)
```

### 1.3 核心原则

| 原则 | 体现 |
|------|------|
| **单一职责 (SRP)** | 每个类只做一件事：DroneHAL 管通信、FlightController 管运动、MissionExecutor 管任务 |
| **依赖倒置 (DIP)** | FlightController 依赖 `IStateProvider`/`ICommandPublisher` 接口，不依赖 `DroneHAL` 具体类 |
| **开闭原则 (OCP)** | 切换传感器只需实现新的 `IStateProvider`，上层代码零修改 |
| **接口分离 (ISP)** | 三个细粒度接口（状态/指令/视觉），按需注入 |
| **RAII** | 所有资源以 `unique_ptr`/`shared_ptr` 管理，无裸 `new/delete` |

---

## 2. 目录结构

### 新包 `core_2026/`

```
core_2026/
├── CMakeLists.txt
├── package.xml
├── launch/
│   ├── core_launch.py
│   └── gazebo_launch.py
└── src/
    ├── main.cpp                        ← 入口
    ├── common/                         ← Layer 0: 纯数据
    │   ├── drone_state.hpp             ← 新增：统一状态结构体
    │   ├── target.hpp                  ← 重构：移除 const_cast
    │   ├── velocity.hpp                ← 重构：const 正确性
    │   └── path.hpp                    ← 重构：遍历索引私有化
    ├── hal/                            ← Layer 1: 硬件抽象
    │   ├── i_state_provider.hpp        ← 新增：纯虚接口
    │   ├── i_command_publisher.hpp     ← 新增：纯虚接口
    │   ├── i_vision_provider.hpp       ← 新增：纯虚接口
    │   ├── drone_hal.hpp               ← 新增：唯一 ROS2 Node
    │   └── drone_hal.cpp               ← 新增：mutex 保护回调
    ├── control/                        ← Layer 2: 控制
    │   ├── pid_controller.hpp/.cpp     ← 新增：独立可配置 PID
    │   ├── flight_controller.hpp       ← 重构：不再继承 Node
    │   └── flight_controller.cpp       ← 重构：依赖接口注入
    ├── mission/                        ← Layer 3: 任务
    │   ├── mission_executor.hpp        ← 新增：枚举状态机
    │   └── mission_executor.cpp        ← 新增：每状态独立方法
    └── system/                         ← 编排层
        ├── drone_system.hpp            ← 新增：生命周期管理
        └── drone_system.cpp            ← 新增：pre_flight_checks
```

---

## 3. 设计原则对照

| SOLID 原则 | 原 `core` | 新 `core_2026` |
|-----------|-----------|---------------|
| **S** 单一职责 | ✗ `quadcopter` 承担 4 类职责 | ✓ 每类只做一件事 |
| **O** 开闭原则 | ✗ 换传感器需改 `quadcopter` | ✓ 实现新 `IStateProvider` 即可 |
| **L** 里氏替换 | — | ✓ 接口可替换不同实现（仿真/实机） |
| **I** 接口分离 | ✗ 通过 `friend class` 暴露全部 | ✓ 三个细粒度接口按需注入 |
| **D** 依赖倒置 | ✗ 高层依赖低层具体类 | ✓ 高层依赖抽象接口 |

---

## 4. 编译与运行

### 编译

```bash
cd ~/ros2/Quadcopter
colcon build --packages-select core_2026
source install/setup.bash
```

### 实机运行

```bash
ros2 launch core_2026 core_launch.py
```

### Gazebo 仿真

```bash
ros2 launch core_2026 gazebo_launch.py
```

### 单独运行节点（调试）

```bash
ros2 run core_2026 quad_node
```

---

## 5. 后续扩展建议

### 5.1 ROS2 参数服务器集成 PID 热调参

在 `DroneHAL` 或 `DroneSystem` 中创建 ROS2 参数，并通过回调调用 `fc_->set_pid_config(...)` 实现运行时调参：

```cpp
this->declare_parameter("pid_xy_kp", 1.0);
// 参数变更回调
auto cb = [&](const rclcpp::Parameter& p) {
    auto cfg = fc_->get_pid_config();
    cfg.xy.kp = p.as_double();
    fc_->set_pid_config(cfg);
};
```

### 5.2 仿真 HAL

实现一个 `SimDroneHAL` 仅实现三个接口、读取 Gazebo 话题，可在无实机情况下完整测试 `FlightController` 和 `MissionExecutor`。

### 5.3 单元测试

`PidController` 无任何 ROS 依赖，可直接用 `gtest` 测试：

```cpp
TEST(PidTest, NoOvershoot) {
    PidController pid({1.0f, 0.0f, 0.0f, 1.0f, 10.0f});
    EXPECT_NEAR(pid.update(1.0f, 0.05), 1.0f, 0.001f);
}
```

### 5.4 任务可配置化

`MissionExecutor` 的 `default_altitude_`、`kFollowVx` 等参数可进一步通过 ROS2 参数注入，使同一二进制文件适配不同场地。

### 5.5 action_msgs 集成

`fly_to_target` 可封装为 ROS2 Action，对外暴露带反馈的异步接口，与 `core_action` 包实现统一。
