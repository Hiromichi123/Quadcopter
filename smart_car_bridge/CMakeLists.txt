cmake_minimum_required(VERSION 3.8)
project(smart_car_bridge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(mavros_msgs REQUIRED)
find_package(libmavconn REQUIRED)
find_package(mavlink REQUIRED)
find_package(messages REQUIRED)

add_executable(smart_car_bridge_node
  src/smart_car_bridge_node.cpp
)

target_include_directories(smart_car_bridge_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(smart_car_bridge_node
  rclcpp
  std_msgs
  geometry_msgs
  mavros_msgs
  libmavconn
  mavlink
  messages
)

install(TARGETS smart_car_bridge_node DESTINATION lib/${PROJECT_NAME})
install(FILES launch/smart_car_bridge.launch.py DESTINATION share/${PROJECT_NAME}/launch)
install(DIRECTORY mavlink DESTINATION share/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
