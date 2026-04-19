# Quadcopter - ROS2 Quadcopter Flight Control System

🌐 Language / 语言: [简体中文](README.md) | **English**

[![ROS2](https://img.shields.io/badge/ROS2-Humble-blue.svg)](https://docs.ros.org/en/humble/)
[![License](https://img.shields.io/badge/license-TODO-green.svg)](LICENSE)
[![Rust](https://img.shields.io/badge/rust-stable-orange.svg)](https://www.rust-lang.org/)

A ROS2-based quadcopter flight control system integrating vision, planning, and state-machine control. The repository supports multi-language development with C++, Python, and Rust.

## 📑 Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Packages](#packages)
- [Quick Start](#quick-start)
- [Documentation](#documentation)
- [Project Structure](#project-structure)
- [Contributing](#contributing)

## 🎯 Overview

This project provides a complete quadcopter control stack, designed for practical ROS2-based development and experimentation.

Core technologies include:

- **ROS2 Humble**: robotics middleware
- **MAVROS2**: MAVLink communication bridge
- **OpenCV**: computer vision
- **PCL**: point cloud processing
- **RealSense**: depth camera support
- **Multi-language stack**: C++, Python, Rust

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      Quadcopter System                      │
├──────────────┬──────────────┬──────────────┬───────────────┤
│ core (legacy)│ core_2026    │  core_rs     │  vision_py    │
│ flight ctrl   │ layered C++  │  Rust stack  │  vision stack │
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

### core_2026 Internal Layering

```
┌──────────────────────────────────────────────────────┐
│  layer4_system/   System orchestration               │
│  DroneSystem · lifecycle · pre_flight_checks()       │
├──────────────────────────────────────────────────────┤
│  layer3_mission/  Mission execution                  │
│  MissionExecutor · enum class state machine          │
├──────────────────────────────────────────────────────┤
│  layer2_control/  Flight control                     │
│  FlightController · PidController (configurable)     │
├──────────────────────────────────────────────────────┤
│  layer1_hal/      Hardware abstraction               │
│  DroneHAL (single ROS2 Node)                         │
│  IStateProvider · ICommandPublisher · IVisionProvider│
├──────────────────────────────────────────────────────┤
│  layer0_common/   Shared data models                 │
│  DroneState · Target · Velocity · Path               │
└──────────────────────────────────────────────────────┘
```

## ✨ Features

### Core
- ✅ Autonomous flight control (position and velocity)
- ✅ Path planning and waypoint following
- ✅ Finite-state mission management
- ✅ Vision-based object recognition and localization
- ✅ Real-time obstacle avoidance

### Advanced
- 🔄 Hybrid multi-language architecture (C++/Python/Rust)
- 🚀 High-performance Rust modules
- 📷 Vision servoing support
- 🎯 Marker/QR-related perception capability
- 📊 RViz-friendly runtime monitoring

## 💻 Requirements

### OS
- Ubuntu 20.04 LTS to 22.04 LTS (recommended)

### Software
- ROS2 Humble
- Rust (stable)
- Python 3.8+
- OpenCV 4.x
- PCL 1.10+
- RealSense SDK 2.x

### Hardware
- MAVROS-compatible FCU (PX4/ArduPilot)
- RealSense camera (optional)
- GPS module (optional)

## 📥 Installation

### 1. Install ROS2 Humble

```bash
# FishROS one-click installer (friendly for beginners)
wget http://fishros.com/install -O fishros && bash fishros

# Or use official instructions:
# https://docs.ros.org/en/humble/Installation.html
```

### 2. Install Rust

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env
cargo install --debug cargo-ament-build
```

### 3. Install Python / colcon dependencies

```bash
pip install git+https://github.com/colcon/colcon-cargo.git
pip install git+https://github.com/colcon/colcon-ros-cargo.git
sudo apt install -y git libclang-dev python3-pip python3-vcstool
```

### 4. Install MAVROS

```bash
sudo apt install ros-humble-mavros ros-humble-mavros-extras
wget https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh
chmod +x install_geographiclib_datasets.sh
sudo ./install_geographiclib_datasets.sh
```

### 5. Clone and build

```bash
git clone <repository-url> ~/ros2/quadcopter
cd ~/ros2/quadcopter

source /opt/ros/humble/setup.bash
colcon build

# Recommended for development
colcon build --symlink-install

# Build selected package
colcon build --packages-select core_rs
```

### 6. Configure environment

```bash
echo "source ~/ros2/quadcopter/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

## 📦 Packages

### core
- Language: C++
- Role: legacy core controller (stable but superseded)

### core_2026
- Language: C++17+
- Role: refactored layered controller
- Key points:
  - `DroneHAL` is the only ROS2 node in the control stack
  - `FlightController` depends on interfaces, not ROS node inheritance
  - PID is modular and runtime configurable
  - `MissionExecutor` uses explicit enum-based states

See [core_2026/REFACTOR.md](core_2026/REFACTOR.md) for details.

### core_rs
- Language: Rust
- Role: high-performance controller implementation

### cv_tools
- Language: C++
- Role: CV helper library and message interfaces

### vision_py
- Language: Python
- Role: visual perception and processing pipelines

### ros2_tools
- Language: C++
- Role: utility nodes/messages for system integration

### messages
- Role: custom message/service definitions

## 🚀 Quick Start

```bash
# Terminal 1
ros2 launch mavros px4.launch

# Terminal 2
ros2 run core_rs core_rs

# Terminal 3
ros2 run vision_py ball_depth
```

Or launch a full stack (depending on your setup):

```bash
ros2 launch core_rs core.py
```

Useful checks:

```bash
ros2 node list
ros2 topic list
ros2 interface show cv_tools/msg/Vision
```

## 📚 Documentation

See the `doc/` folder (Chinese):

- [doc/1.安装指南.md](doc/1.安装指南.md)
- [doc/2.ros2_rust指南.md](doc/2.ros2_rust指南.md)
- [doc/3.编写第一个节点.md](doc/3.编写第一个节点.md)
- [doc/4.开始写一个话题.md](doc/4.开始写一个话题.md)
- [doc/ros2.md](doc/ros2.md)

## 📂 Project Structure

```text
quadcopter/
├── README.md
├── README.en.md
├── core/
├── core_2026/
├── core_rs/
├── cv_tools/
├── vision_py/
├── ros2_tools/
├── messages/
├── vision/
├── yolip/
└── doc/
```

## 🤝 Contributing

Contributions are welcome.

1. Fork this repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

TODO: add license information.

## 👤 Maintainer

**Hiromichi123**
Email: 2271612727@qq.com

---

2025 © Quadcopter Project
