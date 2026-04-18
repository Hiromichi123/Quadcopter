#!/usr/bin/env python3
from dataclasses import dataclass
from typing import Dict, List, Tuple

# Event tuple format from dvs_raw_serial_node:
# (x, y, p, dt_us)
Event = Tuple[float, float, int, int]


@dataclass
class DetectionResult:
    detected: bool
    confidence: float
    center_x: float
    center_y: float
    radius: float
    approach_score: float
    strategy: str
    note: str


class EventFilter:
    """Low-latency event filter with refractory and flicker suppression."""

    def __init__(self, width: int = 128, height: int = 128,
                 refractory_us: int = 350, flicker_window_us: int = 600) -> None:
        self.width = width
        self.height = height
        self.refractory_us = refractory_us
        self.flicker_window_us = flicker_window_us

        n = width * height
        self._last_ts_us = [-10**12] * n
        self._last_p = [-1] * n

    def _idx(self, x: int, y: int) -> int:
        return y * self.width + x

    def filter_batch(self, events: List[Event]) -> List[Event]:
        filtered: List[Event] = []
        for x_f, y_f, p_i, t_i in events:
            x = int(x_f)
            y = int(y_f)
            p = int(p_i)
            t = int(t_i)

            if x < 0 or y < 0 or x >= self.width or y >= self.height:
                continue

            idx = self._idx(x, y)
            dt = t - self._last_ts_us[idx]

            # Refractory filtering: ignore repeated triggers at one pixel.
            if dt >= 0 and dt < self.refractory_us:
                continue

            # Polarity flip in a tiny window is often sensor flicker.
            if self._last_p[idx] != -1 and self._last_p[idx] != p and dt >= 0 and dt < self.flicker_window_us:
                self._last_ts_us[idx] = t
                self._last_p[idx] = p
                continue

            self._last_ts_us[idx] = t
            self._last_p[idx] = p
            filtered.append((float(x), float(y), p, t))

        return filtered


class BallDetector:
    """Three candidate detectors for a fast DVS hover obstacle task."""

    def __init__(self, width: int = 128, height: int = 128) -> None:
        self.width = width
        self.height = height
        self._last_energy = 0.0
        self._ema_energy = 0.0

    def detect(self, events: List[Event], strategy: str) -> DetectionResult:
        if not events:
            return DetectionResult(False, 0.0, -1.0, -1.0, 0.0, 0.0, strategy, 'no_events')

        if strategy == 'density':
            return self._detect_density(events)
        if strategy == 'cluster':
            return self._detect_cluster(events)
        if strategy == 'radial':
            return self._detect_radial(events)

        return DetectionResult(False, 0.0, -1.0, -1.0, 0.0, 0.0, strategy, 'unknown_strategy')

    def _energy_and_approach(self, n_events: int) -> float:
        energy = float(n_events)
        self._ema_energy = 0.8 * self._ema_energy + 0.2 * energy
        approach_score = max(0.0, energy - self._ema_energy)
        self._last_energy = energy
        return approach_score

    def _detect_density(self, events: List[Event]) -> DetectionResult:
        cell = 8
        gx = self.width // cell
        gy = self.height // cell
        grid = [[0 for _ in range(gx)] for _ in range(gy)]

        sx = 0.0
        sy = 0.0
        for x, y, _, _ in events:
            ix = int(x) // cell
            iy = int(y) // cell
            if 0 <= ix < gx and 0 <= iy < gy:
                grid[iy][ix] += 1
            sx += x
            sy += y

        peak = 0
        peak_ix = 0
        peak_iy = 0
        for iy in range(gy):
            for ix in range(gx):
                if grid[iy][ix] > peak:
                    peak = grid[iy][ix]
                    peak_ix = ix
                    peak_iy = iy

        n = len(events)
        cx = sx / n
        cy = sy / n
        radius = max(2.0, (peak / max(1, n)) * 25.0)
        confidence = min(1.0, peak / 55.0)
        detected = peak >= 25 and n >= 40
        approach = self._energy_and_approach(n)

        note = f'peak_cell=({peak_ix},{peak_iy}) peak={peak} n={n}'
        return DetectionResult(detected, confidence, cx, cy, radius, approach, 'density', note)

    def _detect_cluster(self, events: List[Event]) -> DetectionResult:
        occupancy: Dict[Tuple[int, int], int] = {}
        for x, y, _, _ in events:
            xi = int(x)
            yi = int(y)
            key = (xi, yi)
            occupancy[key] = occupancy.get(key, 0) + 1

        visited = set()
        best_cluster: List[Tuple[int, int]] = []
        for seed in occupancy.keys():
            if seed in visited:
                continue

            stack = [seed]
            cluster = []
            visited.add(seed)
            while stack:
                cx, cy = stack.pop()
                cluster.append((cx, cy))
                for nx in (cx - 1, cx, cx + 1):
                    for ny in (cy - 1, cy, cy + 1):
                        if nx == cx and ny == cy:
                            continue
                        nkey = (nx, ny)
                        if nkey in occupancy and nkey not in visited:
                            visited.add(nkey)
                            stack.append(nkey)

            if len(cluster) > len(best_cluster):
                best_cluster = cluster

        n = len(events)
        if not best_cluster:
            return DetectionResult(False, 0.0, -1.0, -1.0, 0.0, self._energy_and_approach(n), 'cluster', 'no_cluster')

        min_x = min(pt[0] for pt in best_cluster)
        max_x = max(pt[0] for pt in best_cluster)
        min_y = min(pt[1] for pt in best_cluster)
        max_y = max(pt[1] for pt in best_cluster)

        w = max_x - min_x + 1
        h = max_y - min_y + 1
        area_box = float(w * h)
        fill = len(best_cluster) / max(1.0, area_box)
        ratio = min(w, h) / max(1.0, float(max(w, h)))

        cx = sum(pt[0] for pt in best_cluster) / len(best_cluster)
        cy = sum(pt[1] for pt in best_cluster) / len(best_cluster)
        radius = (w + h) * 0.25

        shape_score = max(0.0, min(1.0, 0.6 * ratio + 0.4 * fill))
        confidence = shape_score
        detected = len(best_cluster) >= 22 and ratio >= 0.45
        approach = self._energy_and_approach(n)

        note = (
            f'cluster_size={len(best_cluster)} box={w}x{h} '
            f'fill={fill:.2f} ratio={ratio:.2f}'
        )
        return DetectionResult(detected, confidence, cx, cy, radius, approach, 'cluster', note)

    def _detect_radial(self, events: List[Event]) -> DetectionResult:
        n = len(events)
        cx = sum(e[0] for e in events) / n
        cy = sum(e[1] for e in events) / n

        # Mean radial distance and relative variance for circularity.
        dists = []
        for x, y, _, _ in events:
            dx = x - cx
            dy = y - cy
            dists.append((dx * dx + dy * dy) ** 0.5)

        mean_r = sum(dists) / max(1, len(dists))
        if mean_r <= 1e-6:
            return DetectionResult(False, 0.0, cx, cy, 0.0, self._energy_and_approach(n), 'radial', 'degenerate_radius')

        var_r = sum((d - mean_r) ** 2 for d in dists) / max(1, len(dists))
        std_r = var_r ** 0.5
        cv = std_r / mean_r

        confidence = max(0.0, min(1.0, 1.0 - cv))
        detected = n >= 40 and cv <= 0.55 and 2.0 <= mean_r <= 40.0
        approach = self._energy_and_approach(n)
        note = f'mean_r={mean_r:.2f} std_r={std_r:.2f} cv={cv:.2f} n={n}'

        return DetectionResult(detected, confidence, cx, cy, mean_r, approach, 'radial', note)


def filter_small_connected_components(events: List[Event], min_component_pixels: int) -> List[Event]:
    """Remove events whose connected pixel component is smaller than threshold.

    Connectivity uses 8-neighborhood on unique pixel coordinates.
    """
    if min_component_pixels <= 1 or not events:
        return events

    pixel_events: Dict[Tuple[int, int], List[Event]] = {}
    for e in events:
        x = int(e[0])
        y = int(e[1])
        key = (x, y)
        pixel_events.setdefault(key, []).append(e)

    visited = set()
    kept: List[Event] = []

    for seed in pixel_events.keys():
        if seed in visited:
            continue

        stack = [seed]
        component_pixels: List[Tuple[int, int]] = []
        visited.add(seed)

        while stack:
            cx, cy = stack.pop()
            component_pixels.append((cx, cy))

            for nx in (cx - 1, cx, cx + 1):
                for ny in (cy - 1, cy, cy + 1):
                    if nx == cx and ny == cy:
                        continue
                    nkey = (nx, ny)
                    if nkey in pixel_events and nkey not in visited:
                        visited.add(nkey)
                        stack.append(nkey)

        if len(component_pixels) >= min_component_pixels:
            for px in component_pixels:
                kept.extend(pixel_events[px])

    return kept


def compute_avoidance_command(
    result: DetectionResult,
    frame_width: int = 128,
    frame_height: int = 128,
    trigger_approach: float = 8.0,
    trigger_confidence: float = 0.45,
    max_side_speed: float = 0.8,
    back_speed: float = -0.7,
) -> Tuple[float, float, float, str]:
    """Return vx, vy, vz, reason with a simple hover obstacle heuristic."""
    if not result.detected:
        return 0.0, 0.0, 0.0, 'no_detection'

    if result.confidence < trigger_confidence:
        return 0.0, 0.0, 0.0, 'low_confidence'

    if result.approach_score < trigger_approach:
        return 0.0, 0.0, 0.0, 'not_approaching'

    # x image axis: right positive. Move opposite to obstacle side.
    x_norm = (result.center_x - frame_width * 0.5) / max(1.0, frame_width * 0.5)
    y_norm = (result.center_y - frame_height * 0.5) / max(1.0, frame_height * 0.5)

    vy = max(-max_side_speed, min(max_side_speed, -x_norm * max_side_speed))
    vz = max(-0.4, min(0.4, -y_norm * 0.4))
    vx = back_speed
    return vx, vy, vz, 'avoid_active'
