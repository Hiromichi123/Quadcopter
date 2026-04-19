from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description() -> LaunchDescription:
    fcu_url = LaunchConfiguration('fcu_url')
    dvs_port = LaunchConfiguration('dvs_port')

    processing_strategy = LaunchConfiguration('processing_strategy')
    filter_strength = LaunchConfiguration('filter_strength')
    filter_min_component_pixels = LaunchConfiguration('filter_min_component_pixels')
    approach_score_mode = LaunchConfiguration('approach_score_mode')
    detect_min_circle_pixels = LaunchConfiguration('detect_min_circle_pixels')

    video_output_path = LaunchConfiguration('video_output_path')
    video_fps = LaunchConfiguration('video_fps')
    video_scale = LaunchConfiguration('video_scale')

    launch_args = [
        DeclareLaunchArgument('fcu_url', default_value='serial:///dev/ttyACM0:57600'),
        DeclareLaunchArgument('dvs_port', default_value='/dev/ttyACM1'),
        DeclareLaunchArgument('processing_strategy', default_value='cluster'),
        DeclareLaunchArgument('filter_strength', default_value='0.40'),
        DeclareLaunchArgument('filter_min_component_pixels', default_value='10'),
        DeclareLaunchArgument('approach_score_mode', default_value='merged'),
        DeclareLaunchArgument('detect_min_circle_pixels', default_value='200'),
        DeclareLaunchArgument('video_output_path', default_value='~/dvs_filtered_result.avi'),
        DeclareLaunchArgument('video_fps', default_value='30.0'),
        DeclareLaunchArgument('video_scale', default_value='6'),
    ]

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
    )

    lidar_data_node = Node(
        package='ros2_tools',
        executable='lidar_data_node',
        output='screen',
        parameters=[{
            'use_simulation': False,
            'simulation_odom_topic': '/absolute_pose',
            'real_robot_odom_topic': '/aft_mapped_to_init',
        }],
    )

    lidar_bridge_node = Node(
        package='ros2_tools',
        executable='lidar_to_px4_bridge',
        output='screen',
    )

    dvs_min_node = Node(
        package='vision_py',
        executable='dvs_minimal_record_node.py',
        output='screen',
        parameters=[{
            'port': dvs_port,
            'processing_strategy': processing_strategy,
            'filter_strength': filter_strength,
            'filter_min_component_pixels': filter_min_component_pixels,
            'approach_score_mode': approach_score_mode,
            'detect_min_circle_pixels': detect_min_circle_pixels,
            'video_output_path': video_output_path,
            'video_fps': video_fps,
            'video_scale': video_scale,
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
            lidar_bridge_node,
            dvs_min_node,
            core_node,
        ]
    )
