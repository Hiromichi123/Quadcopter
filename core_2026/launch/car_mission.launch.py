import os

import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    """Launch file for smart car mission executor."""
    fcu_url = LaunchConfiguration('fcu_url')
    real_robot_odom_topic = LaunchConfiguration('real_robot_odom_topic')
    udp_target_ip = LaunchConfiguration('udp_target_ip')
    udp_target_port = LaunchConfiguration('udp_target_port')

    launch_args = [
        DeclareLaunchArgument('fcu_url', default_value='serial:///dev/ttyS0:460800'),
        DeclareLaunchArgument('real_robot_odom_topic', default_value='/aft_mapped_to_init'),
        DeclareLaunchArgument('udp_target_ip', default_value='192.168.0.3'),
        DeclareLaunchArgument('udp_target_port', default_value='9005'),
    ]

    lidar_udp = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(
                os.path.dirname(os.path.realpath(__file__)),
                'lidar_udp.launch.py',
            )
        ]),
        launch_arguments={
            'real_robot_odom_topic': real_robot_odom_topic,
            'udp_target_ip': udp_target_ip,
            'udp_target_port': udp_target_port,
            'rviz': 'False',
        }.items(),
    )

    mavros_node = Node(
        package='mavros',
        executable='mavros_node',
        namespace='mavros',
        parameters=[{
            'fcu_url': fcu_url,
            'tgt_system': 1,
            'tgt_component': 1,
            'fcu_protocol': 'v2.0',
        }],
        output='screen',
    )

    lidar_data_node = Node(
        package='ros2_tools',
        executable='lidar_data_node',
        parameters=[{
            'use_simulation': False,
            'simulation_odom_topic': '/absolute_pose',
            'real_robot_odom_topic': real_robot_odom_topic,
        }],
        output='screen',
    )

    lidar_to_px4_bridge = Node(
        package='ros2_tools',
        executable='lidar_to_px4_bridge',
        parameters=[{
            'real_robot_odom_topic': real_robot_odom_topic,
        }],
        output='screen',
    )

    smart_car_bridge = Node(
        package='smart_car_bridge',
        executable='smart_car_bridge_node',
        name='smart_car_bridge_node',
        parameters=[{
            'platform_mode': 'car',
            'mavlink_source_topic': '/uas1/mavlink_source',
            'mavlink_sink_topic': '/uas1/mavlink_sink',
            'platform_target_topic': '/platform/target',
            'flight_setpoint_topic': '/mavros/setpoint_position/local',
            'target_system': 1,
            'target_component': 0,
            'host_system': 255,
            'host_component': 191,
            'control_rate_hz': 50.0,
            'control_input_timeout_ms': 500,
            'max_speed_mps': 1.5,
            'max_curvature': 2.5,
        }],
        output='screen',
    )

    car_mission_node = Node(
        package='core_2026',
        executable='car_mission_node',
        name='car_mission_executor',
        output='screen',
    )

    return launch.LaunchDescription(
        launch_args + [
            mavros_node,
            lidar_udp,
            lidar_data_node,
            lidar_to_px4_bridge,
            smart_car_bridge,
            car_mission_node,
        ]
    )
