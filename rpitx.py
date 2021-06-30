#!/usr/bin/env python

import RPi.GPIO as GPIO
from RPi.GPIO import output
#import subprocess
#import time
from time import sleep
#import os
import sys
from os import system

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_UP)

transmit = False
if GPIO.input(12) == False:
	print("Version is v1 with UHF BPF")
	transmit = True
	txLed = 27
	txLedOn = 1
	txLedOff = 0
elif GPIO.input(27) == False:
	print("Version is TFB")
	transmit = True
	txLed = 22
	txLedOn = 0
	txLedOff = 1
elif GPIO.input(13) == False:
	print("Version is v1 with VHF BPF")
	print("VHF transmit not implemented yet")
#	transmit = True
	txLed = 27
	txLedOn = 1
	txLedOff = 0
else:
	print("No BPF")
	txLed = 27
	txLedOn = 1 
	txLedOff = 0

GPIO.setup(txLed, GPIO.OUT)
output(txLed, txLedOff)

# print(txLedOn)
print(txLed)
# GPIO.setup(27, GPIO.OUT)
# GPIO.output(27, 0)

debug_mode = 0

if __name__ == "__main__":

	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if (('d' == sys.argv[1]) or ('-d' in sys.argv[1])):
			debug_mode = 1
			
	print(transmit)

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
		file = open("/home/pi/CubeSatSim/sim.cfg")
		callsign = file.readline().split(" ")[0]
	except:
		callsign = "AMSAT"
		if (debug_mode == 1):
			print("Can't read callsign from sim.cfg file, defaulting to AMSAT")		
	print(callsign)

	sleep(1)
	output(txLed, txLedOn)
	if (debug_mode == 1):
		system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
	else:
		system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
		
	sleep(4); # was 8
	output(txLed, txLedOff)

	sleep(1)

	
	if (transmit):
	
#		print 'Length: ', len(sys.argv)
    
#		if (len(sys.argv)) > 1:
#        		print("There are arguments!")
		if (mode == 'a'):
			print("AFSK")
			sleep(5)
			try:
				file = open("/home/pi/CubeSatSim/t.txt")
				file.close()
			except:
				system("echo '" + callsign + "-11>APCSS:hi hi 100 199 199 199 298 299 299 278 380 350 300 300 439 400 400 400 500 500 500 500 600 600 600 650' > /home/pi/CubeSatSim/t.txt && echo 'AMSAT>APCSS:010101/hi hi ' >> /home/pi/CubeSatSim/t.txt")
			for x in range(5):
				output(txLed, txLedOn)
				if (debug_mode == 1):
					system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
				else:
					system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")					
				sleep(0.2)
				output(txLed, txLedOff)
				sleep(3.8)
			while True:
				try:
					f = open("/home/pi/CubeSatSim/ready")
					output(txLed, txLedOn)
					if (debug_mode == 1):
						system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
					else:
						system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					output(txLed, txLedOff)
					f.close()
					system("sudo rm /home/pi/CubeSatSim/ready")
					sleep(0.5)
				except:		  
					sleep(0.5)
		elif (mode == 'm'):
			print("CW")
#			sleep(4)
			try:
				file = open("/home/pi/CubeSatSim/cw.txt")
				file.close()
			except:
				system("echo 'hi hi 100 199 199 199 298 299 299 278 380 350 300 300 439 400 400 400 500 500 500 500 600 600 600 650' > /home/pi/CubeSatSim/cw.txt")
			output(txLed, txLedOn)
			if (debug_mode == 1):
				system("gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
			else:
				system("gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
			output(txLed, txLedOff)

			while True:
				try:
					f = open("/home/pi/CubeSatSim/cwready")
					output(txLed, txLedOn)
					if (debug_mode == 1):
						system("gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
					else:
						system("gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					output(txLed, txLedOff)
					f.close()
					system("sudo rm /home/pi/CubeSatSim/cwready")
					sleep(1)
				except:		  
					sleep(1)
		elif (mode == 's'):
			print("SSTV")
			try: 
				from picamera import PiCamera
#					from pysstv.sstv import SSTV
				camera = PiCamera()
				print("Camera present")
				camera_present = 1
				camera.close()
			except:
				print("No camera available")
				print(" -> if camera plugged in, is software enabled?")
				camera_present = 0

#				while 1:
			output(txLed, txLedOff)
			if (camera_present == 1):
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg") 
					print ("Sending SSTV image")
					output(txLed, txLedOn)
					if (debug_mode == 1):
						system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3")
					else:
						system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					output(txLed, txLedOff)
	#					sleep(1)
				except:
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
				while 1:
					system("raspistill -o /home/pi/CubeSatSim/camera_out.jpg -w 320 -h 256") #  > /dev/null 2>&1")
					print("Photo taken")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/camera_out.jpg") 
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg > /dev/null 2>&1") 
					print ("Sending SSTV image")
					output(txLed, txLedOn)
					if (debug_mode == 1):
						system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3")
					else:
						system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					output(txLed, txLedOff)
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg.wav > /dev/null 2>&1") 
					sleep(1)
			else:
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg") 
					print ("Sending SSTV image")
					output(txLed, txLedOn)
					if (debug_mode == 1):
						system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3")
					else:
						system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					output(txLed, txLedOff)
					sleep(1)
				except:
					print("image 1 did not load - copy from CubeSatSim/sstv directory")
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("Second SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					while 1:
						print ("Sending SSTV image")
						output(txLed, txLedOn)
						if (debug_mode == 1):
							system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3") 
						else:
							system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
						output(txLed, txLedOff)
						sleep(5)
				except:	
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
					system("(while true; do (sleep 5 && cat /home/pi/CubeSatSim/wav/sstv.wav); done) | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 &")
					while 1:
						output(txLed, txLedOn)
						sleep(60)
						output(txLed, txLedOff)
						sleep(1)

		elif (mode == 'b'):
			print("BPSK")
			system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.9e6 -t float &")
			print("Turning LED on/off")
			while 1:
				output(txLed, txLedOff)
				sleep(0.5)
				output(txLed, txLedOn)
				sleep(4.0)
		else:
			print("FSK") 
			system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 &")
			print("Turning LED on/off")
			while 1:
				output(txLed, txLedOff)
				sleep(0.5)
				output(txLed, txLedOn)
				sleep(4.0)
#		else:
#			print("FSK") 
#			system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")

	else:
		print("No Band Pass Filter so no telemetry transmit.  See http://cubesatsim.org/wiki for instructions on how to build the BPF.")
		while 1:
			sleep(5)
