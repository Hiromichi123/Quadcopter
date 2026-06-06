import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    """Launch MID360, Point-LIO, and UDP pose forwarding."""
    real_robot_odom_topic = LaunchConfiguration('real_robot_odom_topic')
    udp_target_ip = LaunchConfiguration('udp_target_ip')
    udp_target_port = LaunchConfiguration('udp_target_port')
    rviz = LaunchConfiguration('rviz')

    launch_args = [
        DeclareLaunchArgument('real_robot_odom_topic', default_value='/aft_mapped_to_init'),
        DeclareLaunchArgument('udp_target_ip', default_value='192.168.0.3'),
        DeclareLaunchArgument('udp_target_port', default_value='9005'),
        DeclareLaunchArgument('rviz', default_value='False'),
    ]

    livox_ros_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(
                get_package_share_directory('livox_ros_driver2'),
                'launch_ROS2',
                'msg_MID360_launch.py',
            )
        ])
    )

    base_to_livox_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='base_to_livox_tf',
        arguments=['0', '0', '0', '0', '0', '0', '1', 'base_link', 'livox_frame'],
        output='screen',
    )

    point_lio = TimerAction(
        period=5.0,
        actions=[
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource([
                    PathJoinSubstitution([
                        FindPackageShare('point_lio'),
                        'launch',
                        'point_lio.launch.py',
                    ])
                ]),
                launch_arguments={
                    'rviz': rviz,
                    'udp_odom_topic': real_robot_odom_topic,
                    'udp_target_ip': udp_target_ip,
                    'udp_target_port': udp_target_port,
                }.items(),
            )
        ],
    )

    return LaunchDescription(
        launch_args + [
            livox_ros_driver,
            base_to_livox_tf,
            point_lio,
        ]
    )
