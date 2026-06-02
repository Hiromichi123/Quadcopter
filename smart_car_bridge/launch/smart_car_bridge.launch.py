from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    fcu_url = LaunchConfiguration('fcu_url')
    platform_mode = LaunchConfiguration('platform_mode')
    start_mavros = LaunchConfiguration('start_mavros')

    return LaunchDescription([
        DeclareLaunchArgument('fcu_url', default_value='serial:///dev/ttyUSB0:115200'),
        DeclareLaunchArgument('platform_mode', default_value='car'),
        DeclareLaunchArgument('start_mavros', default_value='true'),
        Node(
            package='mavros',
            executable='mavros_node',
            condition=IfCondition(start_mavros),
            parameters=[{
                'fcu_url': fcu_url,
                'tgt_system': 1,
                'tgt_component': 1,
                'fcu_protocol': 'v2.0',
            }],
        ),
        Node(
            package='smart_car_bridge',
            executable='smart_car_bridge_node',
            name='smart_car_bridge_node',
            parameters=[{
                'platform_mode': platform_mode,
                'mavlink_source_topic': '/uas1/mavlink_source',
                'mavlink_sink_topic': '/uas1/mavlink_sink',
                'platform_target_topic': '/platform/target',
                'flight_setpoint_topic': '/mavros/setpoint_position/local',
                'target_system': 1,
                'target_component': 0,
                'host_system': 255,
                'host_component': 191,
                'control_rate_hz': 20.0,
                'control_input_timeout_ms': 150,
                'max_speed_mps': 1.5,
                'max_curvature': 2.5,
            }],
        ),
    ])
