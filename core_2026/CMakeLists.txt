cmake_minimum_required(VERSION 3.8)
project(core_2026)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── 依赖 ──────────────────────────────────────────────────────
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(mavros_msgs REQUIRED)
find_package(ros2_tools REQUIRED)
find_package(vision_py REQUIRED)

# ── 源文件 ────────────────────────────────────────────────────
set(SOURCES
  src/layer1_hal/drone_hal.cpp
  src/layer2_control/pid_controller.cpp
  src/layer2_control/flight_controller.cpp
  src/layer3_mission/mission_executor.cpp
  src/layer4_system/drone_system.cpp
  src/main.cpp
)

# ── 可执行文件 ────────────────────────────────────────────────
add_executable(quad_node ${SOURCES})

# 头文件搜索路径：以 src/ 为根，使 #include "layer1_hal/..." 等路径有效
target_include_directories(quad_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

# ── 依赖链接 ──────────────────────────────────────────────────
ament_target_dependencies(quad_node
  rclcpp
  geometry_msgs
  nav_msgs
  std_msgs
  mavros_msgs
  ros2_tools
  vision_py
)

set(CMAKE_INSTALL_RPATH /usr/local/lib)

# ── 安装 ──────────────────────────────────────────────────────
install(TARGETS quad_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch/ DESTINATION share/${PROJECT_NAME}/)

ament_package()
