from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
import os
import launch
from launch import LaunchDescription
from launch.actions import TimerAction, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
    fcu_url = LaunchConfiguration('fcu_url')
    dvs_port = LaunchConfiguration('dvs_port')
    real_robot_odom_topic = LaunchConfiguration('real_robot_odom_topic')

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
        DeclareLaunchArgument('real_robot_odom_topic', default_value='/aft_mapped_to_init'),
        DeclareLaunchArgument('processing_strategy', default_value='cluster'),
        DeclareLaunchArgument('filter_strength', default_value='0.40'),
        DeclareLaunchArgument('filter_min_component_pixels', default_value='10'),
        DeclareLaunchArgument('approach_score_mode', default_value='merged'),
        DeclareLaunchArgument('detect_min_circle_pixels', default_value='200'),
        DeclareLaunchArgument('video_output_path', default_value='~/dvs_filtered_result.avi'),
        DeclareLaunchArgument('video_fps', default_value='60.0'),
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

    livox_ros_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(get_package_share_directory('livox_ros_driver2'), 'launch_ROS2', 'msg_MID360_launch.py')
        ])
    )

    tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="base_to_livox_tf",
        arguments=["0", "0", "0", "0", "0", "0", "1", "base_link", "livox_frame"]
    )

    slam = TimerAction(
        period=5.0,  # 延迟 10s 启动 PointLIO
        actions=[
                IncludeLaunchDescription(
                PythonLaunchDescriptionSource([
                    PathJoinSubstitution([
                        FindPackageShare("point_lio"),
                        "launch",
                        "point_lio.launch.py"
                    ])
                ]),
                launch_arguments={"rviz": "False"}.items(),
            )
        ]
    )
    
    ros2_tools_nodes = [
        Node(package='ros2_tools', 
             executable='lidar_data_node',
             parameters=[{
                'use_simulation': False, # 仿真开关
                'simulation_odom_topic': '/absolute_pose', # gazebo的里程计话题
                'real_robot_odom_topic': real_robot_odom_topic # PointLIO的里程计话题
             }]),
        Node(
            package='ros2_tools',
            executable='lidar_to_px4_bridge',
            parameters=[{
                'real_robot_odom_topic': real_robot_odom_topic
            }]
        )
    ]

    dvs_min_node = Node(
        package='vision_py',
        executable='dvs_minimal_record_node',
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
            livox_ros_driver,
            tf,
            slam,
            *ros2_tools_nodes,
            dvs_min_node,
            core_node,
        ]
    )
