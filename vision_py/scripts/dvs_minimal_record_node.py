#!/usr/bin/env python3
import csv
import os
import threading
import time
from collections import deque
from typing import Deque, List, Optional, Tuple

import cv2
import numpy as np
import serial

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy

from std_msgs.msg import String
from geometry_msgs.msg import Twist

try:
    from dvs_event_processing import (
        BallDetector,
        EventFilter,
        compute_avoidance_command,
        filter_small_connected_components,
    )
except ModuleNotFoundError:
    from scripts.dvs_event_processing import (
        BallDetector,
        EventFilter,
        compute_avoidance_command,
        filter_small_connected_components,
    )


# (x, y, p, dt_us)
Event = Tuple[float, float, int, int]


class DvsMinimalRecordNode(Node):
    def __init__(self) -> None:
        super().__init__('dvs_minimal_record_node')

        # I/O and runtime
        self.declare_parameter('port', '/dev/ttyACM1')
        self.declare_parameter('baudrate', 115200)
        self.declare_parameter('publish_period_ms', 5.0)
        self.declare_parameter('serial_timeout_sec', 0.1)
        self.declare_parameter('max_events_per_batch', 4096)
        self.declare_parameter('skip_out_of_range', True)

        # Processing and output topics
        self.declare_parameter('processing_strategy', 'cluster')
        self.declare_parameter('filter_refractory_us', 350)
        self.declare_parameter('filter_flicker_window_us', 600)
        self.declare_parameter('filter_strength', 0.40)
        self.declare_parameter('filter_min_component_pixels', 10)
        self.declare_parameter('detect_min_events', 20)
        self.declare_parameter('detect_min_circle_pixels', 200)
        self.declare_parameter('approach_score_mode', 'merged')
        self.declare_parameter('avoid_confidence_threshold', 0.45)
        self.declare_parameter('avoid_approach_threshold', 8.0)
        self.declare_parameter('detection_topic', '/dvs/ball_detection')
        self.declare_parameter('avoid_cmd_topic', '/dvs/avoid_cmd')

        # Recording
        self.declare_parameter('video_output_path', '~/dvs_filtered_result.avi')
        self.declare_parameter('video_fps', 60.0)
        self.declare_parameter('video_scale', 6)
        self.declare_parameter('video_codec', 'MJPG')
        self.declare_parameter('draw_confidence_threshold', 0.15)
        self.declare_parameter('draw_circle_thickness', 2)
        self.declare_parameter('draw_circle_expand_px', 0)
        self.declare_parameter('processing_metrics_output_path', '')
        self.declare_parameter('trajectory_max_points', 300)

        self.port = self.get_parameter('port').get_parameter_value().string_value
        self.baudrate = self.get_parameter('baudrate').get_parameter_value().integer_value
        self.publish_period_ms = self.get_parameter('publish_period_ms').get_parameter_value().double_value
        self.serial_timeout_sec = self.get_parameter('serial_timeout_sec').get_parameter_value().double_value
        self.max_events_per_batch = self.get_parameter('max_events_per_batch').get_parameter_value().integer_value
        self.skip_out_of_range = self.get_parameter('skip_out_of_range').get_parameter_value().bool_value

        self.processing_strategy = self.get_parameter('processing_strategy').get_parameter_value().string_value
        self.filter_refractory_us = self.get_parameter('filter_refractory_us').get_parameter_value().integer_value
        self.filter_flicker_window_us = self.get_parameter('filter_flicker_window_us').get_parameter_value().integer_value
        self.filter_strength = self.get_parameter('filter_strength').get_parameter_value().double_value
        self.filter_min_component_pixels = self.get_parameter('filter_min_component_pixels').get_parameter_value().integer_value
        self.detect_min_events = self.get_parameter('detect_min_events').get_parameter_value().integer_value
        self.detect_min_circle_pixels = self.get_parameter('detect_min_circle_pixels').get_parameter_value().integer_value
        self.approach_score_mode = self.get_parameter('approach_score_mode').get_parameter_value().string_value
        self.avoid_confidence_threshold = self.get_parameter('avoid_confidence_threshold').get_parameter_value().double_value
        self.avoid_approach_threshold = self.get_parameter('avoid_approach_threshold').get_parameter_value().double_value
        self.detection_topic = self.get_parameter('detection_topic').get_parameter_value().string_value
        self.avoid_cmd_topic = self.get_parameter('avoid_cmd_topic').get_parameter_value().string_value

        self.video_output_path = self.get_parameter('video_output_path').get_parameter_value().string_value
        self.video_fps = self.get_parameter('video_fps').get_parameter_value().double_value
        self.video_scale = self.get_parameter('video_scale').get_parameter_value().integer_value
        self.video_codec = self.get_parameter('video_codec').get_parameter_value().string_value
        self.draw_confidence_threshold = self.get_parameter('draw_confidence_threshold').get_parameter_value().double_value
        self.draw_circle_thickness = self.get_parameter('draw_circle_thickness').get_parameter_value().integer_value
        self.draw_circle_expand_px = self.get_parameter('draw_circle_expand_px').get_parameter_value().integer_value
        self.processing_metrics_output_path = self.get_parameter('processing_metrics_output_path').get_parameter_value().string_value
        self.trajectory_max_points = self.get_parameter('trajectory_max_points').get_parameter_value().integer_value

        self._apply_filter_strength()

        qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.VOLATILE,
        )
        self.pub_detection = self.create_publisher(String, self.detection_topic, qos)
        self.pub_avoid = self.create_publisher(Twist, self.avoid_cmd_topic, qos)

        self._event_filter = EventFilter(
            width=128,
            height=128,
            refractory_us=int(self.filter_refractory_us),
            flicker_window_us=int(self.filter_flicker_window_us),
        )
        self._ball_detector = BallDetector(width=128, height=128)

        self._writer_raw = None
        self._writer_detect = None
        self._writer_traj = None
        self._metrics_file = None
        self._metrics_writer = None
        self._trajectory_points: Deque[Tuple[int, int]] = deque(maxlen=max(10, int(self.trajectory_max_points)))
        self._init_outputs()

        self._lock = threading.Lock()
        self._events: List[Event] = []
        self._batch_start_ns = 0
        self._running = True
        self._serial = None

        self._reader_thread = threading.Thread(target=self._serial_reader_loop, daemon=True)
        self._reader_thread.start()

        self._timer = self.create_timer(
            self.publish_period_ms / 1000.0,
            self._process_batch,
        )

        self.get_logger().info(
            f'DVS minimal record node started: port={self.port} baud={self.baudrate} '
            f'strategy={self.processing_strategy} filter_strength={self.filter_strength:.2f} '
            f'min_component_pixels={self.filter_min_component_pixels} '
            f'min_circle_pixels={self.detect_min_circle_pixels} '
            f'video_detect={self.video_output_path}'
        )

    def _apply_filter_strength(self) -> None:
        if self.filter_strength < 0.0:
            return

        s = max(0.0, min(1.0, float(self.filter_strength)))
        s2 = s * s
        self.filter_strength = s

        self.filter_refractory_us = int(80 + s2 * 12000)
        self.filter_flicker_window_us = int(120 + s2 * 20000)
        self.detect_min_events = int(8 + s2 * 260)
        self.avoid_confidence_threshold = 0.25 + s * 0.65
        self.avoid_approach_threshold = 2.0 + s2 * 80.0

    def _init_outputs(self) -> None:
        self.video_output_path = os.path.abspath(os.path.expanduser(self.video_output_path))
        path_root, path_ext = os.path.splitext(self.video_output_path)
        if not path_ext:
            path_ext = '.avi'

        raw_path = f'{path_root}_raw{path_ext}'
        detect_path = f'{path_root}_detect{path_ext}'
        trajectory_path = f'{path_root}_trajectory{path_ext}'

        if self.processing_metrics_output_path:
            metrics_path = os.path.abspath(os.path.expanduser(self.processing_metrics_output_path))
        else:
            metrics_path = f'{path_root}_timing.csv'

        video_dir = os.path.dirname(detect_path)
        if video_dir:
            os.makedirs(video_dir, exist_ok=True)

        metrics_dir = os.path.dirname(metrics_path)
        if metrics_dir:
            os.makedirs(metrics_dir, exist_ok=True)

        fourcc = cv2.VideoWriter.fourcc(*self.video_codec)
        side = 128 * max(1, int(self.video_scale))
        video_fps = float(max(1.0, self.video_fps))

        self._writer_raw = cv2.VideoWriter(
            raw_path,
            fourcc,
            video_fps,
            (side, side),
            True,
        )
        self._writer_detect = cv2.VideoWriter(
            detect_path,
            fourcc,
            video_fps,
            (side, side),
            True,
        )
        self._writer_traj = cv2.VideoWriter(
            trajectory_path,
            fourcc,
            video_fps,
            (side, side),
            True,
        )

        bad_writer = (
            self._writer_raw is None or not self._writer_raw.isOpened() or
            self._writer_detect is None or not self._writer_detect.isOpened() or
            self._writer_traj is None or not self._writer_traj.isOpened()
        )
        if bad_writer:
            self.get_logger().error(
                f'Failed to open one or more video writers: raw={raw_path}, detect={detect_path}, trajectory={trajectory_path}'
            )
            if self._writer_raw is not None:
                self._writer_raw.release()
            if self._writer_detect is not None:
                self._writer_detect.release()
            if self._writer_traj is not None:
                self._writer_traj.release()
            self._writer_raw = None
            self._writer_detect = None
            self._writer_traj = None
        else:
            self.get_logger().info(
                f'Video outputs: raw={raw_path}, detect={detect_path}, trajectory={trajectory_path}, fps={video_fps:.1f}'
            )

        try:
            self._metrics_file = open(metrics_path, 'w', newline='')
            self._metrics_writer = csv.writer(self._metrics_file)
            self._metrics_writer.writerow([
                'ros_time_ns',
                'batch_events',
                'filtered_events',
                'detected',
                'confidence',
                'filter_ms',
                'detect_ms',
                'avoid_ms',
                'write_ms',
                'total_ms',
                'note',
            ])
            self._metrics_file.flush()
            self.get_logger().info(f'Processing timing log: {metrics_path}')
        except Exception as e:
            self.get_logger().error(f'Failed to open timing log file {metrics_path}: {e}')
            self._metrics_file = None
            self._metrics_writer = None

    def destroy_node(self):
        self._running = False
        if self._reader_thread.is_alive():
            self._reader_thread.join(timeout=1.0)
        if self._serial is not None:
            try:
                self._serial.close()
            except Exception:
                pass
        if self._writer_raw is not None:
            self._writer_raw.release()
            self._writer_raw = None
        if self._writer_detect is not None:
            self._writer_detect.release()
            self._writer_detect = None
        if self._writer_traj is not None:
            self._writer_traj.release()
            self._writer_traj = None
        if self._metrics_file is not None:
            try:
                self._metrics_file.flush()
                self._metrics_file.close()
            except Exception:
                pass
            self._metrics_file = None
            self._metrics_writer = None
        super().destroy_node()

    @staticmethod
    def decode_event(buff: bytes) -> Tuple[int, int, int]:
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
                    timeout=self.serial_timeout_sec,
                )

                while self._running:
                    buff = self._serial.read(2)
                    if len(buff) != 2:
                        continue

                    now_ns = time.monotonic_ns()
                    x, y, p = self.decode_event(buff)

                    if self.skip_out_of_range and not (0 <= x < 128 and 0 <= y < 128):
                        continue

                    with self._lock:
                        if not self._events:
                            self._batch_start_ns = now_ns
                        dt_us = int((now_ns - self._batch_start_ns) / 1000)
                        self._events.append((float(x), float(y), int(p), dt_us))

                        if len(self._events) >= self.max_events_per_batch:
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

    def _process_batch(self) -> None:
        t_total_start = time.perf_counter_ns()
        with self._lock:
            if not self._events:
                return
            events = self._events
            self._events = []

        t_filter_start = time.perf_counter_ns()
        filtered = self._event_filter.filter_batch(events)
        filtered = filter_small_connected_components(
            filtered,
            int(self.filter_min_component_pixels),
        )
        t_filter_end = time.perf_counter_ns()

        result = None
        note = ''

        if len(filtered) < int(self.detect_min_events):
            note = f'below_min_events:{len(filtered)}'
            self._publish_detection_status(
                detected=False,
                confidence=0.0,
                center_x=-1.0,
                center_y=-1.0,
                radius=0.0,
                support_pixels=0,
                approach_score=0.0,
                strategy=self.processing_strategy,
                note=note,
            )

            t_detect_end = time.perf_counter_ns()

            t_avoid_start = time.perf_counter_ns()
            self.pub_avoid.publish(Twist())
            t_avoid_end = time.perf_counter_ns()

            t_write_start = time.perf_counter_ns()
            self._write_frames(events, filtered, None)
            t_write_end = time.perf_counter_ns()

            total_ms = (t_write_end - t_total_start) / 1e6
            self._log_processing_metrics(
                batch_events=len(events),
                filtered_events=len(filtered),
                detected=False,
                confidence=0.0,
                filter_ms=(t_filter_end - t_filter_start) / 1e6,
                detect_ms=(t_detect_end - t_filter_end) / 1e6,
                avoid_ms=(t_avoid_end - t_avoid_start) / 1e6,
                write_ms=(t_write_end - t_write_start) / 1e6,
                total_ms=total_ms,
                note=note,
            )
            return

        t_detect_start = time.perf_counter_ns()
        result = self._ball_detector.detect(
            filtered,
            self.processing_strategy,
            self.approach_score_mode,
        )

        if result.detected and int(result.support_pixels) < int(self.detect_min_circle_pixels):
            result.detected = False
            result.note = (
                f'{result.note} '
                f'min_circle_pixels_fail={result.support_pixels}<{self.detect_min_circle_pixels}'
            )

        t_detect_end = time.perf_counter_ns()

        self._publish_detection_status(
            detected=result.detected,
            confidence=result.confidence,
            center_x=result.center_x,
            center_y=result.center_y,
            radius=result.radius,
            support_pixels=result.support_pixels,
            approach_score=result.approach_score,
            strategy=result.strategy,
            note=result.note,
        )

        t_avoid_start = time.perf_counter_ns()
        vx, vy, vz, _ = compute_avoidance_command(
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
        t_avoid_end = time.perf_counter_ns()

        t_write_start = time.perf_counter_ns()
        self._write_frames(events, filtered, result)
        t_write_end = time.perf_counter_ns()

        total_ms = (t_write_end - t_total_start) / 1e6
        self._log_processing_metrics(
            batch_events=len(events),
            filtered_events=len(filtered),
            detected=bool(result.detected),
            confidence=float(result.confidence),
            filter_ms=(t_filter_end - t_filter_start) / 1e6,
            detect_ms=(t_detect_end - t_detect_start) / 1e6,
            avoid_ms=(t_avoid_end - t_avoid_start) / 1e6,
            write_ms=(t_write_end - t_write_start) / 1e6,
            total_ms=total_ms,
            note=result.note,
        )

    def _publish_detection_status(
        self,
        detected: bool,
        confidence: float,
        center_x: float,
        center_y: float,
        radius: float,
        support_pixels: int,
        approach_score: float,
        strategy: str,
        note: str,
    ) -> None:
        detect_time_ns = int(self.get_clock().now().nanoseconds)
        msg = String()
        msg.data = (
            '{'
            f'"detected": {str(bool(detected)).lower()}, '
            f'"detect_time_ns": {detect_time_ns}, '
            f'"confidence": {float(confidence):.3f}, '
            f'"center_x": {float(center_x):.2f}, '
            f'"center_y": {float(center_y):.2f}, '
            f'"radius": {float(radius):.2f}, '
            f'"support_pixels": {int(support_pixels)}, '
            f'"approach_score": {float(approach_score):.2f}, '
            f'"strategy": "{strategy}", '
            f'"note": "{note}"'
            '}'
        )
        self.pub_detection.publish(msg)

    def _render_events_canvas(self, events: List[Event]) -> np.ndarray:
        canvas = np.zeros((128, 128, 3), dtype=np.uint8)
        for x_f, y_f, p_i, _ in events:
            x = int(x_f)
            y = int(y_f)
            if not (0 <= x < 128 and 0 <= y < 128):
                continue
            if int(p_i) > 0:
                canvas[y, x, 1] = 255
            else:
                canvas[y, x, 0] = 255
        return canvas

    def _write_frames(self, raw_events: List[Event], filtered_events: List[Event], result) -> None:
        if self._writer_raw is None or self._writer_detect is None or self._writer_traj is None:
            return

        raw_canvas = self._render_events_canvas(raw_events)
        detect_canvas = self._render_events_canvas(filtered_events)
        trajectory_canvas = np.zeros((128, 128, 3), dtype=np.uint8)

        if (
            result is not None
            and result.detected
            and result.confidence >= float(self.draw_confidence_threshold)
        ):
            cx = int(max(0, min(127, result.center_x)))
            cy = int(max(0, min(127, result.center_y)))
            rr = int(max(3, min(62, result.radius + float(self.draw_circle_expand_px))))
            thickness = max(1, int(self.draw_circle_thickness))
            cv2.circle(detect_canvas, (cx, cy), rr, (0, 0, 255), thickness)
            cv2.circle(detect_canvas, (cx, cy), 2, (0, 0, 255), -1)
            self._trajectory_points.append((cx, cy))

        if len(self._trajectory_points) > 0:
            for i in range(1, len(self._trajectory_points)):
                p0 = self._trajectory_points[i - 1]
                p1 = self._trajectory_points[i]
                cv2.line(trajectory_canvas, p0, p1, (0, 255, 255), 1)
            px, py = self._trajectory_points[-1]
            cv2.circle(trajectory_canvas, (px, py), 2, (0, 0, 255), -1)

        side = 128 * max(1, int(self.video_scale))
        raw_frame = cv2.resize(raw_canvas, (side, side), interpolation=cv2.INTER_NEAREST)
        detect_frame = cv2.resize(detect_canvas, (side, side), interpolation=cv2.INTER_NEAREST)
        trajectory_frame = cv2.resize(trajectory_canvas, (side, side), interpolation=cv2.INTER_NEAREST)
        self._writer_raw.write(raw_frame)
        self._writer_detect.write(detect_frame)
        self._writer_traj.write(trajectory_frame)

    def _log_processing_metrics(
        self,
        batch_events: int,
        filtered_events: int,
        detected: bool,
        confidence: float,
        filter_ms: float,
        detect_ms: float,
        avoid_ms: float,
        write_ms: float,
        total_ms: float,
        note: str,
    ) -> None:
        if self._metrics_writer is None or self._metrics_file is None:
            return

        self._metrics_writer.writerow([
            int(self.get_clock().now().nanoseconds),
            int(batch_events),
            int(filtered_events),
            int(bool(detected)),
            f'{float(confidence):.4f}',
            f'{float(filter_ms):.3f}',
            f'{float(detect_ms):.3f}',
            f'{float(avoid_ms):.3f}',
            f'{float(write_ms):.3f}',
            f'{float(total_ms):.3f}',
            str(note).replace('\n', ' '),
        ])
        self._metrics_file.flush()


def main(args=None) -> None:
    rclpy.init(args=args)
    node = DvsMinimalRecordNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
