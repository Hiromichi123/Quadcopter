#!/usr/bin/env python3
import select
import sys
import termios
import tty

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy

from geometry_msgs.msg import Twist


class DvsAvoidKeyNode(Node):
    def __init__(self) -> None:
        super().__init__('dvs_avoid_key_node')

        self.declare_parameter('topic', '/dvs/avoid_cmd')
        self.declare_parameter('cmd_x', -0.45)
        self.declare_parameter('cmd_y', 0.35)
        self.declare_parameter('cmd_z', 0.20)

        topic = self.get_parameter('topic').get_parameter_value().string_value
        self.cmd_x = float(self.get_parameter('cmd_x').get_parameter_value().double_value)
        self.cmd_y = float(self.get_parameter('cmd_y').get_parameter_value().double_value)
        self.cmd_z = float(self.get_parameter('cmd_z').get_parameter_value().double_value)

        qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.VOLATILE,
        )
        self.pub_avoid = self.create_publisher(Twist, topic, qos)

        self.get_logger().info(
            'DVS avoid key node started. Press SPACE to publish once, press q to quit. '
            f'topic={topic}, cmd=({self.cmd_x:.2f}, {self.cmd_y:.2f}, {self.cmd_z:.2f})'
        )

    def publish_once(self) -> None:
        msg = Twist()
        msg.linear.x = self.cmd_x
        msg.linear.y = self.cmd_y
        msg.linear.z = self.cmd_z
        self.pub_avoid.publish(msg)
        self.get_logger().info(
            f'Published avoid cmd once: x={msg.linear.x:.2f}, y={msg.linear.y:.2f}, z={msg.linear.z:.2f}'
        )


def _read_key_nonblocking() -> str:
    ready, _, _ = select.select([sys.stdin], [], [], 0.05)
    if ready:
        return sys.stdin.read(1)
    return ''


def main(args=None) -> None:
    rclpy.init(args=args)
    node = DvsAvoidKeyNode()

    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)

    try:
        tty.setcbreak(fd)
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=0.01)
            ch = _read_key_nonblocking()
            if not ch:
                continue
            if ch == ' ':
                node.publish_once()
            elif ch.lower() == 'q':
                node.get_logger().info('Quit requested, shutting down key node.')
                break
    except KeyboardInterrupt:
        pass
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
