from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description() -> LaunchDescription:
    # ===== Launch arguments =====
    start_mavros = LaunchConfiguration('start_mavros')
    fcu_url = LaunchConfiguration('fcu_url')

    processing_strategy = LaunchConfiguration('processing_strategy')
    filter_strength = LaunchConfiguration('filter_strength')
    filter_min_component_pixels = LaunchConfiguration('filter_min_component_pixels')
    approach_score_mode = LaunchConfiguration('approach_score_mode')
    detect_min_circle_pixels = LaunchConfiguration('detect_min_circle_pixels')

    visualize_enabled = LaunchConfiguration('visualize_enabled')
    visualize_compare_mode = LaunchConfiguration('visualize_compare_mode')
    visualize_scale = LaunchConfiguration('visualize_scale')
    visualize_persistent_trail_enabled = LaunchConfiguration('visualize_persistent_trail_enabled')
    visualize_persistent_trail_decay = LaunchConfiguration('visualize_persistent_trail_decay')

    launch_args = [
        DeclareLaunchArgument(
            'start_mavros',
            default_value='true',
            description='Start mavros_node for PX4 SITL connection.'
        ),
        DeclareLaunchArgument(
            'fcu_url',
            default_value='udp://:14540@127.0.0.1:14557',
            description='MAVROS FCU URL for simulation.'
        ),
        DeclareLaunchArgument('processing_strategy', default_value='cluster'),
        DeclareLaunchArgument('filter_strength', default_value='0.40'),
        DeclareLaunchArgument('filter_min_component_pixels', default_value='10'),
        DeclareLaunchArgument('approach_score_mode', default_value='merged'),
        DeclareLaunchArgument('detect_min_circle_pixels', default_value='200'),
        DeclareLaunchArgument('visualize_enabled', default_value='true'),
        DeclareLaunchArgument('visualize_compare_mode', default_value='true'),
        DeclareLaunchArgument('visualize_scale', default_value='6'),
        DeclareLaunchArgument('visualize_persistent_trail_enabled', default_value='true'),
        DeclareLaunchArgument('visualize_persistent_trail_decay', default_value='0.90'),
    ]

    # ===== Nodes =====
    mavros_node = Node(
        package='mavros',
        executable='mavros_node',
        output='screen',
        arguments=['--ros-args', '--log-level', 'warn'],
        parameters=[{
            'fcu_url': fcu_url,
            'tgt_system': 1,
            'tgt_component': 1,
            'fcu_protocol': 'v2.0',
        }],
        condition=IfCondition(start_mavros),
    )

    lidar_data_node = Node(
        package='ros2_tools',
        executable='lidar_data_node',
        output='screen',
        parameters=[{
            'use_simulation': True,
            'simulation_odom_topic': '/absolute_pose',
            'real_robot_odom_topic': '/aft_mapped_to_init',
        }],
    )

    dvs_node = Node(
        package='vision_py',
        executable='dvs_raw_serial_node.py',
        output='screen',
        parameters=[{
            'processing_enabled': True,
            'processing_strategy': processing_strategy,
            'filter_strength': filter_strength,
            'filter_min_component_pixels': filter_min_component_pixels,
            'approach_score_mode': approach_score_mode,
            'detect_min_circle_pixels': detect_min_circle_pixels,
            'visualize_enabled': visualize_enabled,
            'visualize_compare_mode': visualize_compare_mode,
            'visualize_scale': visualize_scale,
            'visualize_persistent_trail_enabled': visualize_persistent_trail_enabled,
            'visualize_persistent_trail_decay': visualize_persistent_trail_decay,
        }],
    )

    core_node = Node(
        package='core_2026',
        executable='quad_node',
        output='screen',
    )

    return LaunchDescription(
        launch_args + [
            mavros_node,
            lidar_data_node,
            dvs_node,
            core_node,
        ]
    )
