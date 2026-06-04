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
    start_mavros = LaunchConfiguration('start_mavros')
    
    return LaunchDescription([
        DeclareLaunchArgument('target_speed', default_value='0.5'),
        DeclareLaunchArgument('accel_time', default_value='3.0'),
        DeclareLaunchArgument('target_curvature', default_value='2.5'),
        DeclareLaunchArgument('start_bridge', default_value='true'),
        DeclareLaunchArgument('start_mavros', default_value='true'),

        # Smart Car Bridge (MAVLink interface)
        Node(
            package='smart_car_bridge',
            executable='smart_car_bridge_node',
            name='smart_car_bridge_node',
            condition=IfCondition(start_bridge),
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
        ),
        
        # MAVROS Node (Serial communication)
        Node(
            package='mavros',
            executable='mavros_node',
            namespace='mavros',
            condition=IfCondition(start_mavros),
            parameters=[{
                'fcu_url': 'serial:///dev/ttyS0:460800',
                'tgt_system': 1,
                'tgt_component': 1,
                'fcu_protocol': 'v2.0',
            }],
            output='screen',
        ),
        
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
