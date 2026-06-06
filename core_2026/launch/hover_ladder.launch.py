from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    fcu_url = LaunchConfiguration('fcu_url')
    start_mavros = LaunchConfiguration('start_mavros')

    return LaunchDescription([
        DeclareLaunchArgument(
            'fcu_url',
            default_value='serial:///dev/ttyACM0:57600',
            description='MAVROS FCU serial URL.',
        ),
        DeclareLaunchArgument(
            'start_mavros',
            default_value='true',
            description='Start mavros_node before quad_node.',
        ),
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
            package='core_2026',
            executable='quad_node',
            name='quad_node',
        ),
    ])
