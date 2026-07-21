#!/usr/bin/env python

#import subprocess
import time
from time import sleep
#import os
import sys
from os import system
from os import path
from PIL import Image, ImageDraw, ImageFont, ImageColor
import serial	
import random
import subprocess

def output(pin, value):
	command = "gpio -g write " + str(pin) + " " + str(value)
	system(command)
	print(command)

def input(pin):
	# command = "gpio -g read " + str(pin)
	query = ["gpio", "-g", "read", str(pin)] # Read GPIO pin
	command = "gpio -g read " + str(pin)
	try:
		result = subprocess.run(query, capture_output=True, text=True, check=True)
#		print(f"Command run was: {query}")
#		print("Sucess!")
#		print(f"Output of the command (stdout): {result.stdout}")
#		print(f"{command}: {result.stdout}")
		return int(result.stdout)
	except subprocess.CalledProcessError as e:
#		print(f"Command failed with return code: {e.returncode}")
#		print(f"Command run was: {e.cmd}")
#		print(f"Output of the command (stdout): {e.stdout}")
#		print(f"Error output of the command (stderr): {e.stderr}")
		print(f"{command}: -1")
		return -1

def setup(pin, config):
	if config == "in" or config == "out" or config == "up" or config == "down":
		command = "gpio -g mode " + str(pin) + " " + config
		system(command)
		print(command)
	else:
		print(f"Unknown gpio setup configuration: {config}")

def blink(times):
	powerPin = 16
	for i in range(times):
		system("gpio -g write " + str(powerPin) + " 0") # blink two times
		sleep(0.1)
		system("gpio -g write " + str(powerPin) + " 1")
		sleep(0.1)

def sim_failure_check():
	try:
		global card
		global cam_fail
		global sim_mode
		global sim_config
		cam_fail = False
		file = open("/home/pi/CubeSatSim/failure_mode.txt")
		fail_mode = int(file.read(2))
#		print("Fail_mode: ")
#		print(fail_mode)
		if (fail_mode == 11):
			card = "Device"  # Change audio so no FM audio plays
			print("Failure mode no FM audio")
			sim_mode = True
		elif (fail_mode == 7):
			cam_fail = True
			print("Failure mode camera fail")	
			sim_mode = True
			card = "Headphones"
		elif (fail_mode == -1):
			print("No failure mode")
			card = "Headphones"
			if sim_config:
				sim_mode = True
		else:
			print("Other failure mode")
			card = "Headphones"
			sim_mode = True
	except:
		print("No failure mode")
		card = "Headphones"
		if sim_config:
			sim_mode = True

def battery_saver_check():
	try:
		global txc
		f = open("/home/pi/CubeSatSim/battery_saver", "r")
		f.close()
		txc = False
		print("Safe Mode!")
		print("battery saver activated")
	except:
		print("battery saver not activated")
#		txc = True

def blink(times):
	powerPin = 16
	for i in range(times):
		output(powerPin, 0) # blink 
		sleep(0.1)
		output(powerPin, 1)
		sleep(0.1)
		
def increment_mode():
	print("increment mode")
	powerPin = 16
	try:
		file = open("/home/pi/CubeSatSim/.mode")
		mode = file.read(1)
	except:
#		mode = "f"
		if (debug_mode == 1):
			print("Can't open .mode file") # , defaulting to FSK")
	file.close()
	print("Mode is: ")
	print(mode)
	if (mode == 'a'):
		mode = 'f'
		blink(2)
		sleep(2.5)

	elif (mode == 'f'):
		mode = 'b'
		blink(3)
		sleep(2.5)
	
	elif (mode == 'b'):
		mode = 's'
		blink(4)
		sleep(2.5)

	elif (mode == 's'):
		mode = 'm'
		blink(5)
		sleep(2.5)
	else:
		mode = 'a'
		blink(1)
		sleep(2.5)

	try:	
		file = open("/home/pi/CubeSatSim/.mode", "w")
		count_string = str(command_count)
		file.write(mode)
		file.close()
		print(".mode file written")
		
		output(txLed, 0)
		output(powerPin, 0)
		print("sudo reboot -h now")
		setup(powerPin, "out")
		output(powerPin, 0);
#		system("reboot -h now")
#		release = True;

		print("Changing mode now")
#		system("/home/pi/CubeSatSim/config -" + mode)
		system("reboot -h now")

		sleep(10);
	except:
		print("can't write to .mode file")
		
def camera_photo():
	global cam_fail
	global os_status
	sim_failure_check()
	system("sudo rm /home/pi/CubeSatSim/camera_out.jpg")
	stored_image = False
	try:
		if os_status == "bookworm":
			system("rpicam-still -o /home/pi/CubeSatSim/camera_out.jpg --width 320 --height 256") #  > /dev/null 2>&1")
		else:
			system("raspistill -o /home/pi/CubeSatSim/camera_out.jpg -w 320 -h 256")
		f = open("/home/pi/CubeSatSim/camera_out.jpg")
		f.close()
		print("Photo taken")
		if (cam_fail == True):
			system("cp /home/pi/CubeSatSim/sstv//sstv_image_2_320_x_256.jpeg /home/pi/CubeSatSim/camera_out.jpg")
			print("Using stored image")
			stored_image = True
	except:
		system("cp /home/pi/CubeSatSim/sstv//sstv_image_2_320_x_256.jpeg /home/pi/CubeSatSim/camera_out.jpg")
		print("Using stored image")
		stored_image = True
	if (stored_image == False):	
		file='/home/pi/CubeSatSim/camera_out.jpg'
		font1 = ImageFont.truetype('DejaVuSerif.ttf', 20)
		font2 = ImageFont.truetype('DejaVuSerif-Bold.ttf', 16)
	
		try:
			filep = open("/home/pi/CubeSatSim/telem_string.txt")
			telem_string = filep.readline()
		except:
			telem_string = ""
			if (debug_mode == 1):
				print("Can't read telem_string.txt")		
		print(telem_string)
		
		img = Image.open(file)
		draw = ImageDraw.Draw(img) 
	#					draw.text((10, 10), callsign, font=font2, fill='white')
	#					draw.text((120, 10), telem_string, font=font2, fill='white')				
		draw.text((12, 12), callsign, font=font1, fill='black')
		draw.text((10, 10), callsign, font=font1, fill='white')
		draw.text((112, 12), telem_string, font=font2, fill='black')  # was 122
		draw.text((110, 10), telem_string, font=font2, fill='white')  # was 120
		img.save(file)

		return True
	else:
		return False

def program_fm(rx, tx, rxpl_value, sq, txpl_value):

	try:
		global gpsd_status
		global pd
		global ptt
		global two_meter_rpitx
		global txc
		global restore_txc
		
		if (gpsd_status == "active"):
			print("Stopping gpsd.socket")
			system("sudo systemctl stop gpsd.socket")
		print("Programming FM module!\n");
		txf = float(tx)
		rxf = float(rx)
		
		if (txf > 144.0) and (txf < 148.0):
			two_meter_rpitx = True
			print("Transmitting in 2m band using rptix")
#			if (txc):
#				restore_txc = True
#				print("Restore FM with 70cm band")
			txc = False
			tx = "434.9000"
#		elif (two_meter_rpitx):
#			two_meter_rpitx = False
#			print("Stop transmitting in 2m bacnd using rptix")
#			if (restore_txc):
#				txc = True
#				restore_txc = False
#				print("Restoring FM with 70cm band")
#				print("Rebooting")
#				system("sudo reboot now")
			
		if (txf > 450.0) or ((txf < 420.0) and (txf > 148.0)) or (txf < 144.0) :
			tx = "434.9000"
			print("Transmit frequency out of amateur bands")
		
		if (rxf > 450.0) or (rxf < 420.0):
			rx = "435.0000"		
			print("Receive frequency out of FM bounds")
		output(pd, 1)
		output (ptt, 1)
		try:
			ser = serial.Serial("/dev/ttyAMA0", 9600)
			print(ser.portstr)
		#		uhf_string = "AT+DMOSETGROUP=0," + rx +"," + tx + ",0,3,0,0\r\n"
			uhf_string = "AT+DMOSETGROUP=0," + rx + "," + tx + "," + rxpl_value + "," + sq + "," + txpl_value + ",0\r\n"
			print(uhf_string)
			for i in range(6):
		#			ser.write(b"AT+DMOSETGROUP=0,435.0000,434.9000,0,3,0,0\r\n")
				ser.write(uhf_string.encode())
				sleep(0.1)
			ser.close()
			ser = serial.Serial("/dev/ttyAMA0", 115200) # reset back to 115200 for cubesatsim code for payload sensor data
		except Exception as e:
			print(f"An error occurred: {e}")	
			print("Error in serial write")
	#	output(pd, 0)  # Don't turn off receiver
		if (gpsd_status == "active"):
			print("Restarting gpsd.socket")
			system("sudo systemctl restart gpsd.socket")
	except Exception as e:
		print(f"An error occurred: {e}")
		print("program_fm failed")

def start_repeater(tx_doppler_freq_hz):
	global txLed
	print("Starting repeater")
	output(txLed, 1)
	txr = "{:.3f}".format(tx_doppler_freq_hz/1e3)
	print(txr)
	system("sudo nc -l 8011 | csdr convert_i16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + txr + " > /dev/null 2>&1 &")
	sleep(0.5)
	system("sudo arecord -D shared_mic -r48000 -fS16_LE -c1 | nc localhost 8011 &")

def stop_repeater():
	global txLed
#	print("No carrier detected, stopping repeater")
	output(txLed, 0)
	system("sudo rpitx -i null > /dev/null 2>&1")
	system("sudo killall -9 arecord > /dev/null 2>&1")
	system("sudo killall -9 nc > /dev/null 2>&1")
#	system("sudo killall -9 rpitx > /dev/null 2>&1")
	print("Resetting audio")
	system("sudo /etc/init.d/alsa-utils stop")
	system("sudo /etc/init.d/alsa-utils start")
	print("Finished resetting audio")
#	print("Ready to detect carrier")

iss_doppler_passes = {
	5: [
		{"time_sec": -180, "velocity_kms": -4.00, "doppler_434_khz": 5.79, "obs_434_mhz": 434.9058, "doppler_144_khz": 1.93, "obs_144_mhz": 144.9019},
        {"time_sec": -170, "velocity_kms": -3.89, "doppler_434_khz": 5.64, "obs_434_mhz": 434.9056, "doppler_144_khz": 1.88, "obs_144_mhz": 144.9019},
        {"time_sec": -160, "velocity_kms": -3.78, "doppler_434_khz": 5.48, "obs_434_mhz": 434.9055, "doppler_144_khz": 1.83, "obs_144_mhz": 144.9018},
        {"time_sec": -150, "velocity_kms": -3.67, "doppler_434_khz": 5.32, "obs_434_mhz": 434.9053, "doppler_144_khz": 1.77, "obs_144_mhz": 144.9018},
        {"time_sec": -140, "velocity_kms": -3.54, "doppler_434_khz": 5.14, "obs_434_mhz": 434.9051, "doppler_144_khz": 1.71, "obs_144_mhz": 144.9017},
        {"time_sec": -130, "velocity_kms": -3.42, "doppler_434_khz": 4.96, "obs_434_mhz": 434.9050, "doppler_144_khz": 1.65, "obs_144_mhz": 144.9017},
        {"time_sec": -120, "velocity_kms": -3.28, "doppler_434_khz": 4.76, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.59, "obs_144_mhz": 144.9016},
        {"time_sec": -110, "velocity_kms": -3.14, "doppler_434_khz": 4.55, "obs_434_mhz": 434.9046, "doppler_144_khz": 1.52, "obs_144_mhz": 144.9015},
        {"time_sec": -100, "velocity_kms": -2.99, "doppler_434_khz": 4.33, "obs_434_mhz": 434.9043, "doppler_144_khz": 1.44, "obs_144_mhz": 144.9014},
        {"time_sec": -90, "velocity_kms": -2.83, "doppler_434_khz": 4.10, "obs_434_mhz": 434.9041, "doppler_144_khz": 1.37, "obs_144_mhz": 144.9014},
        {"time_sec": -80, "velocity_kms": -2.66, "doppler_434_khz": 3.86, "obs_434_mhz": 434.9039, "doppler_144_khz": 1.29, "obs_144_mhz": 144.9013},
        {"time_sec": -70, "velocity_kms": -2.48, "doppler_434_khz": 3.60, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.20, "obs_144_mhz": 144.9012},
        {"time_sec": -60, "velocity_kms": -2.29, "doppler_434_khz": 3.32, "obs_434_mhz": 434.9033, "doppler_144_khz": 1.11, "obs_144_mhz": 144.9011},
        {"time_sec": -50, "velocity_kms": -2.09, "doppler_434_khz": 3.03, "obs_434_mhz": 434.9030, "doppler_144_khz": 1.01, "obs_144_mhz": 144.9010},
        {"time_sec": -40, "velocity_kms": -1.87, "doppler_434_khz": 2.71, "obs_434_mhz": 434.9027, "doppler_144_khz": 0.90, "obs_144_mhz": 144.9009},
        {"time_sec": -30, "velocity_kms": -1.63, "doppler_434_khz": 2.37, "obs_434_mhz": 434.9024, "doppler_144_khz": 0.79, "obs_144_mhz": 144.9008},
        {"time_sec": -20, "velocity_kms": -1.33, "doppler_434_khz": 1.93, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.64, "obs_144_mhz": 144.9006},
        {"time_sec": -10, "velocity_kms": -0.84, "doppler_434_khz": 1.22, "obs_434_mhz": 434.9012, "doppler_144_khz": 0.41, "obs_144_mhz": 144.9004},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": 0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": 0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.84, "doppler_434_khz": -1.22, "obs_434_mhz": 434.8988, "doppler_144_khz": -0.41, "obs_144_mhz": 144.8996},
        {"time_sec": 20, "velocity_kms": 1.33, "doppler_434_khz": -1.93, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.64, "obs_144_mhz": 144.8994},
        {"time_sec": 30, "velocity_kms": 1.63, "doppler_434_khz": -2.37, "obs_434_mhz": 434.8976, "doppler_144_khz": -0.79, "obs_144_mhz": 144.8992},
        {"time_sec": 40, "velocity_kms": 1.87, "doppler_434_khz": -2.71, "obs_434_mhz": 434.8973, "doppler_144_khz": -0.90, "obs_144_mhz": 144.8991},
        {"time_sec": 50, "velocity_kms": 2.09, "doppler_434_khz": -3.03, "obs_434_mhz": 434.8970, "doppler_144_khz": -1.01, "obs_144_mhz": 144.8990},
        {"time_sec": 60, "velocity_kms": 2.29, "doppler_434_khz": -3.32, "obs_434_mhz": 434.8967, "doppler_144_khz": -1.11, "obs_144_mhz": 144.8989},
        {"time_sec": 70, "velocity_kms": 2.48, "doppler_434_khz": -3.60, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.20, "obs_144_mhz": 144.8988},
        {"time_sec": 80, "velocity_kms": 2.66, "doppler_434_khz": -3.86, "obs_434_mhz": 434.8961, "doppler_144_khz": -1.29, "obs_144_mhz": 144.8987},
        {"time_sec": 90, "velocity_kms": 2.83, "doppler_434_khz": -4.10, "obs_434_mhz": 434.8959, "doppler_144_khz": -1.37, "obs_144_mhz": 144.8986},
        {"time_sec": 100, "velocity_kms": 2.99, "doppler_434_khz": -4.33, "obs_434_mhz": 434.8957, "doppler_144_khz": -1.44, "obs_144_mhz": 144.8986},
        {"time_sec": 110, "velocity_kms": 3.14, "doppler_434_khz": -4.55, "obs_434_mhz": 434.8954, "doppler_144_khz": -1.52, "obs_144_mhz": 144.8985},
        {"time_sec": 120, "velocity_kms": 3.28, "doppler_434_khz": -4.76, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.59, "obs_144_mhz": 144.8984},
        {"time_sec": 130, "velocity_kms": 3.42, "doppler_434_khz": -4.96, "obs_434_mhz": 434.8950, "doppler_144_khz": -1.65, "obs_144_mhz": 144.8983},
        {"time_sec": 140, "velocity_kms": 3.54, "doppler_434_khz": -5.14, "obs_434_mhz": 434.8949, "doppler_144_khz": -1.71, "obs_144_mhz": 144.8983},
        {"time_sec": 150, "velocity_kms": 3.67, "doppler_434_khz": -5.32, "obs_434_mhz": 434.8947, "doppler_144_khz": -1.77, "obs_144_mhz": 144.8982},
        {"time_sec": 160, "velocity_kms": 3.78, "doppler_434_khz": -5.48, "obs_434_mhz": 434.8945, "doppler_144_khz": -1.83, "obs_144_mhz": 144.8982},
        {"time_sec": 170, "velocity_kms": 3.89, "doppler_434_khz": -5.64, "obs_434_mhz": 434.8944, "doppler_144_khz": -1.88, "obs_144_mhz": 144.8981},
        {"time_sec": 180, "velocity_kms": 4.00, "doppler_434_khz": -5.79, "obs_434_mhz": 434.8942, "doppler_144_khz": -1.93, "obs_144_mhz": 144.8981}
	],
    20: [
        {"time_sec": -180, "velocity_kms": -5.81, "doppler_434_khz": 8.43, "obs_434_mhz": 434.9084, "doppler_144_khz": 2.81, "obs_144_mhz": 144.9028},
        {"time_sec": -170, "velocity_kms": -5.68, "doppler_434_khz": 8.24, "obs_434_mhz": 434.9082, "doppler_144_khz": 2.75, "obs_144_mhz": 144.9027},
        {"time_sec": -160, "velocity_kms": -5.55, "doppler_434_khz": 8.05, "obs_434_mhz": 434.9081, "doppler_144_khz": 2.68, "obs_144_mhz": 144.9027},
        {"time_sec": -150, "velocity_kms": -5.39, "doppler_434_khz": 7.82, "obs_434_mhz": 434.9078, "doppler_144_khz": 2.61, "obs_144_mhz": 144.9026},
        {"time_sec": -140, "velocity_kms": -5.22, "doppler_434_khz": 7.57, "obs_434_mhz": 434.9076, "doppler_144_khz": 2.52, "obs_144_mhz": 144.9025},
        {"time_sec": -130, "velocity_kms": -5.02, "doppler_434_khz": 7.28, "obs_434_mhz": 434.9073, "doppler_144_khz": 2.43, "obs_144_mhz": 144.9024},
        {"time_sec": -120, "velocity_kms": -4.81, "doppler_434_khz": 6.98, "obs_434_mhz": 434.9070, "doppler_144_khz": 2.32, "obs_144_mhz": 144.9023},
        {"time_sec": -110, "velocity_kms": -4.57, "doppler_434_khz": 6.63, "obs_434_mhz": 434.9066, "doppler_144_khz": 2.21, "obs_144_mhz": 144.9022},
        {"time_sec": -100, "velocity_kms": -4.30, "doppler_434_khz": 6.24, "obs_434_mhz": 434.9062, "doppler_144_khz": 2.08, "obs_144_mhz": 144.9021},
        {"time_sec": -90, "velocity_kms": -4.00, "doppler_434_khz": 5.80, "obs_434_mhz": 434.9058, "doppler_144_khz": 1.93, "obs_144_mhz": 144.9019},
        {"time_sec": -80, "velocity_kms": -3.68, "doppler_434_khz": 5.34, "obs_434_mhz": 434.9053, "doppler_144_khz": 1.78, "obs_144_mhz": 144.9018},
        {"time_sec": -70, "velocity_kms": -3.32, "doppler_434_khz": 4.82, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.60, "obs_144_mhz": 144.9016},
        {"time_sec": -60, "velocity_kms": -2.92, "doppler_434_khz": 4.24, "obs_434_mhz": 434.9042, "doppler_144_khz": 1.41, "obs_144_mhz": 144.9014},
        {"time_sec": -50, "velocity_kms": -2.50, "doppler_434_khz": 3.63, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.21, "obs_144_mhz": 144.9012},
        {"time_sec": -40, "velocity_kms": -2.04, "doppler_434_khz": 2.96, "obs_434_mhz": 434.9030, "doppler_144_khz": 0.99, "obs_144_mhz": 144.9010},
        {"time_sec": -30, "velocity_kms": -1.56, "doppler_434_khz": 2.26, "obs_434_mhz": 434.9023, "doppler_144_khz": 0.75, "obs_144_mhz": 144.9008},
        {"time_sec": -20, "velocity_kms": -1.05, "doppler_434_khz": 1.52, "obs_434_mhz": 434.9015, "doppler_144_khz": 0.51, "obs_144_mhz": 144.9005},
        {"time_sec": -10, "velocity_kms": -0.53, "doppler_434_khz": 0.77, "obs_434_mhz": 434.9008, "doppler_144_khz": 0.26, "obs_144_mhz": 144.9003},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.53, "doppler_434_khz": -0.77, "obs_434_mhz": 434.8992, "doppler_144_khz": -0.26, "obs_144_mhz": 144.8997},
        {"time_sec": 20, "velocity_kms": 1.05, "doppler_434_khz": -1.52, "obs_434_mhz": 434.8985, "doppler_144_khz": -0.51, "obs_144_mhz": 144.8995},
        {"time_sec": 30, "velocity_kms": 1.56, "doppler_434_khz": -2.26, "obs_434_mhz": 434.8977, "doppler_144_khz": -0.75, "obs_144_mhz": 144.8992},
        {"time_sec": 40, "velocity_kms": 2.04, "doppler_434_khz": -2.96, "obs_434_mhz": 434.8970, "doppler_144_khz": -0.99, "obs_144_mhz": 144.8990},
        {"time_sec": 50, "velocity_kms": 2.50, "doppler_434_khz": -3.63, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.21, "obs_144_mhz": 144.8988},
        {"time_sec": 60, "velocity_kms": 2.92, "doppler_434_khz": -4.24, "obs_434_mhz": 434.8958, "doppler_144_khz": -1.41, "obs_144_mhz": 144.8986},
        {"time_sec": 70, "velocity_kms": 3.32, "doppler_434_khz": -4.82, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.60, "obs_144_mhz": 144.8984},
        {"time_sec": 80, "velocity_kms": 3.68, "doppler_434_khz": -5.34, "obs_434_mhz": 434.8947, "doppler_144_khz": -1.78, "obs_144_mhz": 144.8982},
        {"time_sec": 90, "velocity_kms": 4.00, "doppler_434_khz": -5.80, "obs_434_mhz": 434.8942, "doppler_144_khz": -1.93, "obs_144_mhz": 144.8981},
        {"time_sec": 100, "velocity_kms": 4.30, "doppler_434_khz": -6.24, "obs_434_mhz": 434.8938, "doppler_144_khz": -2.08, "obs_144_mhz": 144.8979},
        {"time_sec": 110, "velocity_kms": 4.57, "doppler_434_khz": -6.63, "obs_434_mhz": 434.8934, "doppler_144_khz": -2.21, "obs_144_mhz": 144.8978},
        {"time_sec": 120, "velocity_kms": 4.81, "doppler_434_khz": -6.98, "obs_434_mhz": 434.8930, "doppler_144_khz": -2.32, "obs_144_mhz": 144.8977},
        {"time_sec": 130, "velocity_kms": 5.02, "doppler_434_khz": -7.28, "obs_434_mhz": 434.8927, "doppler_144_khz": -2.43, "obs_144_mhz": 144.8976},
        {"time_sec": 140, "velocity_kms": 5.22, "doppler_434_khz": -7.57, "obs_434_mhz": 434.8924, "doppler_144_khz": -2.52, "obs_144_mhz": 144.8975},
        {"time_sec": 150, "velocity_kms": 5.39, "doppler_434_khz": -7.82, "obs_434_mhz": 434.8922, "doppler_144_khz": -2.61, "obs_144_mhz": 144.8974},
        {"time_sec": 160, "velocity_kms": 5.55, "doppler_434_khz": -8.05, "obs_434_mhz": 434.8919, "doppler_144_khz": -2.68, "obs_144_mhz": 144.8973},
        {"time_sec": 170, "velocity_kms": 5.68, "doppler_434_khz": -8.24, "obs_434_mhz": 434.8918, "doppler_144_khz": -2.75, "obs_144_mhz": 144.8973},
        {"time_sec": 180, "velocity_kms": 5.81, "doppler_434_khz": -8.43, "obs_434_mhz": 434.8916, "doppler_144_khz": -2.81, "obs_144_mhz": 144.8972}
    ],
    40: [
        {"time_sec": -180, "velocity_kms": -6.67, "doppler_434_khz": 9.68, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.22, "obs_144_mhz": 144.9032},
        {"time_sec": -170, "velocity_kms": -6.60, "doppler_434_khz": 9.57, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.19, "obs_144_mhz": 144.9032},
        {"time_sec": -160, "velocity_kms": -6.52, "doppler_434_khz": 9.46, "obs_434_mhz": 434.9095, "doppler_144_khz": 3.15, "obs_144_mhz": 144.9032},
        {"time_sec": -150, "velocity_kms": -6.43, "doppler_434_khz": 9.33, "obs_434_mhz": 434.9093, "doppler_144_khz": 3.11, "obs_144_mhz": 144.9031},
        {"time_sec": -140, "velocity_kms": -6.32, "doppler_434_khz": 9.17, "obs_434_mhz": 434.9092, "doppler_144_khz": 3.05, "obs_144_mhz": 144.9031},
        {"time_sec": -130, "velocity_kms": -6.19, "doppler_434_khz": 8.98, "obs_434_mhz": 434.9090, "doppler_144_khz": 2.99, "obs_144_mhz": 144.9030},
        {"time_sec": -120, "velocity_kms": -6.04, "doppler_434_khz": 8.76, "obs_434_mhz": 434.9088, "doppler_144_khz": 2.92, "obs_144_mhz": 144.9029},
        {"time_sec": -110, "velocity_kms": -5.86, "doppler_434_khz": 8.50, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.83, "obs_144_mhz": 144.9028},
        {"time_sec": -100, "velocity_kms": -5.65, "doppler_434_khz": 8.20, "obs_434_mhz": 434.9082, "doppler_144_khz": 2.73, "obs_144_mhz": 144.9027},
        {"time_sec": -90, "velocity_kms": -5.39, "doppler_434_khz": 7.82, "obs_434_mhz": 434.9078, "doppler_144_khz": 2.61, "obs_144_mhz": 144.9026},
        {"time_sec": -80, "velocity_kms": -5.08, "doppler_434_khz": 7.37, "obs_434_mhz": 434.9074, "doppler_144_khz": 2.46, "obs_144_mhz": 144.9025},
        {"time_sec": -70, "velocity_kms": -4.72, "doppler_434_khz": 6.85, "obs_434_mhz": 434.9068, "doppler_144_khz": 2.28, "obs_144_mhz": 144.9023},
        {"time_sec": -60, "velocity_kms": -4.28, "doppler_434_khz": 6.21, "obs_434_mhz": 434.9062, "doppler_144_khz": 2.07, "obs_144_mhz": 144.9021},
        {"time_sec": -50, "velocity_kms": -3.77, "doppler_434_khz": 5.47, "obs_434_mhz": 434.9055, "doppler_144_khz": 1.82, "obs_144_mhz": 144.8993}, # manual note context adjustments active
        {"time_sec": -40, "velocity_kms": -3.17, "doppler_434_khz": 4.60, "obs_434_mhz": 434.9046, "doppler_144_khz": 1.53, "obs_144_mhz": 144.9015},
        {"time_sec": -30, "velocity_kms": -2.48, "doppler_434_khz": 3.60, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.20, "obs_144_mhz": 144.9012},
        {"time_sec": -20, "velocity_kms": -1.70, "doppler_434_khz": 2.47, "obs_434_mhz": 434.9025, "doppler_144_khz": 0.82, "obs_144_mhz": 144.9008},
        {"time_sec": -10, "velocity_kms": -0.87, "doppler_434_khz": 1.26, "obs_434_mhz": 434.9013, "doppler_144_khz": 0.42, "obs_144_mhz": 144.9004},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.87, "doppler_434_khz": -1.26, "obs_434_mhz": 434.8987, "doppler_144_khz": -0.42, "obs_144_mhz": 144.8996},
        {"time_sec": 20, "velocity_kms": 1.70, "doppler_434_khz": -2.47, "obs_434_mhz": 434.8975, "doppler_144_khz": -0.82, "obs_144_mhz": 144.8992},
        {"time_sec": 30, "velocity_kms": 2.48, "doppler_434_khz": -3.60, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.20, "obs_144_mhz": 144.8988},
        {"time_sec": 40, "velocity_kms": 3.17, "doppler_434_khz": -4.60, "obs_434_mhz": 434.8954, "doppler_144_khz": -1.53, "obs_144_mhz": 144.8985},
        {"time_sec": 50, "velocity_kms": 3.77, "doppler_434_khz": -5.47, "obs_434_mhz": 434.8945, "doppler_144_khz": -1.82, "obs_144_mhz": 144.8982},
        {"time_sec": 60, "velocity_kms": 4.28, "doppler_434_khz": -6.21, "obs_434_mhz": 434.8938, "doppler_144_khz": -2.07, "obs_144_mhz": 144.8979},
        {"time_sec": 70, "velocity_kms": 4.72, "doppler_434_khz": -6.85, "obs_434_mhz": 434.8932, "doppler_144_khz": -2.28, "obs_144_mhz": 144.8977},
        {"time_sec": 80, "velocity_kms": 5.08, "doppler_434_khz": -7.37, "obs_434_mhz": 434.8926, "doppler_144_khz": -2.46, "obs_144_mhz": 144.8975},
        {"time_sec": 90, "velocity_kms": 5.39, "doppler_434_khz": -7.82, "obs_434_mhz": 434.8922, "doppler_144_khz": -2.61, "obs_144_mhz": 144.8974},
        {"time_sec": 100, "velocity_kms": 0.87, "doppler_434_khz": -8.19, "obs_434_mhz": 434.8918, "doppler_144_khz": -2.73, "obs_144_mhz": 144.8973},
        {"time_sec": 110, "velocity_kms": 1.70, "doppler_434_khz": -8.50, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.83, "obs_144_mhz": 144.8972},
        {"time_sec": 120, "velocity_kms": 2.48, "doppler_434_khz": -8.76, "obs_434_mhz": 434.8912, "doppler_144_khz": -2.92, "obs_144_mhz": 144.8971},
        {"time_sec": 130, "velocity_kms": 3.17, "doppler_434_khz": -8.98, "obs_434_mhz": 434.8910, "doppler_144_khz": -2.99, "obs_144_mhz": 144.8970},
        {"time_sec": 140, "velocity_kms": 3.77, "doppler_434_khz": -9.17, "obs_434_mhz": 434.8908, "doppler_144_khz": -3.05, "obs_144_mhz": 144.8969},
        {"time_sec": 150, "velocity_kms": 4.28, "doppler_434_khz": -9.33, "obs_434_mhz": 434.8907, "doppler_144_khz": -3.11, "obs_144_mhz": 144.8969},
        {"time_sec": 160, "velocity_kms": 4.72, "doppler_434_khz": -9.46, "obs_434_mhz": 434.8905, "doppler_144_khz": -3.15, "obs_144_mhz": 144.8968},
        {"time_sec": 170, "velocity_kms": 5.08, "doppler_434_khz": -9.58, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.19, "obs_144_mhz": 144.8968},
        {"time_sec": 180, "velocity_kms": 5.39, "doppler_434_khz": -9.68, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.22, "obs_144_mhz": 144.8968}
	],
    70: [	
    	{"time_sec": -180, "velocity_kms": -7.00, "doppler_434_khz": 10.15, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.38, "obs_144_mhz": 144.9034},
    	{"time_sec": -170, "velocity_kms": -6.95, "doppler_434_khz": 10.09, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.36, "obs_144_mhz": 144.9034},
    	{"time_sec": -160, "velocity_kms": -6.91, "doppler_434_khz": 10.02, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.34, "obs_144_mhz": 144.9033},
    	{"time_sec": -150, "velocity_kms": -6.86, "doppler_434_khz":  9.95, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.32, "obs_144_mhz": 144.9033},
    	{"time_sec": -140, "velocity_kms": -6.78, "doppler_434_khz":  9.84, "obs_434_mhz": 434.9098, "doppler_144_khz": 3.28, "obs_144_mhz": 144.9033},
    	{"time_sec": -130, "velocity_kms": -6.70, "doppler_434_khz":  9.72, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.24, "obs_144_mhz": 144.9032},
    	{"time_sec": -120, "velocity_kms": -6.62, "doppler_434_khz":  9.60, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.20, "obs_144_mhz": 144.9032},
    	{"time_sec": -110, "velocity_kms": -6.47, "doppler_434_khz":  9.39, "obs_434_mhz": 434.9094, "doppler_144_khz": 3.13, "obs_144_mhz": 144.9031},
    	{"time_sec": -100, "velocity_kms": -6.32, "doppler_434_khz":  9.17, "obs_434_mhz": 434.9092, "doppler_144_khz": 3.05, "obs_144_mhz": 144.9031},
    	{"time_sec": -90, "velocity_kms": -6.17, "doppler_434_khz":  8.95, "obs_434_mhz": 434.9090, "doppler_144_khz": 2.98, "obs_144_mhz": 144.9030},
    	{"time_sec": -80, "velocity_kms": -5.86, "doppler_434_khz":  8.50, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.83, "obs_144_mhz": 144.9028},
    	{"time_sec": -70, "velocity_kms": -5.55, "doppler_434_khz":  8.05, "obs_434_mhz": 434.9081, "doppler_144_khz": 2.68, "obs_144_mhz": 144.9027},
   		{"time_sec": -60, "velocity_kms": -5.24, "doppler_434_khz":  7.60, "obs_434_mhz": 434.9076, "doppler_144_khz": 2.53, "obs_144_mhz": 144.9025},
   		{"time_sec": -50, "velocity_kms": -4.60, "doppler_434_khz":  6.67, "obs_434_mhz": 434.9067, "doppler_144_khz": 2.22, "obs_144_mhz": 144.9022},
   		{"time_sec": -40, "velocity_kms": -3.96, "doppler_434_khz":  5.74, "obs_434_mhz": 434.9057, "doppler_144_khz": 1.91, "obs_144_mhz": 144.9019},
    	{"time_sec": -30, "velocity_kms": -3.32, "doppler_434_khz":  4.82, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.60, "obs_144_mhz": 144.9016},
    	{"time_sec": -20, "velocity_kms": -2.21, "doppler_434_khz":  3.21, "obs_434_mhz": 434.9032, "doppler_144_khz": 1.07, "obs_144_mhz": 144.9011},
    	{"time_sec": -10, "velocity_kms": -1.11, "doppler_434_khz":  1.61, "obs_434_mhz": 434.9016, "doppler_144_khz": 0.53, "obs_144_mhz": 144.9005},
     	{"time_sec": 0, "velocity_kms":  0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
   		{"time_sec": 10, "velocity_kms":  1.11, "doppler_434_khz": -1.61, "obs_434_mhz": 434.8984, "doppler_144_khz": -0.53, "obs_144_mhz": 144.8995},
  	 	{"time_sec": 20, "velocity_kms":  2.21, "doppler_434_khz": -3.21, "obs_434_mhz": 434.8968, "doppler_144_khz": -1.07, "obs_144_mhz": 144.8989},
    	{"time_sec": 30, "velocity_kms":  3.32, "doppler_434_khz": -4.82, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.60, "obs_144_mhz": 144.8984},
    	{"time_sec": 40, "velocity_kms":  3.96, "doppler_434_khz": -5.74, "obs_434_mhz": 434.8943, "doppler_144_khz": -1.91, "obs_144_mhz": 144.8981},
   		{"time_sec": 50, "velocity_kms":  4.60, "doppler_434_khz": -6.67, "obs_434_mhz": 434.8933, "doppler_144_khz": -2.22, "obs_144_mhz": 144.8978},
   		{"time_sec": 60, "velocity_kms":  5.24, "doppler_434_khz": -7.60, "obs_434_mhz": 434.8924, "doppler_144_khz": -2.53, "obs_144_mhz": 144.8975},
   		{"time_sec": 70, "velocity_kms":  5.55, "doppler_434_khz": -8.05, "obs_434_mhz": 434.8919, "doppler_144_khz": -2.68, "obs_144_mhz": 144.8973},
   		{"time_sec": 80, "velocity_kms":  5.86, "doppler_434_khz": -8.50, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.83, "obs_144_mhz": 144.8972},
   		{"time_sec": 90, "velocity_kms":  6.17, "doppler_434_khz": -8.95, "obs_434_mhz": 434.8910, "doppler_144_khz": -2.98, "obs_144_mhz": 144.8970},
   		{"time_sec": 100, "velocity_kms":  6.32, "doppler_434_khz": -9.17, "obs_434_mhz": 434.8908, "doppler_144_khz": -3.05, "obs_144_mhz": 144.8969},
        {"time_sec": 110, "velocity_kms":  6.47, "doppler_434_khz": -9.39, "obs_434_mhz": 434.8906, "doppler_144_khz": -3.13, "obs_144_mhz": 144.8969},
        {"time_sec": 120, "velocity_kms":  6.62, "doppler_434_khz": -9.60, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.20, "obs_144_mhz": 144.8968},
        {"time_sec": 130, "velocity_kms":  6.70, "doppler_434_khz": -9.72, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.24, "obs_144_mhz": 144.8968},
        {"time_sec": 140, "velocity_kms":  6.78, "doppler_434_khz": -9.84, "obs_434_mhz": 434.8902, "doppler_144_khz": -3.28, "obs_144_mhz": 144.8967},
        {"time_sec": 150, "velocity_kms":  6.86, "doppler_434_khz": -9.95, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.32, "obs_144_mhz": 144.8967},
        {"time_sec": 160, "velocity_kms":  6.91, "doppler_434_khz": -10.02, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.34, "obs_144_mhz": 144.8967},
        {"time_sec": 170, "velocity_kms":  6.95, "doppler_434_khz": -10.09, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.36, "obs_144_mhz": 144.8966},
        {"time_sec": 180, "velocity_kms":  7.00, "doppler_434_khz": -10.15, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.38, "obs_144_mhz": 144.8966}
    ],
    85: [	
        {"time_sec": -180, "velocity_kms": -7.15, "doppler_434_khz": 10.37, "obs_434_mhz": 434.9104, "doppler_144_khz": 3.46, "obs_144_mhz": 144.9035},
        {"time_sec": -170, "velocity_kms": -7.12, "doppler_434_khz": 10.32, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.44, "obs_144_mhz": 144.9034},
        {"time_sec": -160, "velocity_kms": -7.08, "doppler_434_khz": 10.28, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.42, "obs_144_mhz": 144.9034},
        {"time_sec": -150, "velocity_kms": -7.05, "doppler_434_khz": 10.23, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.41, "obs_144_mhz": 144.9034},
        {"time_sec": -140, "velocity_kms": -6.99, "doppler_434_khz": 10.15, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.38, "obs_144_mhz": 144.9034},
        {"time_sec": -130, "velocity_kms": -6.94, "doppler_434_khz": 10.06, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.35, "obs_144_mhz": 144.9034},
        {"time_sec": -120, "velocity_kms": -6.88, "doppler_434_khz":  9.98, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.33, "obs_144_mhz": 144.9033},
        {"time_sec": -110, "velocity_kms": -6.75, "doppler_434_khz":  9.80, "obs_434_mhz": 434.9098, "doppler_144_khz": 3.26, "obs_144_mhz": 144.9033},
        {"time_sec": -100, "velocity_kms": -6.63, "doppler_434_khz":  9.61, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.20, "obs_144_mhz": 144.9032},
        {"time_sec": -90, "velocity_kms": -6.50, "doppler_434_khz":  9.43, "obs_434_mhz": 434.9094, "doppler_144_khz": 3.14, "obs_144_mhz": 144.9031},
        {"time_sec": -80, "velocity_kms": -6.23, "doppler_434_khz":  9.04, "obs_434_mhz": 434.9090, "doppler_144_khz": 3.01, "obs_144_mhz": 144.9030},
        {"time_sec": -70, "velocity_kms": -5.97, "doppler_434_khz":  8.66, "obs_434_mhz": 434.9087, "doppler_144_khz": 2.88, "obs_144_mhz": 144.9029},
        {"time_sec": -60, "velocity_kms": -5.70, "doppler_434_khz":  8.27, "obs_434_mhz": 434.9083, "doppler_144_khz": 2.76, "obs_144_mhz": 144.9028},
        {"time_sec": -50, "velocity_kms": -5.10, "doppler_434_khz":  7.40, "obs_434_mhz": 434.9074, "doppler_144_khz": 2.47, "obs_144_mhz": 144.9025},
        {"time_sec": -40, "velocity_kms": -4.50, "doppler_434_khz":  6.53, "obs_434_mhz": 434.9065, "doppler_144_khz": 2.18, "obs_144_mhz": 144.9022},
        {"time_sec": -30, "velocity_kms": -3.90, "doppler_434_khz":  5.66, "obs_434_mhz": 434.9057, "doppler_144_khz": 1.89, "obs_144_mhz": 144.9019},
        {"time_sec": -20 ,"velocity_kms": -2.60, "doppler_434_khz":  3.77, "obs_434_mhz": 434.9038, "doppler_144_khz": 1.26, "obs_144_mhz": 144.9013},
        {"time_sec": -10 ,"velocity_kms": -1.30, "doppler_434_khz":  1.89, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.63, "obs_144_mhz": 144.9006},
        {"time_sec": 0 ,"velocity_kms":  0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10 ,"velocity_kms":  1.30, "doppler_434_khz": -1.89, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.63, "obs_144_mhz": 144.8994},
        {"time_sec": 20 ,"velocity_kms":  2.60, "doppler_434_khz": -3.77, "obs_434_mhz": 434.8962, "doppler_144_khz": -1.26, "obs_144_mhz": 144.8987},
        {"time_sec": 30 ,"velocity_kms":  3.90, "doppler_434_khz": -5.66, "obs_434_mhz": 434.8943, "doppler_144_khz": -1.89, "obs_144_mhz": 144.8981},
        {"time_sec": 40 ,"velocity_kms":  4.50, "doppler_434_khz": -6.53, "obs_434_mhz": 434.8935, "doppler_144_khz": -2.18, "obs_144_mhz": 144.8978},
        {"time_sec": 50 ,"velocity_kms":  5.10, "doppler_434_khz": -7.40, "obs_434_mhz": 434.8926, "doppler_144_khz": -2.47, "obs_144_mhz": 144.8975},
        {"time_sec": 60 ,"velocity_kms":  5.70, "doppler_434_khz": -8.27, "obs_434_mhz": 434.8917, "doppler_144_khz": -2.76, "obs_144_mhz": 144.8972},
        {"time_sec": 70 ,"velocity_kms":  5.97, "doppler_434_khz": -8.66, "obs_434_mhz": 434.8913, "doppler_144_khz": -2.88, "obs_144_mhz": 144.8971},
        {"time_sec": 80 ,"velocity_kms":  6.23, "doppler_434_khz": -9.04, "obs_434_mhz": 434.8910, "doppler_144_khz": -3.01, "obs_144_mhz": 144.8970},
        {"time_sec": 90 ,"velocity_kms":  6.50, "doppler_434_khz": -9.43, "obs_434_mhz": 434.8906, "doppler_144_khz": -3.14, "obs_144_mhz": 144.8969},
        {"time_sec": 100 ,"velocity_kms":  6.63, "doppler_434_khz": -9.61, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.20, "obs_144_mhz": 144.8968},
        {"time_sec": 110 ,"velocity_kms":  6.75, "doppler_434_khz": -9.80, "obs_434_mhz": 434.8902, "doppler_144_khz": -3.26, "obs_144_mhz": 144.8967},
        {"time_sec": 120 ,"velocity_kms":  6.88, "doppler_434_khz": -9.98, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.33, "obs_144_mhz": 144.8967},
        {"time_sec": 130 ,"velocity_kms":  6.94, "doppler_434_khz": -10.06, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.35, "obs_144_mhz": 144.8966},
        {"time_sec": 140 ,"velocity_kms":  6.99, "doppler_434_khz": -10.15, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.38, "obs_144_mhz": 144.8966},
        {"time_sec": 150 ,"velocity_kms":  7.05, "doppler_434_khz": -10.23, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.41, "obs_144_mhz": 144.8966},
        {"time_sec": 160 ,"velocity_kms":  7.08, "doppler_434_khz": -10.28, "obs_434_mhz": 434.8917, "doppler_144_khz": -3.42, "obs_144_mhz": 144.8966},
        {"time_sec": 170 ,"velocity_kms":  7.12, "doppler_434_khz": -10.32, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.44, "obs_144_mhz": 144.8966},
        {"time_sec": 180 ,"velocity_kms":  7.15, "doppler_434_khz": -10.37, "obs_434_mhz": 434.8896, "doppler_144_khz": -3.46, "obs_144_mhz": 144.8965}
    ],
	89: [
        {"time_sec": -180, "velocity_kms": -7.17, "doppler_434_khz": 10.40, "obs_434_mhz": 434.9104, "doppler_144_khz": 3.47, "obs_144_mhz": 144.9035},
        {"time_sec": -170, "velocity_kms": -7.13, "doppler_434_khz": 10.35, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.45, "obs_144_mhz": 144.9034},
        {"time_sec": -160, "velocity_kms": -7.09, "doppler_434_khz": 10.28, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.43, "obs_144_mhz": 144.9034},
        {"time_sec": -150, "velocity_kms": -7.04, "doppler_434_khz": 10.21, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.40, "obs_144_mhz": 144.9034},
        {"time_sec": -140, "velocity_kms": -6.97, "doppler_434_khz": 10.12, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.37, "obs_144_mhz": 144.9034},
        {"time_sec": -130, "velocity_kms": -6.90, "doppler_434_khz": 10.01, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.33, "obs_144_mhz": 144.9033},
        {"time_sec": -120, "velocity_kms": -6.81, "doppler_434_khz": 9.87, "obs_434_mhz": 434.9099, "doppler_144_khz": 3.29, "obs_144_mhz": 144.9033},
        {"time_sec": -110, "velocity_kms": -6.69, "doppler_434_khz": 9.71, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.24, "obs_144_mhz": 144.9032},
        {"time_sec": -100, "velocity_kms": -6.55, "doppler_434_khz": 9.51, "obs_434_mhz": 434.9095, "doppler_144_khz": 3.17, "obs_144_mhz": 144.9032},
        {"time_sec": -90, "velocity_kms": -6.38, "doppler_434_khz": 9.25, "obs_434_mhz": 434.9093, "doppler_144_khz": 3.08, "obs_144_mhz": 144.9031},
        {"time_sec": -80, "velocity_kms": -6.15, "doppler_434_khz": 8.93, "obs_434_mhz": 434.9089, "doppler_144_khz": 2.97, "obs_144_mhz": 144.9030},
        {"time_sec": -70, "velocity_kms": -5.87, "doppler_434_khz": 8.51, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.84, "obs_144_mhz": 144.9028},
        {"time_sec": -60, "velocity_kms": -5.49, "doppler_434_khz": 7.97, "obs_434_mhz": 434.9080, "doppler_144_khz": 2.66, "obs_144_mhz": 144.9027},
        {"time_sec": -50, "velocity_kms": -5.01, "doppler_434_khz": 7.26, "obs_434_mhz": 434.9073, "doppler_144_khz": 2.42, "obs_144_mhz": 144.9024},
        {"time_sec": -40, "velocity_kms": -4.37, "doppler_434_khz": 6.34, "obs_434_mhz": 434.9063, "doppler_144_khz": 2.11, "obs_144_mhz": 144.9021},
        {"time_sec": -30, "velocity_kms": -3.55, "doppler_434_khz": 5.15, "obs_434_mhz": 434.9051, "doppler_144_khz": 1.72, "obs_144_mhz": 144.9017},
        {"time_sec": -20, "velocity_kms": -2.53, "doppler_434_khz": 3.67, "obs_434_mhz": 434.9037, "doppler_144_khz": 1.22, "obs_144_mhz": 144.9012},
        {"time_sec": -10, "velocity_kms": -1.32, "doppler_434_khz": 1.92, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.64, "obs_144_mhz": 144.9006},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": 0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": 0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 1.32, "doppler_434_khz": -1.92, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.64, "obs_144_mhz": 144.8994},
        {"time_sec": 20, "velocity_kms": 2.53, "doppler_434_khz": -3.67, "obs_434_mhz": 434.8963, "doppler_144_khz": -1.22, "obs_144_mhz": 144.8988},
        {"time_sec": 30, "velocity_kms": 3.55, "doppler_434_khz": -5.15, "obs_434_mhz": 434.8949, "doppler_144_khz": -1.72, "obs_144_mhz": 144.8983},
        {"time_sec": 40, "velocity_kms": 4.37, "doppler_434_khz": -6.34, "obs_434_mhz": 434.8937, "doppler_144_khz": -2.11, "obs_144_mhz": 144.8979},
        {"time_sec": 50, "velocity_kms": 5.01, "doppler_434_khz": -7.26, "obs_434_mhz": 434.8927, "doppler_144_khz": -2.42, "obs_144_mhz": 144.8976},
        {"time_sec": 60, "velocity_kms": 5.49, "doppler_434_khz": -7.97, "obs_434_mhz": 434.8920, "doppler_144_khz": -2.66, "obs_144_mhz": 144.8973},
        {"time_sec": 70, "velocity_kms": 5.87, "doppler_434_khz": -8.51, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.84, "obs_144_mhz": 144.8972},
        {"time_sec": 80, "velocity_kms": 6.15, "doppler_434_khz": -8.93, "obs_434_mhz": 434.8911, "doppler_144_khz": -2.97, "obs_144_mhz": 144.8970},
        {"time_sec": 90, "velocity_kms": 6.38, "doppler_434_khz": -9.25, "obs_434_mhz": 434.8907, "doppler_144_khz": -3.08, "obs_144_mhz": 144.8969},
        {"time_sec": 100, "velocity_kms": 6.55, "doppler_434_khz": -9.51, "obs_434_mhz": 434.8905, "doppler_144_khz": -3.17, "obs_144_mhz": 144.8968},
        {"time_sec": 110, "velocity_kms": 6.69, "doppler_434_khz": -9.71, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.24, "obs_144_mhz": 144.8968},
        {"time_sec": 120, "velocity_kms": 6.81, "doppler_434_khz": -9.87, "obs_434_mhz": 434.8901, "doppler_144_khz": -3.29, "obs_144_mhz": 144.8967},
        {"time_sec": 130, "velocity_kms": 6.90, "doppler_434_khz": -10.01, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.33, "obs_144_mhz": 144.8967},
        {"time_sec": 140, "velocity_kms": 6.97, "doppler_434_khz": -10.12, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.37, "obs_144_mhz": 144.8966},
        {"time_sec": 150, "velocity_kms": 7.04, "doppler_434_khz": -10.21, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.40, "obs_144_mhz": 144.8966},
        {"time_sec": 160, "velocity_kms": 7.09, "doppler_434_khz": -10.28, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.43, "obs_144_mhz": 144.8966},
        {"time_sec": 170, "velocity_kms": 7.13, "doppler_434_khz": -10.35, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.45, "obs_144_mhz": 144.8966},
        {"time_sec": 180, "velocity_kms": 7.17, "doppler_434_khz": -10.40, "obs_434_mhz": 434.8896, "doppler_144_khz": -3.47, "obs_144_mhz": 144.8965}
	]			
}

def update_doppler(fm="yes"):

	try:
		global start_time
		global tx_doppler_freq_hz
		global rx_doppler_freq_hz
		global rxpl_value
		global txpl_value
		global sq
		global doppler_table
		global mode
		global tx
		global rxf
		global txr
		global rigctl
		print("update_doppler")

		with open("/home/pi/CubeSatSim/frequency.txt", "r") as file:
			frequencies = file.read().split()
		
		tx_frequency = int(frequencies[0])
		rx_frequency = int(frequencies[1])  # Not used right now as FT857 emulation only updates transmit frequency

		print(f"New TX Frequency: {tx_frequency}, new RX Frequency: {rx_frequency}")

		if rigctl:
			if (tx_frequency > 145955000) and (tx_frequency <= 145965000):
				tx_center = 145960000 # AO-91
				rx_center = 435250000 
				print("AO-91 detected!")
			elif (tx_frequency > 145965000) and (tx_frequency < 145975000):
				tx_center = 145960000 # AO-73 detection is shifted up 10kHz to avoid overlap with AO-91
				rx_center = 435140000 
				print("AO-73 detected!")				
			elif (tx_frequency > 145795000) and (tx_frequency < 145805000):	
				tx_center = 145800000 # ISS U/V Repeater
				rx_center = 437800000 
				print("ISS U/V detected!")				
			elif (tx_frequency > 145945000) and (tx_frequency <= 145955000):	
				tx_center = 145950000 # AO-7
				rx_center = 432150000 	
				print("AO-7 detected!")				
			else:
				tx_center = tx_frequency
				rx_center = rxf * 1e6 # default RX requency	
				print("No Satellite detected.  Using default RX frequency with no doppler shift")				

			if (rx_center != rxf * 1e6):	# don't Doppler shift default RX frequency since don't know the shift 
				tx_doppler_shift = tx_frequency - tx_center
				rx_doppler_shift = -1 * (rx_center/tx_center) * tx_doppler_shift
				new_tx_frequency = tx_frequency
				new_rx_frequency = rx_center + rx_doppler_shift
				print("Calculating RX frequency using offset from TX frequency!")
				print(f"Tx Doppler Shift: {tx_doppler_shift:.0f}  Frequency: {new_tx_frequency:.0f}")
				print(f"Rx Doppler Shift: {rx_doppler_shift:.0f}  Frequency: {new_rx_frequency:.0f}")
			else:
				new_tx_frequency = tx_frequency
				new_rx_frequency = rxf * 1e6 # default RX frequency or should we use same as tx frequency with doppler shift?
				print(f"Tx Frequency: {new_tx_frequency:.0f}")
				print(f"Rx Frequency: {new_rx_frequency:.0f}")
		else:
			new_tx_frequency = tx_frequency
			new_rx_frequency = tx_frequency
			print(f"Tx Frequency: {new_tx_frequency:.0f}")
			print(f"Rx Frequency: {new_rx_frequency:.0f}")			
		
		if (tx_doppler_freq_hz != new_tx_frequency) or (rx_doppler_freq_hz != new_rx_frequency):
			tx_doppler_freq_hz = new_tx_frequency
			rx_doppler_freq_hz = new_rx_frequency
			print("Applying Doppler shift!")
#			print(f"Tx Doppler shift: {tx_doppler_freq_hz:.0f}")
#			rx_doppler_freq_hz = rx_doppler_start_hz + rx_doppler_shift_hz
#			print(f"Tx Doppler shift: {tx_doppler_freq_hz:.0f}")
			rx = "{:.4f}".format(rx_doppler_freq_hz/1e6)
			if (mode != 'e'):
				tx = "{:.4f}".format(tx_doppler_freq_hz/1e6)
#					print(tx)
			else:
				txr = "{:.3f}".format(tx_doppler_freq_hz/1e6)
			if (fm != "no"):
				program_fm(rx,tx,rxpl_value,sq,txpl_value)
		else:
			print("Not applying Doppler shift!")
	except Exception as e:
		print(f"An error occurred: {e}")	
		print("update_doppler failed")

morse_table = [  # 0-9, A-Z only by (ASCII - 48)
  [ 3, 3, 3, 3, 3, 0 ],	# 0		
  [ 1, 3, 3, 3, 3, 0 ],	# 1		
  [ 1, 1, 3, 3, 3, 0 ],	# 2		
  [ 1, 1, 1, 3, 3, 0 ],	# 3	
  [ 1, 1, 1, 1, 3, 0 ],	# 4
  [ 1, 1, 1, 1, 1, 0 ],	# 5		
  [ 3, 1, 1, 1, 1, 0 ],	# 6		
  [ 3, 3, 1, 1, 1, 0 ],	# 7		
  [ 3, 3, 3, 1, 1, 0 ],	# 8	
  [ 3, 3, 3, 3, 1, 0 ],	# 9	
  [ 0, 0, 0, 0, 0, 0 ],	# -		   
  [ 0, 0, 0, 0, 0, 0 ],	# -		
  [ 0, 0, 0, 0, 0, 0 ],	# -		   
  [ 0, 0, 0, 0, 0, 0 ],	# -		   
  [ 0, 0, 0, 0, 0, 0 ],	# -		   
  [ 0, 0, 0, 0, 0, 0 ],	# -		
  [ 0, 0, 0, 0, 0, 0 ],	# -		  
  [ 1, 3, 0, 0, 0, 0 ],	# A
  [ 3, 1, 1, 1, 0, 0 ],	# B
  [ 3, 1, 3, 1, 0, 0 ],	# C	
  [ 3, 1, 1, 0, 0, 0 ],	# D	
  [ 1, 0, 0, 0, 0, 0 ],	# E		
  [ 1, 1, 3, 1, 0, 0 ],	# F		
  [ 3, 3, 1, 0, 0, 0 ],	# G	
  [ 1, 1, 1, 1, 0, 0 ],	# H	
  [ 1, 1, 0, 0, 0, 0 ],	# I	
  [ 1, 3, 3, 3, 0, 0 ],	# J		
  [ 3, 1, 3, 0, 0, 0 ],	# K	
  [ 1, 3, 1, 1, 0, 0 ],	# L
  [ 3, 3, 0, 0, 0, 0 ],	# M		
  [ 3, 1, 0, 0, 0, 0 ],	# N	
  [ 3, 3, 3, 0, 0, 0 ],	# O
  [ 1, 3, 3, 1, 0, 0 ],	# P
  [ 3, 3, 1, 3, 0, 0 ],	# Q
  [ 1, 3, 1, 0, 0, 0 ],	# R	
  [ 1, 1, 1, 0, 0, 0 ],	# S		
  [ 3, 0, 0, 0, 0, 0 ],	# T		
  [ 1, 1, 3, 0, 0, 0 ],	# U		
  [ 1, 1, 1, 3, 0, 0 ],	# V		
  [ 1, 3, 3, 0, 0, 0 ],	# W		
  [ 3, 1, 1, 3, 0, 0 ],	# X		
  [ 3, 1, 3, 3, 0, 0 ],	# Y	
  [ 3, 3, 1, 1, 0, 0 ]	# Z	
]

def cw_transmit_fm(morse, tx):
	global txLed
#	global no_command
	global debug_mode
#	global sim_mode
#	global callsign
#	global tx
#	global txr
	
	try:	
		output(txLed, 1)
		print("Transmit FM CW " + morse)
#		status = ""
#		if not no_command:
#			status = status + " C"
#		if sim_mode:
#			status = status + " S"
#		if (mode != 'e'):	
		if (debug_mode == 1):
			system("echo 'hi hi de " + morse + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
		else:
			system("echo 'hi hi de " + morse + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")
#		else:
#			if (debug_mode == 1):
#				system("echo 'hi hi de " + morse + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + txr + "e3")
#			else:
#				system("echo 'hi hi de " + morse + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + txr + "e3 > /dev/null 2>&1")
			
		output(txLed, 0)
	except Exception as e:
		print(f"An error occurred: {e}")	
		print("cw_transmit_fm failed")	
	
def cw_transmit_string(string):
	global morse_timing
	for character in string: 	
		if (character != ' '):
			update_doppler()
			cw_transmit_char(character);
		else:
			sleep(7.0 * morse_timing);
	
def cw_transmit_char(character): 	
	global morse_timing
#	update_doppler("no")
	i = 0
	duration = morse_table[(ord(character.upper()) - ord('0')) % 44][i]
	if duration == 1:
		duration = 1.2
	print(duration)
	while (duration != 0): 
		transmit_carrier(duration * morse_timing)	  
		sleep(morse_timing * 0.4)  # 6)
		i=i+1
		duration = morse_table[(ord(character.upper()) - ord('0')) % 44][i]
		if duration == 1:
			duration = 1.2
	sleep(morse_timing * 2.0) # 3) # 1.5);

def transmit_carrier(duration):
	global tx_doppler_freq_hz
	global txLed
	update_doppler("no")
	command = "timeout -k 0.5 " + str(duration) + " sudo tune -f " + str(tx_doppler_freq_hz) + " > /dev/null 2>&1" # 434.9e6
	start = "sudo tune -f " + str(tx_doppler_freq_hz) + " &" # + " & > /dev/null 2>&1" # 434.9e6
	stop = "sudo rpitx -f 434.9e6 &"
#	killrpitx = "sudo killall -9 rpitx &"
	killtune = "sudo killall -9 tune &"
	output(txLed, 1)
#	system(start)
	system(command)
#	sleep(duration)
#	system(stop)
	output(txLed, 0)
#	system(killrpitx)
#	system(killtune)

print("CubeSatSim v2.2 transmit.py starting...")

system("sudo systemctl restart frequency")

pd = 21
ptt = 20
txc_pin = 7
squelch = 6
green = 16
powerPin = 16
morse_timing = 0.09 # 0.1
two_meter_rpitx = False
restore_txc = False

command_tx = True

setup(13, "up")
setup(12, "up")
setup(27, "up")
setup(txc_pin, "up")
setup(green, "out")
output(powerPin, 1)

transmit = False
txLed = 27
if input(12) == False:
	print("LPF present")
	transmit = True
else:
	print("No LPF")

setup(txLed, "out")

setup(pd, "out")
#output(pd, 1)
output(pd, 0)
setup(ptt, "out")
output(ptt, 1)

txc = False
if input(txc_pin) == False:
	print("TXC is present")
	txc = True
else:
	print("TXC not present")

#  False  # forcing it off
output(txLed, 1)
sleep(1)
output(txLed, 0)

battery_saver_check()	

# print(1)
print(txLed)

debug_mode = 0  # change to 1 to debug transmit

skip = False

tx_doppler_freq_hz = 0
rx_doppler_freq_hz = 0

if __name__ == "__main__":
	mode = "y"
	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if (('d' == sys.argv[1]) or ('-d' in sys.argv[1])):
			debug_mode = 1
		elif (('x' == sys.argv[1]) or ('-x' in sys.argv[1])):	
			mode = "x"
			print("Forcing APRS mode")
		elif (('s' == sys.argv[1]) or ('-s' in sys.argv[1])):
			skip = True
			print("Skipping delay and CW ID")
			
	print(transmit)

	uptime_time = 55  #  55 second boot time if Pi Zero 
	try:
		f = open("/home/pi/CubeSatSim/pi_zero2", "r")
		f.close()
		print("Pi Zero 2 detected!")
		uptime_time = 30  # 30 second boot time if Pi Zero 2
	except:
		print("Pi Zero 2 not detected")

	try:
		system("sudo cat /proc/uptime > /home/pi/CubeSatSim/uptime")
		file = open("/home/pi/CubeSatSim/uptime")
		up = file.read().split(" ")[0]
		print(up)
		uptime = float(up)
		print(uptime)
		if (uptime < uptime_time):
			print("Uptime < threshold seconds")
		else:
			print("Uptime > threshold seconds")
			print("Skip CW ID")
			skip = True
		file.close() 
	except:
		print("Can't open /proc/uptime") 
	print(skip)	

	if ( mode == "y"):
		try:
			file = open("/home/pi/CubeSatSim/.mode")
			mode = file.read(1)
		except:
			mode = "f"
			if (debug_mode == 1):
				print("Can't open .mode file, defaulting to FSK")
	print("Mode is: ")
	print(mode)

	try:
		file = open("/home/pi/CubeSatSim/beacon_off")
		file.close()
		command_tx = False
	except:
		command_tx = True
		if (debug_mode == 1):
			print("Can't open beacon_off file, defaulting to False")
	print("Command_tx: ")
	print(command_tx)

	if (mode != "p") or (command_tx == False):
		system("sudo systemctl stop pacsatsim")
	
	try:
		file = open("/home/pi/CubeSatSim/command_count.txt", "r")
		string = file.read()
		command_count = int(string)
	except:
		command_count = 0
		if (debug_mode == 1):
			print("Can't open command_count file, setting to 0")
		file = open("/home/pi/CubeSatSim/command_count.txt", "w")
		count_string = str(command_count)
		file.write(count_string)
		file.close()
	print("Command_count: ")
	print(command_count)	

	tx_value = '0'
	rx_value = '0'
	sq = '0'
	tx = '434.9000'	
	rx = '435.0000'
	txr = '144.9000'
	sim_mode = False
	sim_config = False
	hab_mode = False
	doppler_mode = False
	rigctl = False
	
	try:
		file = open("/home/pi/CubeSatSim/sim.cfg")
#		callsign = file.readline().split(" ")[0]
		config = file.readline().split()		
		callsign = config[0]
		if len(config) > 4:
			if config[4] == 'y' or config[4] == 'yes':		
				sim_mode = True
				sim_config = True
				print("Simulated telemetry mode is configured")
			else:
				try:
					if path.isfile("/home/pi/CubeSatSim/sim_mode_auto"):
						print("Simulated telemetry mode automatically turned on!")
						sim_mode = True
						sim_config = True
				except:
					if (debug_mode == 1):
						print("/home/pi/CubeSatSim/sim_mode_auto not found")	
		if len(config) > 5:		
			if (mode != 'p') and (mode != 'P'): # squelch off for Pacsat	
				sq = config[5] 		
			print(f'squelch: {sq}')
		if len(config) > 6:
			txf = float(config[6])
			tx = "{:.4f}".format(txf)
			if (mode == 'e'):
				txrf = (txf - 290.0) # - 0.1 # Cross Band Repeater mode transmit frequency in 2m band
				txr = "{:.4f}".format(txrf)
				print("Transmit frequency: ",txr)
			else:
				print("Transmit frequency: ",tx)
		if len(config) > 7:
			rxf = float(config[7])
			rx = "{:.4f}".format(rxf)
			print(rx)
		if len(config) > 8:
			if config[8] == 'y' or config[8] == 'yes':		
				hab_mode = True
				print("Balloon (HAB) mode is configured.")				
		if len(config) > 9:
			rxpl = float(config[9])
#                       print(rxpl)
#                       print( "{:.0f}".format(rxpl))
			rxpl_value = "{:.0f}".format(rxpl)
			print(rxpl_value)
		if len(config) > 10:
			txpl = float(config[10])
#                        print(txpl)
#                        print( "{:.0f}".format(txpl))
			txpl_value = "{:.0f}".format(txpl)
			print(txpl_value)
		if len(config) > 13:
			if config[13] == 'sim':		
				doppler_mode = True
#				system("sudo systemctl restart frequency")
				print("Simulated Doppler frequency shift is enabled.")				
			elif config[13] == 'rig':	
				doppler_mode = True
				rigctl = True
#				system("sudo systemctl restart frequency")
				print("rigctl Doppler frequency shift is enabled.")	
#			else:
#				system("sudo systemctl stop rigctld")
		else:
			system("sudo systemctl stop rigctld")
		print(config)
		print
#		print(callsign)
		print(sq)
#		if sq == '8':
#			print("squelch set to 8, no command input!")
#			no_command = True
#		else:
		no_command = False		
		print(no_command)
	except:
		callsign = "AMSAT"
		if (debug_mode == 1):
			print("Can't read callsign from sim.cfg file, defaulting to AMSAT")	
	file.close()

	no_command = True
	try:
		f = open("/home/pi/CubeSatSim/command_control", "r")
		no_command = False
		f.close()
#		setup(squelch, "up")  ## pull up in case pin is not connected
#		if input(squelch) == False:
#			print("squelch not set correctly, no command input!")
#		else:
#			if (mode != 'n') and (mode != 'x'):
#				print("command and control is activated")
#				no_command = False
#				system("sudo systemctl start command")
#			else:
#				print("Command and control not activated since Transmit Commands mode")		
#				txc = True # Transmit commands only works with FM transceiver, so bypass Battery Saver if activated
	except:
		print("command and control not activated")

	print(callsign)

	query = ["grep", "VERSION_CODENAME=bullseye", "/etc/os-release"] 
	try:
		result = subprocess.run(query, capture_output=True, text=True, check=True)
		print(f"Command run was: {query}")
		os_status = result.stdout.strip()
		print(f"Output of the command (stdout): {os_status}")
	except subprocess.CalledProcessError as e:
#		print(f"Command failed with return code: {e.returncode}")
		print(f"Command run was: {e.cmd}")
		os_status = e.stdout.strip()
		print(f"Output of the command (stdout): {e.stdout}")
#		print(f"Error output of the command (stderr): {e.stderr}")
	if os_status != "VERSION_CODENAME=bullseye":
		os_status = "bookworm"
	else:
		os_status = "bullseye"
	print (os_status)
	
	card = "Headphones"  # default using pcm audio output of Pi Zero
#	card = "Device" # using USB sound card for audio output	

	query = ["sudo", "systemctl", "is-active", "gpsd.socket"]
	try:
		result = subprocess.run(query, capture_output=True, text=True, check=True)
		print(f"Command run was: {query}")
		gpsd_status = result.stdout.strip()
		print(f"Output of the command (stdout): {gpsd_status}")
	except subprocess.CalledProcessError as e:
#		print(f"Command failed with return code: {e.returncode}")
		print(f"Command run was: {e.cmd}")
		gpsd_status = e.stdout.strip()
		print(f"Output of the command (stdout): {e.stdout}")
#		print(f"Error output of the command (stderr): {e.stderr}")
  	
#	if (mode != 'e'): 
	program_fm(rx,tx,rxpl_value,sq,txpl_value)	

	if (doppler_mode == True):	

		cw_transmit_fm("QSY", tx)
		if (mode == 'e'):
			tx_doppler_start_hz = txrf * 1e6
		else:
			tx_doppler_start_hz = txf * 1e6
		tx_doppler_shift_hz = 0
		print(f"Tx center frequency: {tx_doppler_start_hz}")
		rx_doppler_start_hz = rxf * 1e6
		rx_doppler_shift_hz = 0
		print(f"Rx center frequency: {rx_doppler_start_hz}")

		TARGET_PASS = 85           # Maximum elevation profile
	
		doppler_table = iss_doppler_passes[TARGET_PASS]
		print(f"Pass Max Elevation: {TARGET_PASS}°")

		print("MODE: ")
		print(mode)

		update_doppler()
		if (mode == 'm'):
			print("Sleeping 15 seconds")
			sleep(15) # avoid CW jumping around at start
		
	sim_failure_check()

	status = ""
	if not no_command:
		status = status + " C"
	if sim_mode:
		status = status + " S"
	
	if (hab_mode == True) and (mode == 'a'):
		print("Don't transmit CW ID since APRS HAB mode is active")
	else:	
		if (((mode == 'a') or (mode == 'b') or (mode == 'f') or (mode == 's') or (mode == 'j') or (mode == 'p') or (mode == 'P')) and (command_tx == True) and (skip == False)) or ((mode == 'e') and (command_tx == True)):	#		battery_saver_mode
			
#			output(txLed, 1)
#			print("Transmit CW ID")

			if (mode != 'e'):
				cw_transmit_fm(callsign + status, tx)
#				if (debug_mode == 1):
#					system("echo 'hi hi de " + callsign + status + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
#				else:
#					system("echo 'hi hi de " + callsign + status + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")
			else:
				cw_transmit_fm(callsign + status, txr)
#				if (debug_mode == 1):
#					system("echo 'hi hi de " + callsign + status + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + txr + "e3")
#				else:
#					system("echo 'hi hi de " + callsign + status + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + txr + "e3 > /dev/null 2>&1")
#				
#			output(txLed, 0)
	
			sleep(1)
		else:
			print("Don't transmit CW ID since command_tx is False or APRS mode or change of mode")
	
		
	if (transmit):
#		print 'Length: ', len(sys.argv)
    
#		if (len(sys.argv)) > 1:
#        		print("There are arguments!")
		if (mode == 'a') or (mode == 'x') or (mode == 'n') or (mode == 'p') or (mode == 'P'):
#			command_control_check()	
			output(pd, 1)
			output(ptt, 1)
			if (mode == 'a'):
				print("AFSK")
			elif (mode == 'p') or (mode == 'P'):
				if (mode == 'P'):
					print("Pacsat Ground Station")
				else:
					print("Pacsat")
					if (command_tx == True):
						system("sudo systemctl restart pacsatsim")
						print("Starting PacSatSim")
				output(txLed, 0)
				print("0")
				rpitx = "arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1 &"
#				stop_rpitx = "sudo killall -9 rpitx && sudo killall -9 arecord && sudo rpitx -m RF -f  434.9e3 > /dev/null 2>&1"
				stop_rpitx = "sudo killall -9 arecord && sudo rpitx -m RF -f  434.9e3 > /dev/null 2>&1"
				if not txc:
					print("txc is:")
					print(txc)
					system(stop_rpitx)
#					system(rpitx)	
					print("Safe Mode!!")
				while (True):
					if (txc):
						sleep(0.1)
						while (input(ptt) != 0):
							sleep(0.2)
						output(txLed, 1)
#						print("1")
						while (input(ptt) != 1):
							sleep(0.2)					
						output(txLed, 0)
						if (mode == 'p') and (doppler_mode):
							update_doppler()
#						print("0")
					else:
#						sleep(0.1)
						while (input(17) == 0):
							sleep(0.05)
						system(rpitx)
						output(txLed, 1)
#						print("1")
						while (input(17) == 1):
							sleep(0.05)					
						system(stop_rpitx)
						output(txLed, 0)
						if (mode == 'p') and (doppler_mode):
							update_doppler() # "no")						
							rpitx = "arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1 &"

#						print("0")						
						
#						sleep(10)

			else:
				print("Transmit APRS Commands")
				system("sudo systemctl stop command")
#			while True:
#				sleep(0.1)
#			if (mode != 'n'):
#				system("touch /home/pi/CubeSatSim/ready")

			while True:
				try:
					f = open("/home/pi/CubeSatSim/ready")
					f.close()
					if (debug_mode == 1):
						print("Packet ready!")
					system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1")
					system("cat /home/pi/CubeSatSim/t.txt")
					if (command_tx == True):
						output(txLed, 1)
						if (doppler_mode == True):
							update_doppler()
							txf = tx_doppler_freq_hz / 1e6
							tx = "{:.4f}".format(txf)
						if (txc):
							sim_failure_check()
#							output(pd, 1)
							sleep(0.1) # add delay before transmit
							output (ptt, 0)
							sleep(0.3)   # add even more time at start
							system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/telem.wav")
							sleep(0.2)  # add more time at end
							output (ptt, 1)
#							output(pd, 0)							
						else:
#							system("echo 'AMSAT-11>APCSS:010101/hi hi ' >> t.txt")
							system("echo 'AMSAT-11>APCSS:010101/hi hi ' | sudo tee -a /home/pi/CubeSatSim/t.txt")

							if (debug_mode == 1):
								system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")
					
						sleep(0.1)  
#						output (ptt, 1)
#						output(pd, 0)
						output(txLed, 0)
						system("sudo rm /home/pi/CubeSatSim/ready")
					f.close()
				
					if (debug_mode == 1):
						print("Ready for next packet!")
						
					sleep(0.5)
	
				except Exception as e:
#					print(f"An error occurred: {e}")	
#					command_control_check()
					sleep(1)
		elif (mode == 'm'):
#			system("touch /home/pi/CubeSatSim/cwready")
			print("CW")
			while True:
#				command_control_check()
				output (pd, 1)
				output (ptt, 1)					
				try:
					f = open("/home/pi/CubeSatSim/cwready")
					f.close()
					system("sudo rm /home/pi/CubeSatSim/cwready")
##					ch = 1
					for chan in range(7):
						if (doppler_mode):
							try:
#								update_doppler()
								filename="/home/pi/CubeSatSim/cw" + str(chan) + ".txt"
								print(filename)
								file = open(filename)
								cw_string = file.readline()
								print(cw_string)
								cw_transmit_string(cw_string)
							except Exception as e:
								print(f"An error occurred: {e}")
								print("error reading cw string " + str(chan))
						else:	
							command = "gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw" + str(chan) + ".txt -r 48000 > /dev/null 2>&1"
							print(command)
							system(command)
	##						chan = chan + 1						
							if (command_tx == True):
								output(txLed, 1)					
								if (doppler_mode == True):
									update_doppler()
									txf = tx_doppler_freq_hz / 1e6
									tx = "{:.4f}".format(txf)
								if (txc):
									sim_failure_check()
	#								output (pd, 1)
									sleep(0.3)
									output (ptt, 0)	
									system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/morse.wav")
									sleep(0.1)
									output (ptt, 1)
	#								output (pd, 0)
								else:
									if (debug_mode == 1):
										system("cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
									else:
										system("cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")		
								output(txLed, 0)
								
#						command_control_check()
						sleep(2)
					f.close()
					sleep(10)
				except:	
#					command_control_check()
#					print("cw not ready")
					sleep(1)
		elif (mode == 's'):
			print("SSTV")
#			command_control_check()	
			output (ptt, 1)
			output(pd, 1)			
			try: 
#				from picamera import PiCamera
#					from pysstv.sstv import SSTV
#				camera = PiCamera()
				print("Testing for camera")
				if os_status == "bookworm":
					system("rpicam-still -o /home/pi/CubeSatSim/camera_out.jpg --width 320 --height 256") #  > /dev/null 2>&1")
				else:
					system("raspistill -o /home/pi/CubeSatSim/camera_out.jpg -w 320 -h 256")			
				f = open("/home/pi/CubeSatSim/camera_out.jpg")
				f.close()
				print("Camera present")
				camera_present = 1
#				camera.close()
			except:
				print("No camera available")
				print(" -> if camera plugged in, is software enabled?")
				camera_present = 0

#				while 1:
			output(txLed, 0)
#			output (ptt, 1)
#			output(pd, 0)
			if (camera_present == 1):
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg") 
#					command_control_check()	
					
					if (command_tx == True):
						print ("Sending SSTV image")
						if (doppler_mode):
							update_doppler()
							txf = tx_doppler_freq_hz / 1e6
							tx = "{:.4f}".format(txf)						
						output(txLed, 1)
#						battery_saver_check()

						if (txc):
							sim_failure_check()
#							output(pd, 1)
							output (ptt, 0)
							system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav")
							output (ptt, 1)
#							output(pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, 0)

	#					sleep(1)
				except:
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
				while 1:
#					command_control_check()			
					camera_photo()					
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/camera_out.jpg") 
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg > /dev/null 2>&1") 

					if (command_tx == True):
						print ("Sending SSTV image")
						if (doppler_mode):
							update_doppler()
							txf = tx_doppler_freq_hz / 1e6
							tx = "{:.4f}".format(txf)
						
						output(txLed, 1)
#						battery_saver_check()

						if (txc):
#							print(card)
							sim_failure_check()
#							output(pd, 1)
							output (ptt, 0)
							system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/camera_out.jpg.wav")	
							output(ptt, 1)
#							output (pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, 0)
#						output (ptt, 1)
					
#					output(pd, 0)
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg.wav > /dev/null 2>&1") 
					sleep(10)
			else:
				try:
	#				command_control_check()			
					
					file = open("/home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg") 

#					command_control_check()			
					
					if (command_tx == True):

						print ("Sending SSTV image")
						if (doppler_mode):
							update_doppler()
							txf = tx_doppler_freq_hz / 1e6
							tx = "{:.4f}".format(txf)							
						output(txLed, 1)

#						battery_saver_check()

						if (txc):
							sim_failure_check()
#							output(pd, 1)
							output (ptt, 0)
							system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav")
							output(ptt, 1)
#							output (pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, 0)
#						output (ptt, 1)
#						output(pd, 0)
					sleep(1)
				except:
					print("image 1 did not load - copy from CubeSatSim/sstv directory")
				try:
#					command_control_check()			
					
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("Second SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")

					while 1:
						if (command_tx == True):
							print ("Sending SSTV image")
							if (doppler_mode):
								update_doppler()
								txf = tx_doppler_freq_hz / 1e6
								tx = "{:.4f}".format(txf)
							
							output(txLed, 1)
#							battery_saver_check()

							if (txc):
								sim_failure_check()
#								output(pd, 1)
								output (ptt, 0)
								system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav")
								output(ptt, 1)
#								output (pd, 0)
							else:
								if (debug_mode == 1):
									system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3") 
								else:
									system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

							output(txLed, 0)
#							output (ptt, 1)
#							output(pd, 0)
						sleep(10)
				except:	
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
					if (txc == False):
						if (command_tx == True):
							system("(while true; do (sleep 10 && cat /home/pi/CubeSatSim/wav/sstv.wav); done) | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 &")
					while 1:
						if (command_tx == True):
							if (doppler_mode):
								update_doppler()
								txf = tx_doppler_freq_hz / 1e6
								tx = "{:.4f}".format(txf)							
							output(txLed, 1)
						
							if (txc):
								sim_failure_check()
#								output(pd, 1)
								output (ptt, 0)		
								system("aplay -D plughw:CARD=" + card + ",DEV=0 /home/pi/CubeSatSim/sstv.wav")
								output(ptt, 1)
#								output (pd, 0)		
							else:
								sleep(60)
							
							output(txLed, 0)
#							output (ptt, 1)
#							output(pd, 0)
						sleep(10)
						
		elif (mode == 'b') or (mode == 'j'):
#			command_control_check()	
			if (mode == 'b'):
				print("BPSK")		
			else:
				print("FUNcube")
			if (doppler_mode):
				print("No doppler shift in this mode")					
			print("turn on FM rx")
			output(pd, 1)
			output(ptt, 1)
			
			if (command_tx == True):
#				system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.9e6 -t float &")
				system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f " + tx + "e6 -t float &")
			print("Turning LED on/off and listening for carrier")
			image_id = random.randint(0, 255)
			print("Initial image_id: " + str(image_id) + "\n")
			while 1:
#				print ("LED on")
				output(txLed, 0)
				sleep(0.4)
#				if (command_tx == False):
#					output(txLed, 1)
#					sleep(0.03)
#					output(txLed, 0)
#				command_control_check()
				
				if (command_tx == True):		
					output(txLed, 1)

				if (mode == 'b'):
					sleep(4.2)	
				else:  # FUNcube mode image
					for i in range(4):
#						print("Checking image_file.bin")
						try:
							file = open("/home/pi/CubeSatSim/image_file.bin")
							file.close()
	#						image_present = True
							sleep(1.0)
						except:
	#						image_present = False
						
	#					if (image_present == False):
							start = time.perf_counter()
							camera_photo()
							system("/home/pi/ssdv/ssdv -e -n -i " + str(image_id) + " -q 3 -J /home/pi/CubeSatSim/camera_out.jpg /home/pi/CubeSatSim/image_file.bin")
							print("image_id: " + str(image_id) + "\n")
							image_id = ( image_id + 1 ) % 256
							print("new image_id: " + str(image_id) + "\n")
							elapsed_time = time.perf_counter() - start
							print("Elapsed time: ")
							print(elapsed_time)
							if (elapsed_time < 9):
								sleep(9 - time.perf_counter() + start)
	#					else:	
					sleep(0.6)
		elif (mode == 'e'):  # code based on https://zr6aic.blogspot.com/2016/11/creating-2m-fm-repeater-with-raspberry.html
			print("Cross Band Repeater Mode")
#			print("Stopping command and control")
#			system("sudo systemctl stop command")
			print("turn on FM rx")
			output(pd, 1)
			output(ptt, 1)
			setup(squelch, "up")  ## pull up in case pin is not connected	
#			txf = float(tx) - 288.9
#			print("Transmit frequency: ",txf)
			if (command_tx != True):
				print("Beacon mode off so no repeater transmission")

			print("Ready to detect carrier")
#			if (doppler_mode):
#				update_doppler()
#				txr = "{:.3f}".format(tx_doppler_freq_hz/1e6)
#			cw_transmit_fm(callsign + status, txr)	
			start_time = time.perf_counter()
						
			while True:
				if (input(squelch) == False) and (command_tx == True):
					print("Carrier detected")
					if (doppler_mode):
						update_doppler() 
						start_repeater(tx_doppler_freq_hz)
					else:
						start_repeater(txrf * 1e6)
					while (input(squelch) == False):
						sleep(1)
					print("No carrier detected")
					stop_repeater()
				if ((time.perf_counter() - start_time) > 10) and doppler_mode:
					update_doppler()
					start_time = time.perf_counter()
		else:
			print("FSK") 
			if (doppler_mode):
				print("No doppler shift in this mode")				
			print("turn on FM rx")
			output(pd, 1)
			output(ptt, 1)
			
			if (command_tx == True):
				system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 &")
			print("Turning LED on/off and listening for carrier")
			while 1:
				output(txLed, 0)
				sleep(0.4)
#				if (command_tx == False):
#					output(txLed, 1)
#					sleep(0.03)
#					output(txLed, 0)
#				command_control_check()
				if (command_tx == True):		
					output(txLed, 1)
				sleep(4.2)
	else:
		print("No Low Pass Filter so no telemetry transmit.  See http://cubesatsim.org/wiki for instructions on how to build the LPF.")
		while 1:
			sleep(5)
