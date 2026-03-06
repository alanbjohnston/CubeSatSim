import RPi.GPIO as GPIO
import subprocess
import time
import os
from time import sleep

txPin = 27
pttPin = 20

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(txPin, GPIO.OUT)
GPIO.output(txPin, 0)
print("0")
GPIO.setup(pttPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while (True):
	sleep(0.1)
	GPIO.wait_for_edge(pttPin, GPIO.FALLING)
	GPIO.output(txPin, 1)
	sleep(0.1)
	print("1")
	GPIO.wait_for_edge(pttPin, GPIO.RISING)	
	GPIO.output(txPin, 0)
	print("0")
