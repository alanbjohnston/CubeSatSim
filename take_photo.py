import time
from picamera import PiCamera

camera = PiCamera()
#camera.resolution = (3280,2464)
camera.resolution = (2592,1944)
camera.start_preview()

current_time = time.strftime("%H;%M;%S", time.localtime())
camera.capture('/home/pi/payload/img'+current_time+'.jpg')
