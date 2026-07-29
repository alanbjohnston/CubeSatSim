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
###		os.system("sudo nmcli connection down preconfigured")
#		result = subprocess.run(['nmcli', 'connection', 'down', 'preconfigured'], capture_output=True, text=True)
#		print(result, flush=True)
#		result = subprocess.run(['nmcli', 'connection', 'down', 'Hotspot-Manual'], capture_output=True, text=True)
#		print(result, flush=True)
###		os.system("sudo nmcli connection down Hotspot-Manual")
		print("sudo reboot -h now")
		os.system("echo 'reboot due to push button!' | wall")
		GPIO.setwarnings(False)
		GPIO.setup(powerPin, GPIO.OUT)
		GPIO.output(powerPin, 0);
		os.system("sudo reboot now")
#		subprocess.call(['reboot', '-h', 'now'], shell=False)
		return
	blink(1)
	if GPIO.input(push_button):
		print("switch to AFSK")
		os.system("echo 'switch to AFSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -a")
		return
	blink(2)	
	if GPIO.input(push_button):
		print("switch to FSK")
		os.system("echo 'switch to FSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -f")		
		return
	blink(3)
	if GPIO.input(push_button):
		print("switch to BPSK")
		os.system("echo 'switch to BPSK due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -b")
		return
	blink(4)
	if GPIO.input(push_button):
		print("switch to SSTV")
		os.system("echo 'switch to SSTV due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -s")
		return
	blink(5)
	if GPIO.input(push_button):
		print("switch to CW")
		os.system("echo 'switch to CW due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -m")
		return
	blink(6)
	if GPIO.input(push_button):
		print("switch to Repeater")
		os.system("echo 'switch to Repeater due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -e")
		return
	blink(7)
	if GPIO.input(push_button):
		print("switch to FunCube")
		os.system("echo 'switch to FunCube due to push button!' | wall")
		os.system("/home/pi/CubeSatSim/config -j")
		return
	blink(8)
	if GPIO.input(push_button):
		print("switch to PacSat")
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
		print("sudo nmcli connection up preconfigured")
###		os.system("sudo nmcli connection down preconfigured")
#		result = subprocess.run(['nmcli', 'connection', 'down', 'preconfigured'], capture_output=True, text=True)
#		print(result, flush=True)
#		result = subprocess.run(['nmcli', 'connection', 'down', 'Hotspot-Manual'], capture_output=True, text=True)
#		print(result, flush=True)
###		os.system("sudo nmcli connection down Hotspot-Manual")
		print("sudo shutdown -h now")
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
	print("toggle command and control mode")
	try:
		f = open("/home/pi/CubeSatSim/command_control", "r")
		f.close()
		print("command and control will be deactivated")
		os.system('sudo rm /home/pi/CubeSatSim/command_control')
		os.system("echo 'command and control deactivated by push button!' | wall")
		os.system('sudo systemctl restart command')
	except:
		print("command and control will be activated")
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
# GPIO.output(powerPin, 1) 
# sleep(0.5)
# GPIO.output(powerPin, 0)
# sleep(0.5)

GPIO.setup(push_button, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# sleep(1)
if not GPIO.input(push_button): # if pushbutton is held down during boot
		print("Activating WiFi access point!")
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
		print("sudo nmcli device wifi hotspot con-name HotSpot ifname wlan0 ssid CubeSat password amsatao7")	
##		os.system("sudo nmcli device wifi hotspot con-name HotSpot ifname wlan0 ssid CubeSat password amsatao7")
#		os.system("sudo nmcli c add type wifi ifname wlan0 con-name Hotspot-Manual ssid MyHotspot")
#		os.system("sudo nmcli connection modify Hotspot-Manual 802-11-wireless.mode ap 802-11-wireless.band bg")
	#	# os.system("sudo nmcli connection modify Hotspot-Manual wifi-sec.key-mgmt wpa-psk wifi-sec.psk 'YourPassword123' ipv4.method shared")
#		os.system("sudo nmcli connection down uuid 335c2e13-cab4-40bf-a297-7ff2535ec214") # preconfigured")
#		os.system("sudo nmcli device disconnect wlan0")
#		os.system("sudo nmcli connection up uuid d437746a-411f-4f02-95ea-7b2f7665d59a") # Hotspot-Manual")
		result = subprocess.run(['nmcli', 'connection', 'modify', 'Hotspot-Manual', 'connection.interface-name', 'wlan0'], capture_output=True, text=True)
		print(result, flush=True) #.stdout)	
#		result = subprocess.run(['nmcli', 'connection', 'down', 'preconfigured'], capture_output=True, text=True)
#		print(result, flush=True) #.stdout)
#		os.system("sudo nmcli connection up Hotspot-Manual")
		result = subprocess.run(['nmcli', 'radio', 'wifi', 'on'], capture_output=True, text=True)
		print(result, flush=True)	
		result = subprocess.run(['nmcli', 'connection', 'up', 'Hotspot-Manual'], capture_output=True, text=True)
		print(result, flush=True) #.stdout)
#		if (result.returncode != 0):
		print(result.returncode)
		result = subprocess.run(['nmcli', 'connection', 'show'], capture_output=True, text=True)
		print(result, flush=True)			
		sleep(2)
else:
		result = subprocess.run(['nmcli', 'connection', 'modify', 'preconfigured', 'connection.interface-name', 'wlan0'], capture_output=True, text=True)
		print(result, flush=True) #.stdout)	
#		result = subprocess.run(['nmcli', 'connection', 'down', 'Hotspot-Manual'], capture_output=True, text=True)
#		print(result, flush=True) # .stdout)
		result = subprocess.run(['nmcli', 'radio', 'wifi', 'on'], capture_output=True, text=True)
		print(result, flush=True)		
		result = subprocess.run(['nmcli', 'connection', 'up', 'preconfigured'], capture_output=True, text=True)
		print(result, flush=True) # .stdout)
#		if (result.returncode != 0):
		print(result.returncode)
		result = subprocess.run(['nmcli', 'connection', 'show'], capture_output=True, text=True)
		print(result, flush=True)			
#		os.system("sudo nmcli connection up preconfigured")
GPIO.setup(powerPin, GPIO.OUT)
GPIO.output(powerPin, 1)
while (True):
	sleep(1)
	GPIO.wait_for_edge(push_button, GPIO.FALLING)
	change_mode()
	sleep(5)
