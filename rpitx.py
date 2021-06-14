#!/usr/bin/env python

import RPi.GPIO as GPIO
import subprocess
import time
#import os
import sys
from os import system

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_UP)

transmit = False
if GPIO.input(12) == False:
	print("VHF vB5 or Later")
	transmit = True
	txLed = 22
	txLedOn = 1
	txLedOff = 0
elif GPIO.input(27) == False:
	print("TFB")
	transmit = True
	txLed = 22
	txLedOn = 0
	txLedOff = 1
elif GPIO.input(22) == False:
	print("vB5 or Later")
	transmit = True
	txLed = 27
	txLedOn = 0
	txLedOff = 1
else:
	print("No BPF")
	txLed = 27
	txLedOn = 1 
	txLedOff = 0

GPIO.setup(txLed, GPIO.OUT)
print(txLedOn)
print(txLed)
GPIO.setup(27, GPIO.OUT)
GPIO.output(27, 0)

print(transmit)

try:
	file = open("/home/pi/CubeSatSim/.mode")
	mode = file.read(1)
except:
	mode = "f"
print("Mode is: ")
print(mode)

try:
	file = open("/home/pi/CubeSatSim/sim.cfg")
	callsign = file.readline().split(" ")[0]
except:
	callsign = "AMSAT"
print(callsign)

#GPIO.output(27, 1);
GPIO.output(txLed, txLedOn);
system("echo 'de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
#GPIO.output(27, 0);
GPIO.output(txLed, txLedOff);

time.sleep(2)

if __name__ == "__main__":
	
	if (transmit):
	
#		print 'Length: ', len(sys.argv)
    
#		if (len(sys.argv)) > 1:
#        		print("There are arguments!")
		if (mode == 'a'):
			print("AFSK")
			time.sleep(4)
			for x in range(5):
				GPIO.output(txLed, txLedOn);
				system("sudo gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
				GPIO.output(txLed, txLedOff);
				time.sleep(4)
			while True:
				try:
					f = open("/home/pi/CubeSatSim/ready")
					GPIO.output(txLed, txLedOn);
					system("sudo gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
					GPIO.output(txLed, txLedOff);
					f.close()
					system("sudo rm ready")
					time.sleep(0.5)
				except:		  
					time.sleep(0.5)
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
			GPIO.output(txLed, txLedOff)
			if (camera_present == 1):
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg") 
					print ("Sending SSTV image")
					GPIO.output(txLed, txLedOn);		
					system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3") #  > /dev/null 2>&1")
					GPIO.output(txLed, txLedOff)
	#					time.sleep(1)
				except:
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
				while 1:
					system("raspistill -o /home/pi/CubeSatSim/camera_out.jpg -w 320 -h 256") #  > /dev/null 2>&1")
					print("Photo taken")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/camera_out.jpg") 
					print ("Sending SSTV image")
					GPIO.output(txLed, txLedOn);		
					system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3") #  > /dev/null 2>&1")
					GPIO.output(txLed, txLedOff)
					time.sleep(1)
			else:
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg") 
					print ("Sending SSTV image")
					GPIO.output(txLed, txLedOn);		
					system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3") #  > /dev/null 2>&1")
					GPIO.output(txLed, txLedOff)
					time.sleep(1)
				except:
					print("image 1 did not load - copy from CubeSatSim/sstv directory")
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("Second SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					while 1:
						print ("Sending SSTV image")
						GPIO.output(txLed, txLedOn);		
						system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3") #  > /dev/null 2>&1")
						GPIO.output(txLed, txLedOff)
						time.sleep(5)
				except:	
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
					system("(while true; do (sleep 5 && cat /home/pi/CubeSatSim/wav/sstv.wav); done) | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 &")
					while 1:
						GPIO.output(txLed, txLedOn)
						time.sleep(60)
						GPIO.output(txLed, txLedOff)
						time.sleep(1)

		elif (mode == 'b'):
			print("BPSK")
			system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.9e6 -t float &")
			print("Turning LED on/off")
			while 1:
				GPIO.output(txLed, txLedOff)
				time.sleep(0.5)
				GPIO.output(txLed, txLedOn)
				time.sleep(4.0)
		else:
			print("FSK") 
			system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 &")
			print("Turning LED on/off")
			while 1:
				GPIO.output(txLed, txLedOff)
				time.sleep(0.5)
				GPIO.output(txLed, txLedOn)
				time.sleep(4.0)
#		else:
#			print("FSK") 
#			system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")

	else:
		print("No Band Pass Filter so no telemetry transmit.  See http://cubesatsim.org/wiki for instructions on how to build the BPF.")
		while 1:
			time.sleep(5)
