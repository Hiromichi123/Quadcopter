#!/usr/bin/env python3
import threading
import time
from typing import List, Tuple

import cv2
import numpy as np
import serial

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy

from std_msgs.msg import Header
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from sensor_msgs.msg import PointCloud2, PointField
from sensor_msgs_py import point_cloud2

try:
    # Works when installed as plain scripts in lib/vision_py.
    from dvs_event_processing import (
        BallDetector,
        EventFilter,
        compute_avoidance_command,
        filter_small_connected_components,
    )
except ModuleNotFoundError:
    # Fallback for package-style execution.
    from scripts.dvs_event_processing import (
        BallDetector,
        EventFilter,
        compute_avoidance_command,
        filter_small_connected_components,
    )

# (x, y, p, dt_us)
Event = Tuple[float, float, int, int]

class DvsRawSerialNode(Node):
    def __init__(self) -> None:
        super().__init__('dvs_raw_serial_node')

        self.declare_parameter('port', '/dev/ttyACM0')
        self.declare_parameter('baudrate', 115200)
        self.declare_parameter('frame_id', 'dvs_link')
        self.declare_parameter('publish_topic', '/dvs/events')
        self.declare_parameter('publish_filtered_topic', '/dvs/events_filtered')
        self.declare_parameter('detection_topic', '/dvs/ball_detection')
        self.declare_parameter('avoid_cmd_topic', '/dvs/avoid_cmd')
        self.declare_parameter('publish_period_ms', 5.0)
        self.declare_parameter('serial_timeout_sec', 0.1)
        self.declare_parameter('max_events_per_batch', 4096)
        self.declare_parameter('skip_out_of_range', True)
        self.declare_parameter('processing_enabled', True)
        self.declare_parameter('processing_strategy', 'density')
        self.declare_parameter('filter_refractory_us', 350)
        self.declare_parameter('filter_flicker_window_us', 600)
        self.declare_parameter('filter_strength', -1.0)
        self.declare_parameter('filter_min_component_pixels', 12)
        self.declare_parameter('detect_min_events', 20)
        self.declare_parameter('avoid_confidence_threshold', 0.45)
        self.declare_parameter('avoid_approach_threshold', 8.0)
        self.declare_parameter('visualize_enabled', False)
        self.declare_parameter('visualize_window_name', 'DVS Detection')
        self.declare_parameter('visualize_scale', 6)
        self.declare_parameter('visualize_use_filtered', True)
        self.declare_parameter('visualize_compare_mode', False)
        self.declare_parameter('visualize_circle_thickness', 1)
        self.declare_parameter('visualize_circle_expand_px', 0)
        self.declare_parameter('visualize_confidence_threshold', 0.15)

        self.port = self.get_parameter('port').get_parameter_value().string_value
        self.baudrate = self.get_parameter('baudrate').get_parameter_value().integer_value
        self.frame_id = self.get_parameter('frame_id').get_parameter_value().string_value
        self.publish_topic = self.get_parameter('publish_topic').get_parameter_value().string_value
        self.publish_filtered_topic = self.get_parameter('publish_filtered_topic').get_parameter_value().string_value
        self.detection_topic = self.get_parameter('detection_topic').get_parameter_value().string_value
        self.avoid_cmd_topic = self.get_parameter('avoid_cmd_topic').get_parameter_value().string_value
        self.publish_period_ms = self.get_parameter('publish_period_ms').get_parameter_value().double_value
        self.serial_timeout_sec = self.get_parameter('serial_timeout_sec').get_parameter_value().double_value
        self.max_events_per_batch = self.get_parameter('max_events_per_batch').get_parameter_value().integer_value
        self.skip_out_of_range = self.get_parameter('skip_out_of_range').get_parameter_value().bool_value
        self.processing_enabled = self.get_parameter('processing_enabled').get_parameter_value().bool_value
        self.processing_strategy = self.get_parameter('processing_strategy').get_parameter_value().string_value
        self.filter_refractory_us = self.get_parameter('filter_refractory_us').get_parameter_value().integer_value
        self.filter_flicker_window_us = self.get_parameter('filter_flicker_window_us').get_parameter_value().integer_value
        self.filter_strength = self.get_parameter('filter_strength').get_parameter_value().double_value
        self.filter_min_component_pixels = self.get_parameter('filter_min_component_pixels').get_parameter_value().integer_value
        self.detect_min_events = self.get_parameter('detect_min_events').get_parameter_value().integer_value
        self.avoid_confidence_threshold = self.get_parameter('avoid_confidence_threshold').get_parameter_value().double_value
        self.avoid_approach_threshold = self.get_parameter('avoid_approach_threshold').get_parameter_value().double_value
        self.visualize_enabled = self.get_parameter('visualize_enabled').get_parameter_value().bool_value
        self.visualize_window_name = self.get_parameter('visualize_window_name').get_parameter_value().string_value
        self.visualize_scale = self.get_parameter('visualize_scale').get_parameter_value().integer_value
        self.visualize_use_filtered = self.get_parameter('visualize_use_filtered').get_parameter_value().bool_value
        self.visualize_compare_mode = self.get_parameter('visualize_compare_mode').get_parameter_value().bool_value
        self.visualize_circle_thickness = self.get_parameter('visualize_circle_thickness').get_parameter_value().integer_value
        self.visualize_circle_expand_px = self.get_parameter('visualize_circle_expand_px').get_parameter_value().integer_value
        self.visualize_confidence_threshold = self.get_parameter('visualize_confidence_threshold').get_parameter_value().double_value

        self._apply_filter_strength()

        qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.VOLATILE,
        )

        self.pub = self.create_publisher(PointCloud2, self.publish_topic, qos)
        self.pub_filtered = self.create_publisher(PointCloud2, self.publish_filtered_topic, qos)
        self.pub_detection = self.create_publisher(String, self.detection_topic, qos)
        self.pub_avoid = self.create_publisher(Twist, self.avoid_cmd_topic, qos)

        self._event_filter = EventFilter(
            width=128,
            height=128,
            refractory_us=int(self.filter_refractory_us),
            flicker_window_us=int(self.filter_flicker_window_us),
        )
        self._ball_detector = BallDetector(width=128, height=128)
        self._vis_ok = bool(self.visualize_enabled)
        self._last_avoid_reason = 'idle'

        if self._vis_ok:
            try:
                cv2.namedWindow(self.visualize_window_name, cv2.WINDOW_NORMAL)
            except Exception as e:
                self.get_logger().warn(f'Visualization disabled: {e}')
                self._vis_ok = False

        self._lock = threading.Lock()
        self._events: List[Event] = []
        self._batch_start_ns = 0
        self._running = True
        self._serial = None

        self._reader_thread = threading.Thread(target=self._serial_reader_loop, daemon=True)
        self._reader_thread.start()

        self._timer = self.create_timer(
            self.publish_period_ms / 1000.0,
            self._publish_batch
        )

        self.get_logger().info(
            f'Started DVS raw serial node on {self.port} @ {self.baudrate}, '
            f'publishing {self.publish_topic} every {self.publish_period_ms:.2f} ms, '
            f'processing={self.processing_enabled}, strategy={self.processing_strategy}, '
            f'visualize={self._vis_ok}, compare_mode={self.visualize_compare_mode}, '
            f'filter_strength={self.filter_strength:.2f}, refractory_us={self.filter_refractory_us}, '
            f'flicker_us={self.filter_flicker_window_us}, min_events={self.detect_min_events}, '
            f'min_component_pixels={self.filter_min_component_pixels}'
        )

    def _apply_filter_strength(self) -> None:
        """Apply a single tuning knob for overall filtering and trigger strictness.

        filter_strength range:
        - < 0: disabled, keep explicit parameters as-is.
        - 0..1: weak to strong filtering.
        """
        if self.filter_strength < 0.0:
            return

        s = max(0.0, min(1.0, float(self.filter_strength)))
        self.filter_strength = s
        s2 = s * s

        # Time-domain event filtering. Expanded range so s=1 keeps only intense changes.
        self.filter_refractory_us = int(80 + s2 * 12000)         # 80 .. 12080 us
        self.filter_flicker_window_us = int(120 + s2 * 20000)    # 120 .. 20120 us

        # Detection and avoidance trigger strictness.
        self.detect_min_events = int(8 + s2 * 260)               # 8 .. 268
        self.avoid_confidence_threshold = 0.25 + s * 0.65        # 0.25 .. 0.90
        self.avoid_approach_threshold = 2.0 + s2 * 80.0          # 2.0 .. 82.0

    def destroy_node(self):
        self._running = False
        if self._reader_thread.is_alive():
            self._reader_thread.join(timeout=1.0)
        if self._serial is not None:
            try:
                self._serial.close()
            except Exception:
                pass
        if self._vis_ok:
            try:
                cv2.destroyWindow(self.visualize_window_name)
            except Exception:
                pass
        super().destroy_node()

    @staticmethod
    def decode_event(buff: bytes) -> Tuple[int, int, int]:
        """
        Raw protocol from mcu_point_hid.py:
            dx = buff[0] & 0x7f
            dy = ((buff[1] & 0x3f) << 1) + ((buff[0] & 0x80) >> 7)
            p  = buff[1] >> 6
        """
        b0 = buff[0]
        b1 = buff[1]

        dx = b0 & 0x7F
        dy = ((b1 & 0x3F) << 1) + ((b0 & 0x80) >> 7)
        p = b1 >> 6
        return dx, dy, p

    def _serial_reader_loop(self) -> None:
        while self._running:
            try:
                self.get_logger().info(f'Opening serial port: {self.port}')
                self._serial = serial.Serial(
                    self.port,
                    self.baudrate,
                    timeout=self.serial_timeout_sec
                )

                while self._running:
                    buff = self._serial.read(2)
                    if len(buff) != 2:
                        continue

                    now_ns = time.monotonic_ns()
                    x, y, p = self.decode_event(buff)

                    if self.skip_out_of_range:
                        if not (0 <= x < 128 and 0 <= y < 128):
                            continue

                    with self._lock:
                        if not self._events:
                            self._batch_start_ns = now_ns

                        dt_us = int((now_ns - self._batch_start_ns) / 1000)
                        self._events.append((float(x), float(y), int(p), dt_us))

                        # 防止极端情况下批次无限膨胀
                        if len(self._events) >= self.max_events_per_batch:
                            # 留给定时器去发，不在串口线程里直接发
                            pass

            except serial.SerialException as e:
                self.get_logger().error(f'Serial error: {e}')
                time.sleep(1.0)
            except Exception as e:
                self.get_logger().error(f'Unexpected reader error: {e}')
                time.sleep(1.0)
            finally:
                if self._serial is not None:
                    try:
                        self._serial.close()
                    except Exception:
                        pass
                    self._serial = None

    def _publish_batch(self) -> None:
        with self._lock:
            if not self._events:
                if self._vis_ok:
                    # Keep visualization alive even when this period has no events.
                    self._render_visualization([], [], None)
                return
            events = self._events
            self._events = []
            batch_start_ns = self._batch_start_ns

        self._publish_events(events, self.pub)

        if not self.processing_enabled:
            if self._vis_ok:
                self._render_visualization(events, events, None)
            return

        filtered = self._event_filter.filter_batch(events)
        filtered = filter_small_connected_components(
            filtered,
            int(self.filter_min_component_pixels),
        )
        if filtered:
            self._publish_events(filtered, self.pub_filtered)

        if len(filtered) < int(self.detect_min_events):
            self._publish_detection_status(
                detected=False,
                confidence=0.0,
                center_x=-1.0,
                center_y=-1.0,
                radius=0.0,
                approach_score=0.0,
                strategy=self.processing_strategy,
                note=f'below_min_events:{len(filtered)}',
            )
            self.pub_avoid.publish(Twist())
            self._last_avoid_reason = 'below_min_events'
            if self._vis_ok:
                self._render_visualization(events, filtered, None)
            return

        result = self._ball_detector.detect(filtered, self.processing_strategy)
        self._publish_detection_status(
            detected=result.detected,
            confidence=result.confidence,
            center_x=result.center_x,
            center_y=result.center_y,
            radius=result.radius,
            approach_score=result.approach_score,
            strategy=result.strategy,
            note=result.note,
        )

        vx, vy, vz, reason = compute_avoidance_command(
            result,
            frame_width=128,
            frame_height=128,
            trigger_approach=float(self.avoid_approach_threshold),
            trigger_confidence=float(self.avoid_confidence_threshold),
        )
        cmd = Twist()
        cmd.linear.x = float(vx)
        cmd.linear.y = float(vy)
        cmd.linear.z = float(vz)
        self.pub_avoid.publish(cmd)
        self._last_avoid_reason = reason

        if self._vis_ok:
            self._render_visualization(events, filtered, result)

        self.get_logger().debug(
            f'Published {len(events)} raw, {len(filtered)} filtered, '
            f'detected={result.detected}, avoid={reason}, batch_start_ns={batch_start_ns}'
        )

    def _publish_events(self, events: List[Event], publisher) -> None:
        header = Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = self.frame_id

        fields = [
            PointField(name='x', offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name='y', offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name='p', offset=8, datatype=PointField.UINT8, count=1),
            PointField(name='dt_us', offset=9, datatype=PointField.UINT32, count=1),
        ]

        # sensor_msgs_py.point_cloud2.create_cloud 对 tuple/list 自动打包。
        # 为了兼容 PointField 偏移，point step 按 fields 自动处理。
        msg = point_cloud2.create_cloud(header, fields, events)
        publisher.publish(msg)

    def _publish_detection_status(
        self,
        detected: bool,
        confidence: float,
        center_x: float,
        center_y: float,
        radius: float,
        approach_score: float,
        strategy: str,
        note: str,
    ) -> None:
        msg = String()
        msg.data = (
            '{'
            f'"detected": {str(bool(detected)).lower()}, '
            f'"confidence": {float(confidence):.3f}, '
            f'"center_x": {float(center_x):.2f}, '
            f'"center_y": {float(center_y):.2f}, '
            f'"radius": {float(radius):.2f}, '
            f'"approach_score": {float(approach_score):.2f}, '
            f'"strategy": "{strategy}", '
            f'"note": "{note}"'
            '}'
        )
        self.pub_detection.publish(msg)

    def _render_visualization(self, raw_events: List[Event], filtered_events: List[Event], result) -> None:
        if not self._vis_ok:
            return

        try:
            scale = max(1, int(self.visualize_scale))
            raw_canvas = self._events_to_canvas(raw_events)
            filtered_canvas = self._events_to_canvas(filtered_events)

            if result is not None and result.confidence >= float(self.visualize_confidence_threshold):
                # Draw only one best circle for this frame (right panel), with thicker and expanded ring.
                cx = int(max(0, min(127, result.center_x)))
                cy = int(max(0, min(127, result.center_y)))
                rr = int(max(3, min(62, result.radius + float(self.visualize_circle_expand_px))))
                thickness = max(1, int(self.visualize_circle_thickness))
                cv2.circle(filtered_canvas, (cx, cy), rr, (0, 0, 255), thickness)
                cv2.circle(filtered_canvas, (cx, cy), 2, (0, 0, 255), -1)

            if self.visualize_compare_mode:
                raw_enlarged = cv2.resize(raw_canvas, (128 * scale, 128 * scale), interpolation=cv2.INTER_NEAREST)
                filtered_enlarged = cv2.resize(filtered_canvas, (128 * scale, 128 * scale), interpolation=cv2.INTER_NEAREST)

                cv2.putText(raw_enlarged, f'RAW n={len(raw_events)}', (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)
                cv2.putText(filtered_enlarged, f'FILTERED+RESULT n={len(filtered_events)}', (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)

                enlarged = np.concatenate([raw_enlarged, filtered_enlarged], axis=1)
                cv2.putText(
                    enlarged,
                    f'strategy={self.processing_strategy} avoid={self._last_avoid_reason}',
                    (10, 45),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (255, 255, 255),
                    1,
                    cv2.LINE_AA,
                )
            else:
                vis_canvas = filtered_canvas if self.visualize_use_filtered else raw_canvas
                enlarged = cv2.resize(vis_canvas, (128 * scale, 128 * scale), interpolation=cv2.INTER_NEAREST)
                text = (
                    f'strategy={self.processing_strategy} raw={len(raw_events)} '
                    f'flt={len(filtered_events)} avoid={self._last_avoid_reason}'
                )
                cv2.putText(
                    enlarged,
                    text,
                    (10, 20),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (255, 255, 255),
                    1,
                    cv2.LINE_AA,
                )

            if result is not None:
                line2 = f'det={result.detected} conf={result.confidence:.2f} app={result.approach_score:.2f}'
            else:
                line2 = 'det=false conf=0.00 app=0.00 (NO DET)'
            cv2.putText(
                enlarged,
                line2,
                (10, 65 if self.visualize_compare_mode else 40),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (255, 255, 255),
                1,
                cv2.LINE_AA,
            )

            cv2.imshow(self.visualize_window_name, enlarged)
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q') or key == 27:
                self._vis_ok = False
                cv2.destroyWindow(self.visualize_window_name)
                self.get_logger().info('Visualization window closed by user input.')
        except Exception as e:
            self.get_logger().warn(f'Visualization disabled due to runtime error: {e}')
            self._vis_ok = False

    def _events_to_canvas(self, events: List[Event]) -> np.ndarray:
        canvas = np.zeros((128, 128, 3), dtype=np.uint8)
        for x_f, y_f, p_i, _ in events:
            x = int(x_f)
            y = int(y_f)
            if not (0 <= x < 128 and 0 <= y < 128):
                continue

            if int(p_i) > 0:
                # ON polarity in green.
                canvas[y, x, 1] = 255
            else:
                # OFF polarity in blue.
                canvas[y, x, 0] = 255
        return canvas


def main(args=None) -> None:
    rclpy.init(args=args)
    node = DvsRawSerialNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()