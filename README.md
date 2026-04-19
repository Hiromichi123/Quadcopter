# Quadcopter - ROS2四旋翼飞行控制系统

🌐 Language / 语言: **简体中文** | [English](README.en.md)

[![ROS2](https://img.shields.io/badge/ROS2-Humble-blue.svg)](https://docs.ros.org/en/humble/)
[![License](https://img.shields.io/badge/license-TODO-green.svg)](LICENSE)
[![Rust](https://img.shields.io/badge/rust-stable-orange.svg)](https://www.rust-lang.org/)

一个基于ROS2的四旋翼无人机飞行控制系统，集成了视觉识别、路径规划、状态机控制等功能。支持C++、Python和Rust多语言开发。

## 📑 目录

- [项目概述](#项目概述)
- [系统架构](#系统架构)
- [功能特性](#功能特性)
- [环境要求](#环境要求)
- [安装指南](#安装指南)
- [包说明](#包说明)
- [快速开始](#快速开始)
- [开发文档](#开发文档)
- [项目结构](#项目结构)
- [贡献指南](#贡献指南)

## 🎯 项目概述

本项目是一个完整的四旋翼无人机飞行控制系统，致力于打造最优秀的开源飞控库。系统集成了以下核心技术栈：

- **ROS2 Humble**: 机器人操作系统框架
- **MAVROS2**: MAVLink通信协议支持
- **OpenCV**: 计算机视觉处理
- **PCL**: 点云处理
- **RealSense**: 深度相机支持
- **多语言支持**: C++、Python、Rust

## 🏗️ 系统架构

```
┌─────────────────────────────────────────────────────────────┐
│                      Quadcopter System                      │
├──────────────┬──────────────┬──────────────┬───────────────┤
│ core (C++旧) │ core_2026    │  core_rs     │  vision_py    │
│ 飞行控制      │ 重构分层架构  │  (Rust版)    │  视觉识别      │
│ 状态管理      │ 见下方架构图  │  高性能控制  │  图像处理      │
├──────────────┴──────────────┴──────────────┴───────────────┤
│                  ROS2 Communication Layer                   │
│              (Topics, Services, Parameters)                 │
├─────────────────────────────────────────────────────────────┤
│                      MAVROS Interface                       │
│                     (MAVLink Protocol)                      │
├─────────────────────────────────────────────────────────────┤
│                      Flight Controller                      │
│                        (PX4/ArduPilot)                      │
└─────────────────────────────────────────────────────────────┘
```

### core_2026 内部分层架构

```
┌──────────────────────────────────────────────────────┐
│  layer4_system/   系统编排层                          │
│  DroneSystem · 生命周期管理 · pre_flight_checks()     │
├──────────────────────────────────────────────────────┤
│  layer3_mission/  任务执行层                          │
│  MissionExecutor · enum class 状态机                 │
├──────────────────────────────────────────────────────┤
│  layer2_control/  飞行控制层                          │
│  FlightController · PidController（可配置增益）       │
├──────────────────────────────────────────────────────┤
│  layer1_hal/      硬件抽象层                          │
│  DroneHAL（唯一 ROS2 Node）                           │
│  IStateProvider · ICommandPublisher · IVisionProvider │
├──────────────────────────────────────────────────────┤
│  layer0_common/   公共数据类型层                      │
│  DroneState · Target · Velocity · Path               │
└──────────────────────────────────────────────────────┘
```

## ✨ 功能特性

### 核心功能
- ✅ **自主飞行控制**: 基于位置和速度的精确控制
- ✅ **路径规划**: 支持航点任务和轨迹跟踪
- ✅ **有限状态机**: 完整的飞行状态管理
- ✅ **视觉识别**: 目标检测、跟踪和定位
- ✅ **实时避障**: 基于深度相机的障碍物检测

### 高级特性
- 🔄 **混合语言开发**: C++、Python、Rust协同工作
- 🚀 **高性能**: Rust实现的关键控制模块
- 📷 **视觉伺服**: 基于视觉的精确定位和跟踪
- 🎯 **二维码识别**: 自动识别和定位二维码
- 📊 **数据可视化**: RViz集成和实时监控

## 💻 环境要求

### 操作系统
- Ubuntu 20.04 LTS -> 22.04 LTS（推荐）

### 软件依赖
- ROS2 Humble
- Rust (stable)
- Python 3.8+
- OpenCV 4.x
- PCL 1.10+
- RealSense SDK 2.x

### 硬件要求
- 支持MAVROS的飞控（PX4/ArduPilot）
- RealSense深度相机（可选）
- GPS模块（可选）

## 📥 安装指南

### 1. 安装ROS2 Humble

```bash
# 使用鱼香ROS一键安装（推荐新手）
wget http://fishros.com/install -O fishros && bash fishros

# 或访问官网手动安装
# https://docs.ros.org/en/humble/Installation.html
```

### 2. 安装Rust环境

```bash
# 安装Rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env

# 安装cargo-ament-build插件
cargo install --debug cargo-ament-build
```

### 3. 安装Python依赖

```bash
# 安装colcon插件
pip install git+https://github.com/colcon/colcon-cargo.git
pip install git+https://github.com/colcon/colcon-ros-cargo.git

# 安装其他依赖
sudo apt install -y git libclang-dev python3-pip python3-vcstool
```

### 4. 安装MAVROS

```bash
sudo apt install ros-humble-mavros ros-humble-mavros-extras
# 安装GeographicLib数据集
wget https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh
chmod +x install_geographiclib_datasets.sh
sudo ./install_geographiclib_datasets.sh
```

### 5. 克隆并编译项目

```bash
# 克隆项目
git clone <repository-url> ~/ros2/quadcopter
cd ~/ros2/quadcopter

# 编译项目（首次编译）
source /opt/ros/humble/setup.bash
colcon build

# 使用符号链接安装（开发模式，推荐）
colcon build --symlink-install

# 编译特定包
colcon build --packages-select core_rs
```

### 6. 配置环境

```bash
# 添加到~/.bashrc
echo "source ~/ros2/quadcopter/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

## 📦 包说明

### core
**语言**: C++  
**功能**: 核心飞行控制模块（原始版本，功能可用）

- 飞行器状态管理
- 基础控制算法
- ROS2接口封装

> ⚠️ 已被 `core_2026` 重构替代，建议新开发基于 `core_2026`。

**依赖**:
- rclcpp
- geometry_msgs
- nav_msgs
- mavros_msgs

---

### core_2026
**语言**: C++17及以上
**功能**: 核心飞行控制模块（重构版，分层解耦架构）

采用 **5层架构 + SOLID 原则** 完全重构，保留 `core` 全部功能，同时解决原版耦合问题：

| 层 | 目录 | 职责 |
|----|------|------|
| 0 | `layer0_common/` | 纯数据类型，无 ROS 依赖 |
| 1 | `layer1_hal/` | 硬件抽象，唯一 ROS2 Node，含接口定义 |
| 2 | `layer2_control/` | 飞行控制器 + 独立可配置 PID |
| 3 | `layer3_mission/` | 任务状态机，enum class 驱动 |
| 4 | `layer4_system/` | 系统编排，生命周期管理 |

**主要特性**:
- `DroneHAL` 是唯一 `rclcpp::Node`，所有通信集中管理，回调有 `std::mutex` 保护
- `FlightController` 不继承 Node，通过 `IStateProvider` / `ICommandPublisher` 接口注入
- `PidController` 独立类，增益可运行时热更新，可独立单元测试
- `MissionExecutor` 枚举状态机，每状态独立方法

**可执行节点**:
- `quad_node`: 完整飞行控制系统

**依赖**:
- rclcpp, geometry_msgs, nav_msgs, mavros_msgs, ros2_tools, vision_py

**详细改进说明**: 见 [core_2026/REFACTOR.md](core_2026/REFACTOR.md)

### core_rs
**语言**: Rust  
**功能**: 高性能飞行控制器

- 飞行控制器 (`FlightController`)
- 有限状态机 (`FSM`)
- 轨迹规划 (`Trajectory`)
- 四旋翼实体 (`Quadcopter`)

**主要特性**:
- 异步/同步飞行控制
- 精确位置控制
- 速度控制和航点跟踪
- 视觉引导飞行

**依赖**: 
- rclrs，std_msgs，geometry_msgs，mavros_msgs，cv_tools

### cv_tools
**语言**: C++
**功能**: 计算机视觉工具库

- 自定义消息定义
- 视觉数据接口
- RealSense相机支持

**依赖**: 
- sensor_msgs，cv_bridge，OpenCV，RealSense2

### vision_py
**语言**: Python
**功能**: 视觉识别和图像处理

- 目标检测
- 颜色识别
- 深度估计
- 二维码识别

**可执行节点**:
- `ball_calibration`: 球体颜色校准
- `ball_depth`: 球体深度估计
- `d_ballposition`: 球体位置检测

### ros2_tools
**语言**: C++  
**功能**: ROS2通用工具和消息定义

- 自定义消息类型
- 通用服务接口
- 工具函数库

### messages
**功能**: 自定义消息和服务定义

- 飞行控制消息
- 视觉数据消息
- 系统状态消息

## 🚀 快速开始

### 启动飞行控制系统

```bash
# 终端1: 启动MAVROS
ros2 launch mavros px4.launch

# 终端2: 启动核心控制节点
ros2 run core_rs core_rs

# 终端3: 启动视觉节点
ros2 run vision_py ball_depth
```

### 使用Launch文件

```bash
# 启动完整系统
ros2 launch core_rs core.py
```

### 测试基本功能

```bash
# 检查节点状态
ros2 node list

# 查看话题
ros2 topic list

# 查看消息类型
ros2 interface show cv_tools/msg/Vision

# 发送测试命令
ros2 topic pub /cmd_vel geometry_msgs/Twist "..."
```

## 📚 开发文档

详细文档请参考 `doc/` 目录：

- [1.安装指南.md](doc/1.安装指南.md) - ROS2 Rust详细安装步骤
- [2.ros2_rust指南.md](doc/2.ros2_rust指南.md) - Rust开发指南
- [3.编写第一个节点.md](doc/3.编写第一个节点.md) - 节点开发教程
- [4.开始写一个话题.md](doc/4.开始写一个话题.md) - 话题通信教程
- [ros2.md](doc/ros2.md) - ROS2常用命令和技巧

### 常用命令

```bash
# 清理构建文件
rm -rf build/ install/ log/

# 编译单个包
colcon build --packages-select <package_name>

# 查看包列表
ros2 pkg list | grep <keyword>

# 查看消息/服务类型
ros2 interface show <msg_type>

# 运行时使用符号链接（开发推荐）
colcon build --symlink-install
```

## 📂 项目结构

```
quadcopter/
├── README.md                 # 本文档
├── .gitignore               # Git忽略配置
├── build/                   # 编译输出（自动生成）
├── install/                 # 安装文件（自动生成）
├── log/                     # 日志文件（自动生成）
│
├── core/                    # C++核心控制包（原始版）
│   ├── CMakeLists.txt
│   ├── package.xml
│   ├── launch/
│   └── src/classes/
│
├── core_2026/               # C++核心控制包（重构版，推荐）
│   ├── CMakeLists.txt
│   ├── package.xml
│   ├── REFACTOR.md          # 详细重构说明
│   ├── launch/
│   └── src/
│       ├── main.cpp
│       ├── layer0_common/   # 公共数据类型（无 ROS 依赖）
│       ├── layer1_hal/      # 硬件抽象层（唯一 ROS2 Node）
│       ├── layer2_control/  # 飞行控制 + PID
│       ├── layer3_mission/  # 任务状态机
│       └── layer4_system/   # 系统编排
│
├── core_rs/                 # Rust核心控制包
│   ├── Cargo.toml
│   ├── package.xml
│   ├── launch/              # Launch文件
│   ├── src/                 # 源代码
│   │   ├── main.rs         # 主程序
│   │   ├── quadcopter.rs   # 四旋翼实体
│   │   ├── flight_controller.rs  # 飞行控制器
│   │   ├── fsm/            # 有限状态机
│   │   └── trajectory/     # 轨迹规划
│   └── target/             # Rust编译输出
│
├── cv_tools/               # 视觉工具包
│   ├── CMakeLists.txt
│   ├── package.xml
│   ├── include/            # 头文件
│   ├── msg/                # 消息定义
│   └── src/                # 源代码
│
├── vision_py/              # Python视觉识别包
│   ├── package.xml
│   ├── setup.py
│   ├── setup.cfg
│   ├── msg/                # 消息定义
│   └── scripts/            # Python脚本
│
├── ros2_tools/             # ROS2工具包
│   ├── CMakeLists.txt
│   ├── package.xml
│   ├── msg/                # 消息定义
│   └── src/                # 源代码
│
├── messages/               # 自定义消息包
│   ├── CMakeLists.txt
│   ├── package.xml
│   └── msg/                # 消息定义
│
├── vision/                 # 视觉算法库
├── yolip/                  # YOLO相关
│
└── doc/                    # 文档目录
    ├── 1.安装指南.md
    ├── 2.ros2_rust指南.md
    ├── 3.编写第一个节点.md
    ├── 4.开始写一个话题.md
    └── ros2.md
```

## 🔧 开发提示

### 符号链接 vs 常规安装

```bash
# 开发模式 - 修改源码后无需重新构建Python和launch文件
colcon build --symlink-install

# 发布模式 - 完整拷贝所有文件
colcon build
```

## 🤝 贡献指南

欢迎贡献代码、报告问题或提出建议！

1. Fork本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启Pull Request

### 代码规范

- **C++**: 遵循ROS2代码风格
- **Python**: PEP 8
- **Rust**: `rustfmt` 格式化

## 📄 许可证

TODO: 添加许可证信息

## 👤 维护者

**Hiromichi123**  
Email: 2271612727@qq.com

## 🙏 致谢

- ROS2社区
- ros2-rust项目
- MAVROS团队
- 所有贡献者

## 📮 联系方式

如有问题或建议，请通过以下方式联系：

- 提交Issue
- 发送邮件至: 2271612727@qq.com

---

**谦源广道** - 2025 © Quadcopter Project
