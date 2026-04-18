#raw data，包含x，y，p
import serial
import time
import threading
import cv2
import numpy as np

class DrumVisualDemo:
    img_path = {
        0: "./imgs/0.png",
        1: "./imgs/1.png",
        2: "./imgs/2.png",
        3: "./imgs/3.png",
        4: "./imgs/4.png",
        5: "./imgs/5.png",
        6: "./imgs/6.png",
        7: "./imgs/slient.png",
    }

    def __init__(self, window_size:tuple = (640,480)) -> None:
        self.id = 7
        self.window_size = window_size
        self.imgs = np.zeros((128,128,3))
        self._buf = []
        self._img_buf = []
        self._is_read = False
        self.read_time = 0
        self.fps_counter = 0
    def run(self):
        t2 = threading.Thread(target=self.visor)
        t2.start()
        self.drum_area_detection()
    
    def visor(self):
        cv2.namedWindow('Drum', 0)
        while 1:
            cv2.imshow('Drum', cv2.resize(self.imgs, (1024,1024)))
            cv2.waitKey(1)


    def drum_area_detection(self):
        start = time.time()

        with serial.Serial('/dev/ttyACM0', 115200) as ser:
            while 1:
                buff = ser.read(2)
                # buff_fix = buff[0] | (buff[1]>> 8)
       
                dx = buff[0]&0x7f
                dy = ((buff[1]&0x3f)<<1) + ((buff[0]&0x80)>>7)

                # dx = buff[0]
                # dy = buff[1]

                p = buff[1] >> 6
                if dx<128 and dy <128:
#                print(dx, dy)
                    self.imgs[dy,dx,p+1] = 255

                if time.time() - start > 0.03:
                    start = time.time()
                    self.imgs = np.zeros((128,128,3))

# #6bits flicker mode
#                 buff = ser.read(2)

#                 dx = buff[0]&0x3f
#                 dy = ((buff[1]&0x0f)<<2) + ((buff[0]&0xC0)>>6)   

# #                print(dx , dy)                

#                 self.imgs[dy,dx,1] = 255

#                 if time.time() - start > 0.05:
#                     start = time.time()
#                     self.imgs = np.zeros((64,64,3))

# # 7bits flicker mode
#                 buff = ser.read(2)
# #                print(buff) 
#                 dx = ((buff[0]&0x3f)<<1) + ((buff[1]&0x10)>>4)
#                 dy = ((buff[1]&0x0f)<<3) + ((buff[0]&0xC0)>>5) + ((buff[1]&0x20)>>5)

# #                print(dx , dy)                

#                 self.imgs[dy,dx,1] = 255

#                 if time.time() - start > 0.05:
#                     start = time.time()
#                     self.imgs = np.zeros((128,128,3))


drumdemo = DrumVisualDemo(window_size=(640,480))
drumdemo.run()



