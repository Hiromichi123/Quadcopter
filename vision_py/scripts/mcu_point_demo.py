# mcu解码 + 可视化补帧 等效66.7FPS
import serial
import time
import threading
import cv2
import numpy as np

Device = "/dev/ttyACM0"
#Device = "COM8"

def denoise_isolated_u8(img_u8: np.ndarray, min_neighbors: int = 2) -> np.ndarray:
    mask = (img_u8 != 0).astype(np.uint8)
    H, W = mask.shape
    p = np.pad(mask, 1, mode='constant')
    neigh_cnt = (
        p[0:H,   0:W]   + p[0:H,   1:W+1] + p[0:H,   2:W+2] +
        p[1:H+1, 0:W]                 +     p[1:H+1, 2:W+2] +
        p[2:H+2, 0:W]   + p[2:H+2, 1:W+1] + p[2:H+2, 2:W+2]
    )
    out = img_u8.copy()
    out[(mask == 1) & (neigh_cnt < min_neighbors)] = 0
    return out

class DrumVisualDemo:
    def __init__(self) -> None:
        # 双缓冲：accum 用于收点，frame 用于显示
        self.accum = np.zeros((128, 128), dtype=np.uint8)
        self.frame = np.zeros((128, 128), dtype=np.uint8)

        self.lock = threading.Lock()
        self.running = True

        # 20ms 出一帧
        self.frame_period_s = 0.015

        # 可选：显示前去噪
        self.enable_denoise = True
        self.denoise_neighbors = 2

        # 可选：点加粗（更容易看）
        self.draw_radius = 0  # 0=单像素；1=3x3；2=5x5

    def run(self):
        t_vis = threading.Thread(target=self.visor, daemon=True)
        t_vis.start()
        self.drum_area_detection()

    def _draw_point(self, x: int, y: int):
        if self.draw_radius <= 0:
            self.accum[y, x] = 255
            return
        r = self.draw_radius
        y0 = max(0, y - r); y1 = min(127, y + r)
        x0 = max(0, x - r); x1 = min(127, x + r)
        self.accum[y0:y1+1, x0:x1+1] = 255

    def visor(self):
        cv2.namedWindow("Demo", cv2.WINDOW_NORMAL)

        while self.running:
            with self.lock:
                img = self.frame.copy()

            if self.enable_denoise:
                img = denoise_isolated_u8(img, min_neighbors=self.denoise_neighbors)

            vis = cv2.resize(img, (1024, 1024), interpolation=cv2.INTER_NEAREST)
            vis = cv2.applyColorMap(vis, cv2.COLORMAP_TURBO)
            cv2.imshow("Demo", vis)

            k = cv2.waitKey(1) & 0xFF
            if k == 27 or k == ord('q'):
                self.running = False
                break

        cv2.destroyAllWindows()

    def drum_area_detection(self):
        next_tick = time.time() + self.frame_period_s
        count = 0

        with serial.Serial(Device, 115200, timeout=1) as ser:
            while self.running:
                buff = ser.read(2)
                if len(buff) < 2:
                    continue

                dx = ((buff[0] & 0x3f) << 1) + ((buff[1] & 0x10) >> 4)
                dy = ((buff[1] & 0x0f) << 3) + ((buff[0] & 0xC0) >> 5) + ((buff[1] & 0x20) >> 5)

                # 防止越界
                if 0 <= dx < 128 and 0 <= dy < 128:
                    with self.lock:
                        self._draw_point(dx, dy)
                    count += 1

                # 每 20ms 结算一次帧
                now = time.time()
                if now >= next_tick:
                    with self.lock:
                        self.frame[:] = self.accum
                        self.accum.fill(0)

                    # 你想看吞吐的话打开
                    # print("events/20ms:", count)
                    count = 0
                    # 防止累积漂移（用绝对 tick 推进）
                    while next_tick <= now:
                        next_tick += self.frame_period_s

drumdemo = DrumVisualDemo()
drumdemo.run()
