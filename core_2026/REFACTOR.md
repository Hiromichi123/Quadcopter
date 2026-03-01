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

## 1. 背景与动机

原 `core` 包能够完成完整的巡线投掷任务，但代码存在明显的**解耦不足**问题：`quadcopter` 类同时承担 ROS2 节点管理、硬件接口、控制逻辑、任务状态机四类职责，`flight_controller` 通过 `weak_ptr<quadcopter>` 直接访问私有字段，形成强耦合。

随着功能继续增加，这种结构将导致：

- 修改一处逻辑需要理解整个类
- 无法对单个功能模块进行单元测试
- 切换传感器/通信接口需要改动多个文件
- 代码难以被他人复用

`core_2026` 以**分层架构 + 依赖倒置**为目标完成重构，保留全部原有功能，同时实现清晰的边界划分。

---

## 2. 原包问题清单

| # | 问题 | 位置 | 严重程度 |
|---|------|------|----------|
| 1 | **God Object**：一个类承担 4 类职责 | `quadcopter` | ★★★★★ |
| 2 | **直接耦合**：通过 `weak_ptr` 访问内部字段 `lidar_pos->x`、`pos_pub`、`vel_pub` | `flight_controller.cpp` | ★★★★★ |
| 3 | **`friend class`**：绕过封装暴露私有成员 | `quadcopter.h` | ★★★★ |
| 4 | **`const_cast` 滥用**：通过 const 引用强制修改 `target.reached` | `flight_controller.cpp:pos_check` | ★★★ |
| 5 | **PID 参数硬编码**：增益常量无法配置、无法热更新 | `fly_to_target_pid()` | ★★★ |
| 6 | **裸 int flag 状态机**：250 行 switch-case 混在硬件节点中 | `quadcopter::main_loop` | ★★★★ |
| 7 | **重复状态维护**：`lidar_pos` SharedPtr 与裸 float `x/y/z/yaw` 同时存在 | `quadcopter.h` / `lidar_pose_cb` | ★★★ |
| 8 | **变量遮蔽**：`fly_by_vel_duration` 循环内再次 `lock()` 出同名 `quad` | `flight_controller.cpp` | ★★ |
| 9 | **魔法数字**：`-1000.0`、`20`、`5.0` 等未命名常量散布任务代码 | `main_loop` | ★★ |
| 10 | **线程不安全**：回调与主循环共享 `lidar_pos`/`vision_msg` 无 mutex 保护 | `quadcopter` | ★★★★ |

---

## 3. 新架构设计

### 3.1 分层总览

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

### 3.2 依赖关系（箭头 = "依赖于"）

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

### 3.3 核心原则

| 原则 | 体现 |
|------|------|
| **单一职责 (SRP)** | 每个类只做一件事：DroneHAL 管通信、FlightController 管运动、MissionExecutor 管任务 |
| **依赖倒置 (DIP)** | FlightController 依赖 `IStateProvider`/`ICommandPublisher` 接口，不依赖 `DroneHAL` 具体类 |
| **开闭原则 (OCP)** | 切换传感器只需实现新的 `IStateProvider`，上层代码零修改 |
| **接口分离 (ISP)** | 三个细粒度接口（状态/指令/视觉），按需注入 |
| **RAII** | 所有资源以 `unique_ptr`/`shared_ptr` 管理，无裸 `new/delete` |

---

## 4. 目录结构

### 新包 `core_2026/`

```
core_2026/
├── CMakeLists.txt
├── package.xml
├── launch/
│   ├── core_launch.py
│   └── gazebo_launch.py
└── src/
    ├── main.cpp                        ← 3 行入口
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

## 5. 逐文件改进说明

### 5.1 `common/drone_state.hpp`（新增）

**问题**：原 `quadcopter` 中 `lidar_pos` SharedPtr 与裸 float `x/y/z/yaw` 同时存在，`lidar_pose_cb` 需双重更新，容易漏赋值。  
**改进**：引入统一 POD 结构体 `DroneState`，回调只写一处，接口只读一处。

```cpp
// 原：双重维护
std::shared_ptr<ros2_tools::msg::LidarPose> lidar_pos; // 字段1
float x{}, y{}, z{}, yaw{};                           // 字段2（重复！）

// 新：唯一来源
DroneState state_; // 在 lidar_cb 中只更新这一处
```

---

### 5.2 `common/target.hpp`（重构）

**问题**：`mutable bool reached` + `const_cast<Target&>(target).reached = is_reached`  
**改进**：彻底移除 `reached` 字段，`pos_check()` 以返回值表达到达状态；`get_pose()` 标记为 `const`。

```cpp
// 原（有 UB 风险）
const_cast<Target&>(target).reached = is_reached;

// 新
bool reached = flight_ctrl_.pos_check(target); // 调用方接收返回值
```

---

### 5.3 `hal/drone_hal.hpp/.cpp`（新增）

**问题**：通信原语（pub/sub/client）分散在 `quadcopter` 中与控制逻辑混杂，无线程保护。  
**改进**：
- 所有 ROS2 通信只在 `DroneHAL` 中出现
- 回调与读取之间使用 `std::mutex` 保护（原版缺失，存在数据竞争）
- 对外只暴露三个接口，上层无法触碰 ROS handle

```cpp
// 原：spin 线程与主线程之间无同步
vision_msg = msg; // 回调直接覆写

// 新：mutex 保护
void DroneHAL::vision_cb(...) {
    std::lock_guard<std::mutex> lock(vision_mutex_);
    vision_ = *msg;
    has_vision_ = true;
}
```

---

### 5.4 `control/pid_controller.hpp/.cpp`（新增）

**问题**：PID 增益以 `const float Kp_xy = 1.0f` 等硬编码在 `fly_to_target_pid()` 函数体内，无法复用、无法测试、无法运行时调整。  
**改进**：独立 `PidController` 类，以 `PidGains` 结构体注入增益，支持 `set_gains()` 热更新。

```cpp
// 原：硬编码在函数里
const float Kp_xy = 1.0f, Ki_xy = 0.1f, Kd_xy = 0.2f;

// 新：可配置
PidGains gains{1.0f, 0.1f, 0.2f, 0.5f, 1.0f};
PidController pid_x(gains);
float vx = pid_x.update(err_x, dt);
```

---

### 5.5 `control/flight_controller.hpp/.cpp`（重构）

**问题**：
1. `flight_controller` 继承 `rclcpp::Node` 但不创建自己的 pub/sub，却依赖 `quadcopter` 的 pub/sub
2. `weak_ptr<quadcopter>` 直接访问 `quad->lidar_pos->x`、`quad->pos_pub`
3. `fly_by_vel_duration` 内部遮蔽外层 `quad` 变量

**改进**：
- 不再继承 `rclcpp::Node`
- 通过构造函数注入 `IStateProvider&`、`ICommandPublisher&`、`rclcpp::Logger`、`rclcpp::Clock::SharedPtr`
- 变量遮蔽问题消除（内层循环直接使用 `state_.get_state()`）

```cpp
// 原
class flight_controller : public rclcpp::Node {
    std::weak_ptr<quadcopter> quad_node;
    // ...
    auto quad = quad_node.lock();
    quad->pos_pub->publish(...);        // 直接访问私有成员
};

// 新
class FlightController {               // 不继承 Node
    IStateProvider&    state_;
    ICommandPublisher& cmd_;
    // ...
    cmd_.publish_position(target);     // 通过接口，与实现完全解耦
};
```

---

### 5.6 `mission/mission_executor.hpp/.cpp`（新增）

**问题**：`quadcopter::main_loop()` 中 ~150 行的 `switch(flag)` 状态机嵌在硬件节点类内，使用裸 `int flag`，每个 case 直接读取视觉/状态数据，混合业务逻辑。  
**改进**：
- 独立 `MissionExecutor` 类，职责单一
- `enum class State` 替代裸 `int flag`，编译期防止非法值
- 每个状态对应独立私有方法（`on_takeoff()`、`on_line_follow()` 等），逻辑清晰、可独立阅读
- 魔法数字提升为具名常量（`kForwardVx`、`kAlignThreshold` 等）

```cpp
// 原
int flag = 0;
while (rclcpp::ok()) {
    switch (flag) {
        case 0: ...; flag = 1; break;
        case 1: if (...) flag = 2; break;
        // ...250 行...
    }
}

// 新
enum class State { TAKEOFF, FORWARD, LINE_FOLLOW, ALIGN_SHAPE, RETURN_LINE, ALIGN_LAND, LAND, DONE };
void run() {
    while (rclcpp::ok() && current_state_ != State::DONE) {
        switch (current_state_) {
            case State::TAKEOFF:     on_takeoff();     break;
            case State::LINE_FOLLOW: on_line_follow(); break;
            // ...
        }
    }
}
```

---

### 5.7 `system/drone_system.hpp/.cpp`（新增，替换 `quadcopter::quad_init`）

**问题**：`quadcopter::quad_init()` 串联了 `flight_ctrl_init()`、`start_spin_thread()`、`pre_flight_checks_loop()`、`main_loop()`，对象创建与运行流程混合。  
**改进**：`DroneSystem` 构造函数管理创建顺序，`run()` 管理运行流程，职责分离。

---

### 5.8 `main.cpp`（简化）

原版 `main()` 在 `quadcopter.cpp` 末尾，与类定义混在一个文件。  
新版 `main.cpp` 独立存在，仅 3 行有效代码：

```cpp
rclcpp::init(argc, argv);
DroneSystem system;
system.run();
```

---

## 6. 设计原则对照

| SOLID 原则 | 原 `core` | 新 `core_2026` |
|-----------|-----------|---------------|
| **S** 单一职责 | ✗ `quadcopter` 承担 4 类职责 | ✓ 每类只做一件事 |
| **O** 开闭原则 | ✗ 换传感器需改 `quadcopter` | ✓ 实现新 `IStateProvider` 即可 |
| **L** 里氏替换 | — | ✓ 接口可替换不同实现（仿真/实机） |
| **I** 接口分离 | ✗ 通过 `friend class` 暴露全部 | ✓ 三个细粒度接口按需注入 |
| **D** 依赖倒置 | ✗ 高层依赖低层具体类 | ✓ 高层依赖抽象接口 |

---

## 7. 编译与运行

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

## 8. 后续扩展建议

### 8.1 ROS2 参数服务器集成 PID 热调参

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

### 8.2 仿真 HAL

实现一个 `SimDroneHAL` 仅实现三个接口、读取 Gazebo 话题，可在无实机情况下完整测试 `FlightController` 和 `MissionExecutor`。

### 8.3 单元测试

`PidController` 无任何 ROS 依赖，可直接用 `gtest` 测试：

```cpp
TEST(PidTest, NoOvershoot) {
    PidController pid({1.0f, 0.0f, 0.0f, 1.0f, 10.0f});
    EXPECT_NEAR(pid.update(1.0f, 0.05), 1.0f, 0.001f);
}
```

### 8.4 任务可配置化

`MissionExecutor` 的 `default_altitude_`、`kFollowVx` 等参数可进一步通过 ROS2 参数注入，使同一二进制文件适配不同场地。

### 8.5 action_msgs 集成

`fly_to_target` 可封装为 ROS2 Action，对外暴露带反馈的异步接口，与 `core_action` 包实现统一。
