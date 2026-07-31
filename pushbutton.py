#!/usr/bin/env python

# originally based on listen-for-shutdown.py from pi-power-button

import RPi.GPIO as GPIO
import subprocess
import time
import os
from time import sleep

def blink(times):
	blink_time = 0.1
	powerPin = 16
	for i in range(times):	# blink times
		GPIO.output(powerPin, 0) 
		sleep(blink_time)
		GPIO.output(powerPin, 1)
		sleep(blink_time)
	sleep(0.65)

def change_mode():
	push_button = 26
	powerPin = 16
	txPin = 27
	sleep(0.75)
	if GPIO.input(push_button):
		print("sudo reboot -h now", flush=True)
		os.system("echo 'reboot due to push button!' | wall")
		GPIO.setwarnings(False)
		GPIO.setup(powerPin, GPIO.OUT)
		GPIO.output(powerPin, 0);
		os.system("sudo reboot now")
#		subprocess.call(['reboot', '-h', 'now'], shell=False)
		return
	blink(1)
	if GPIO.input(push_button):
		print("switch to AFSK", flush=True)
		os.system("echo 'switch to AFSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -a")
		return
	blink(2)	
	if GPIO.input(push_button):
		print("switch to FSK", flush=True)
		os.system("echo 'switch to FSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -f")		
		return
	blink(3)
	if GPIO.input(push_button):
		print("switch to BPSK", flush=True)
		os.system("echo 'switch to BPSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -b")
		return
	blink(4)
	if GPIO.input(push_button):
		print("switch to SSTV", flush=True)
		os.system("echo 'switch to SSTV due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -s")
		return
	blink(5)
	if GPIO.input(push_button):
		print("switch to CW", flush=True)
		os.system("echo 'switch to CW due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -m")
		return
	blink(6)
	if GPIO.input(push_button):
		print("switch to Repeater", flush=True)
		os.system("echo 'switch to Repeater due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -e")
		return
	blink(7)
	if GPIO.input(push_button):
		print("switch to FunCube", flush=True)
		os.system("echo 'switch to FunCube due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -j")
		return
	blink(8)
	if GPIO.input(push_button):
		print("switch to PacSat", flush=True)
		os.system("echo 'switch to PacSat due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -G")
		return
	for i in range(3):	# blink 3 times slowly
		GPIO.output(powerPin, 0) 
		sleep(0.35)
		GPIO.output(powerPin, 1)
		sleep(0.35)
	sleep(0.65)
	if GPIO.input(push_button):
		print("sudo shutdown -h now", flush=True)
		os.system("echo 'shutdown due to push button!' | wall")
		GPIO.setwarnings(False)
		GPIO.setup(powerPin, GPIO.OUT)
		subprocess.call(['shutdown', '-h', 'now'], shell=False)
		return
	for i in range(3):	# blink two times even more slowly
		GPIO.output(powerPin, 0) 
		sleep(0.7)
		GPIO.output(powerPin, 1)
		sleep(0.7)
	sleep(0.7)
	print("toggle command and control mode", flush=True)
	try:
		f = open("/home/pi/CubeSatSim/command_control", "r")
		f.close()
		print("command and control will be deactivated", flush=True)
		os.system('sudo rm /home/pi/CubeSatSim/command_control')
		os.system("echo 'command and control deactivated by push button!' | wall")
		os.system('sudo systemctl restart command')
	except:
		print("command and control will be activated", flush=True)
		os.system('touch /home/pi/CubeSatSim/command_control')
		os.system("echo 'command and control activated by push button!' | wall")
		os.system('sudo systemctl restart command')
	sleep(1)

powerPin = 16
txPin = 27
push_button = 26
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(txPin, GPIO.OUT)
GPIO.output(txPin, 0)
GPIO.setup(powerPin, GPIO.OUT)
GPIO.output(powerPin, 0)

GPIO.setup(push_button, GPIO.IN, pull_up_down=GPIO.PUD_UP)

if not GPIO.input(push_button): # if pushbutton is held down during boot
		print("Activating WiFi access point!", flush=True)
		GPIO.output(powerPin, 1) 
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1) 
		sleep(0.3)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1) 
		sleep(0.3)
		GPIO.output(powerPin, 0)
		success = 1
		count = 0
		while (success != 0) and (count < 5):
			result = subprocess.run(['nmcli', 'device', 'wifi', 'hotspot', 'con-name', 'Hotspot', 'ifname', 'wlan0', 'ssid', 'CubeSat', 'password', 'amsatao7'], capture_output=True, text=True)
			print(result, flush=True) #.stdout)
			count = count + 1
			success = result.returncode
			sleep(1)	
		result = subprocess.run(['nmcli', 'connection', 'show'], capture_output=True, text=True)
		print(result, flush=True)			
		sleep(2)
		result = subprocess.run(['ttyd', '-p', '8080', 'bash', '&'], capture_output=True, text=True)
		print(result, flush=True) 

GPIO.setup(powerPin, GPIO.OUT)
GPIO.output(powerPin, 1)
while (True):
	sleep(1)
	GPIO.wait_for_edge(push_button, GPIO.FALLING)
	change_mode()
	sleep(5)
