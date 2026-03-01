import launch
import launch_ros.actions

def generate_launch_description():
    return launch.LaunchDescription([
        launch_ros.actions.Node(
            package='ros2_tools',
            executable='lidar_data_node',
        ),
        launch_ros.actions.Node(
            package='vision',
            executable='vision_node.py',
        ),
        # 使用重构后的 core_2026 包
        launch_ros.actions.Node(
            package='core_2026',
            executable='quad_node',
        )
    ])
