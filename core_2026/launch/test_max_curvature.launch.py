from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    """Launch file for max curvature test"""
    target_speed = LaunchConfiguration('target_speed')
    accel_time = LaunchConfiguration('accel_time')
    target_curvature = LaunchConfiguration('target_curvature')
    start_bridge = LaunchConfiguration('start_bridge')
    
    return LaunchDescription([
        DeclareLaunchArgument('target_speed', default_value='0.5'),
        DeclareLaunchArgument('accel_time', default_value='3.0'),
        DeclareLaunchArgument('target_curvature', default_value='2.5'),
        DeclareLaunchArgument('start_bridge', default_value='true'),

        # Smart Car Bridge (transport-agnostic host interface)
        Node(
            package='smart_car_bridge',
            executable='smart_car_bridge_node',
            name='smart_car_bridge_node',
            condition=IfCondition(start_bridge),
            parameters=[{
                'platform_mode': 'car',
                'platform_target_topic': '/platform/target',
                'target_system': 1,
                'target_component': 0,
                'control_rate_hz': 50.0,
                'control_input_timeout_ms': 500,
                'max_speed_mps': 1.5,
                'max_curvature': 2.5,
            }],
            output='screen',
        ),
        
        # MAVROS Node (Serial communication)
        
        # Max Curvature Test (Circular motion test)
        Node(
            package='core_2026',
            executable='test_max_curvature',
            name='test_max_curvature',
            parameters=[{
                'target_speed': target_speed,          # 目标速度 (m/s)
                'accel_time': accel_time,              # 加速时间 (s)
                'target_curvature': target_curvature,  # 目标曲率 (1/m)
            }],
            output='screen',
        ),
    ])
