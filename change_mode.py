#!/usr/bin/env python

import time
from time import sleep
#import os
import sys
import RPi.GPIO as GPIO
from RPi.GPIO import output
from subprocess import call

powerPin = 16
txLed = 27

if __name__ == "__main__":
	mode = 'b'
	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		mode = sys.argv[1]
		
	print(mode)

	GPIO.setmode(GPIO.BCM)
	GPIO.setwarnings(False)
	GPIO.setup(powerPin, GPIO.OUT)
	GPIO.setup(txLed, GPIO.OUT)

	if (mode == 'f'):
		GPIO.output(powerPin, 0) # blink two times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)

	elif (mode == 'b'):
		GPIO.output(powerPin, 0) # blink three times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)
	
	elif (mode == 's'):
		GPIO.output(powerPin, 0) # blink four times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)

	elif (mode == 'm'):
		GPIO.output(powerPin, 0) # blink five times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1);
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)

	elif (mode == 'e'):
		GPIO.output(powerPin, 0) # blink six times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1);
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)

	elif (mode == 'j'):
		GPIO.output(powerPin, 0) # blink seven times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1);
		GPIO.output(powerPin, 1)	
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)			
	
	elif (mode == 'a'):
		mode = 'a'
		GPIO.output(powerPin, 0) # blink one time
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(1)
	
	try:
		file = open("/home/pi/CubeSatSim/command_count.txt", "r")
		string = file.read()
		file.close()
		command_count = int(string)
		command_count += 1
		filec = open("/home/pi/CubeSatSim/command_count.txt", "w")
		command_count_string = str(command_count)
		print(command_count_string)
		string = filec.write(command_count_string)
		filec.close()
	except:
		print("Can't write command_count file!")
	print("Command_count: ")
	print(command_count)	
	
	GPIO.output(txLed, 0)
	GPIO.output(powerPin, 0)
	call("sudo systemctl stop rpitx", shell=True)
#			system("sudo systemctl stop cubesatsim")
	
	print("\n/home/pi/CubeSatSim/config -" + mode)
	call("/home/pi/CubeSatSim/config -" + mode, shell=True)

