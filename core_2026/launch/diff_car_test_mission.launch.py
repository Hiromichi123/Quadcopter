from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('cmd_vel_topic', default_value='/mavros/setpoint_velocity/cmd_vel'),
        DeclareLaunchArgument('lidar_topic', default_value='lidar_data'),
        DeclareLaunchArgument('enable_px4_preflight', default_value='true'),
        DeclareLaunchArgument('offboard_mode', default_value='OFFBOARD'),
        DeclareLaunchArgument('target_distance_m', default_value='1.0'),
        DeclareLaunchArgument('forward_speed_mps', default_value='0.25'),
        DeclareLaunchArgument('turn_rate_radps', default_value='0.45'),
        Node(
            package='core_2026',
            executable='diff_car_test_mission',
            name='diff_car_test_mission',
            output='screen',
            parameters=[{
                'cmd_vel_topic': LaunchConfiguration('cmd_vel_topic'),
                'lidar_topic': LaunchConfiguration('lidar_topic'),
                'enable_px4_preflight': LaunchConfiguration('enable_px4_preflight'),
                'offboard_mode': LaunchConfiguration('offboard_mode'),
                'target_distance_m': LaunchConfiguration('target_distance_m'),
                'forward_speed_mps': LaunchConfiguration('forward_speed_mps'),
                'turn_rate_radps': LaunchConfiguration('turn_rate_radps'),
            }],
        ),
    ])
