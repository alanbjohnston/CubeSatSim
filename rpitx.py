#!/usr/bin/env python

import RPi.GPIO as GPIO
from RPi.GPIO import output
#import subprocess
#import time
from time import sleep
#import os
import sys
from os import system
from PIL import Image, ImageDraw, ImageFont, ImageColor
import serial

def command_control_check():

	global command_control
	global no_command
	global debug_mode
	global command_count
	
	output(pd, 1)
	output(ptt, 1)
	sleep(1)
	if (no_command == False and GPIO.input(squelch) == False):
		print("carrier received!")
		# command_tx = not command_tx
#			print(command_tx)

		try:
			command_count += 1
			filec = open("/home/pi/CubeSatSim/command_count.txt", "w")
			command_count_string = str(command_count)
			print(command_count_string)
			string = filec.write(command_count_string)
			filec.close()
		except:
			if (debug_mode == 1):
				print("Can't write command_count file!")
		print("Command_count: ")
		print(command_count)							
		
		increment_mode()
		
#			if (command_tx == True):
#				print("Turning on transmit")
#				system("echo > command_tx True")
#							output(txLed, txLedOn)
#							sleep(0.5)
#							output(txLed, txLedff)
#			else:
#				print("Turning off transmit")
#				system("echo > command_tx False")
	else:
		print("No carrier received!")
#	output(pd, 0)
#					sleep(1)
		

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
		GPIO.output(powerPin, 0) # blink two times
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(0.1)
		GPIO.output(powerPin, 0)
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(2.5)

	elif (mode == 'f'):
		mode = 'b'
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
		sleep(2.5)
	
	elif (mode == 'b'):
		mode = 's'
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
		sleep(2.5)

	elif (mode == 's'):
		mode = 'm'
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
		sleep(2.5)
	else:
		mode = 'a'
		GPIO.output(powerPin, 0) # blink one time
		sleep(0.1)
		GPIO.output(powerPin, 1)
		sleep(2.5)

	try:	
		file = open("/home/pi/CubeSatSim/.mode", "w")
		count_string = str(command_count)
		file.write(mode)
		file.close()
		print(".mode file written")
		
		GPIO.setwarnings(False)
		GPIO.output(txLed, 0)
		GPIO.output(powerPin, 0)
		print("sudo reboot -h now")
		GPIO.setwarnings(False)
		GPIO.setup(powerPin, GPIO.OUT)
		GPIO.output(powerPin, 0);
#		system("reboot -h now")
#		release = True;

		print("Changing mode now")
#		system("/home/pi/CubeSatSim/config -" + mode)
		system("reboot -h now")

		sleep(10);
	except:
		print("can't write to .mode file")
		

print("CubeSatSim v1.3.2 rpitx.py starting...")

pd = 21
ptt = 20
txc_pin = 7
squelch = 6
green = 16

command_tx = True

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(txc_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(green, GPIO.OUT)

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

# GPIO.setup(txLed, GPIO.OUT)
# output(txLed, txLedOff)

GPIO.setmode(GPIO.BCM) # Repeat to make LED work on Pi 4
GPIO.setwarnings(False)
GPIO.setup(txLed, GPIO.OUT)

GPIO.setup(pd, GPIO.OUT)
#output(pd, 1)
output(pd, 0)
GPIO.setup(ptt, GPIO.OUT)
output (ptt, 1)

txc = False
if GPIO.input(txc_pin) == False:
	print("TXC is present")
	txc = True;
else:
	print("TXC not present")

# txc = False  # forcing it off
output(txLed, txLedOn)
sleep(1)
output(txLed, txLedOff)

battery_saver_check()	

# print(txLedOn)
print(txLed)
# GPIO.setup(27, GPIO.OUT)
# GPIO.output(27, 0)

debug_mode = 0  #no debugging rpitx

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
		file = open("/home/pi/CubeSatSim/command_tx")
		char = file.read(1)
		if (char == 'T'):
			command_tx = True
		else:
			command_tx = False
	except:
		command_tx = True
		if (debug_mode == 1):
			print("Can't open command_tx file, defaulting to True")
	print("Command_tx: ")
	print(command_tx)
	
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
	
	try:
		file = open("/home/pi/CubeSatSim/sim.cfg")
#		callsign = file.readline().split(" ")[0]
		config = file.readline().split()		
		callsign = config[0]
		if len(config) > 5:
			sq = config[5]
		else:	
			sq = '0'
		if len(config) > 6:
#                       tx = config[6]
                        txf = float(config[6])
                        print(txf)
#                        print( "{:.4f}".format(txf))
                        tx = "{:.4f}".format(txf)
                        print(tx)
		else:	
			tx = '434.9000'			
		if len(config) > 7:
#                       rx = config[7]
                        rxf = float(config[7])
                        print(rxf)
#                        print( "{:.4f}".format(rxf))
                        rx = "{:.4f}".format(rxf)
                        print(rx)
		else:	
			rx = '435.0000'	
		print(config)
		print
#		print(callsign)
		print(sq)
		if sq == '8':
			print("squelch set to 8, no command input!")
			no_command = True
		else:
			no_command = False		
		print(no_command)
	except:
		callsign = "AMSAT"
		if (debug_mode == 1):
			print("Can't read callsign from sim.cfg file, defaulting to AMSAT")	
	file.close()

	try:
		f = open("/home/pi/CubeSatSim/command_control", "r")
		f.close()
		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(squelch, GPIO.IN, pull_up_down=GPIO.PUD_UP)  ## pull up in case pin is not connected
		if GPIO.input(squelch) == False:
			print("squelch not set correctly, no command input!")
			no_command = True
		else:
			print("command and control is activated")
			no_command = False
	except:
		print("command and control not activated")
		no_command = True
	
	print(callsign)
	GPIO.setmode(GPIO.BCM)  # added to make Tx LED work on Pi 4
	GPIO.setup(txLed, GPIO.OUT)

	print("Programming FM module!\n");	
	output(pd, 1)
	output (ptt, 1)
	try:
		ser = serial.Serial("/dev/ttyAMA0", 9600)
		print(ser.portstr)
		uhf_string = "AT+DMOSETGROUP=0," + rx +"," + tx + ",0,3,0,0\r\n"
		for i in range(6):
#			ser.write(b"AT+DMOSETGROUP=0,435.0000,434.9000,0,3,0,0\r\n")
			ser.write(uhf_string.encode())
			sleep(0.1)
	except:
		print("Error in serial write")
	ser.close()
	output(pd, 0)
	
	sleep(10)  # delay so cubesatsim code catches up
	
	system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1")
	
	command_control_check()
	
#	if (mode != 'a') and (command_tx == True):	
#	if (command_tx == True):	
	if (mode != 'm'):	
#		battery_saver_mode
		output(txLed, txLedOn)			
#		if (txc):
#			output(pd, 1)
#			output (ptt, 0)
#			sleep(0.1)
#			system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/morse.wav")
#			sleep(0.1)
#			output (ptt, 1)
#			output(pd, 0)
#		else:	
		if (True):
			if (debug_mode == 1):
#				system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3")
				system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
			else:
#				system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1")
				system("echo 'hi hi de " + callsign + "' > id.txt && gen_packets -M 20 /home/pi/CubeSatSim/id.txt -o /home/pi/CubeSatSim/morse.wav -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")
		
		output(txLed, txLedOff)

		sleep(1)
	else:
		print("Don't transmit CW ID since command_tx is False or APRS mode")

	if (transmit):
	
#		print 'Length: ', len(sys.argv)
    
#		if (len(sys.argv)) > 1:
#        		print("There are arguments!")
		if (mode == 'a'):
			command_control_check()	
			output(pd, 1)
			output(ptt, 1)			
			print("AFSK")
			while True:
				sleep(0.1)
			while True:
				try:
					f = open("/home/pi/CubeSatSim/ready")
					f.close()
					if (debug_mode == 1):
						print("Packet ready!")
					system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1")
					system("cat /home/pi/CubeSatSim/t.txt")
					if (command_tx == True):
						output(txLed, txLedOn)
#						output(pd, 1)
#						output (ptt, 0)
#						sleep(.1)
#						
#						battery_saver_check()
						if (txc):
#							output(pd, 1)
							sleep(0.1) # add delay before transmit
							output (ptt, 0)
							sleep(0.3)   # add even more time at start
							system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/telem.wav")							
							sleep(0.2)  # add more time at end
							output (ptt, 1)
#							output(pd, 0)							
						else:			
							if (debug_mode == 1):
								system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("gen_packets -o /home/pi/CubeSatSim/telem.wav /home/pi/CubeSatSim/t.txt -r 48000 > /dev/null 2>&1 && cat /home/pi/CubeSatSim/telem.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")
					
						sleep(0.1)  
#						output (ptt, 1)
#						output(pd, 0)
						output(txLed, txLedOff)
						system("sudo rm /home/pi/CubeSatSim/ready")
					f.close()
				
					if (debug_mode == 1):
						print("Ready for next packet!")
						
					sleep(0.5)
				except:
					command_control_check()
					sleep(1)
		elif (mode == 'm'):
			print("CW")
			while True:
				command_control_check()
				
				try:
					f = open("/home/pi/CubeSatSim/cwready")
					f.close()
					system("sudo rm /home/pi/CubeSatSim/cwready")
##					ch = 1
					for chan in range(7):
						command = "gen_packets -M 20 -o /home/pi/CubeSatSim/morse.wav /home/pi/CubeSatSim/cw" + str(chan) + ".txt -r 48000 > /dev/null 2>&1"
						print(command)
						system(command)
##						chan = chan + 1						
						if (command_tx == True):
							output(txLed, txLedOn)					
	
							if (txc):
								output (pd, 1)
								sleep(0.3)
								output (ptt, 0)	
								system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/morse.wav")
								sleep(0.1)
								output (ptt, 1)
#								output (pd, 0)
							else:
								if (debug_mode == 1):
									system("cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3")
								else:
									system("cat /home/pi/CubeSatSim/morse.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")					
							output(txLed, txLedOff)
							
						command_control_check()	
					f.close()
					sleep(5)
				except:	
					command_control_check()
					sleep(1)
		elif (mode == 's'):
			print("SSTV")
			command_control_check()	
			try: 
#				from picamera import PiCamera
#					from pysstv.sstv import SSTV
#				camera = PiCamera()
				print("Testing for camera")
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
			output(txLed, txLedOff)
#			output (ptt, 1)
#			output(pd, 0)
			if (camera_present == 1):
				try:
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg") 
					command_control_check()	
					
					if (command_tx == True):
						print ("Sending SSTV image")
						output(txLed, txLedOn)
#						battery_saver_check()

						if (txc):
							output(pd, 1)
							output (ptt, 0)
							system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav")
							output (ptt, 1)
							output(pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, txLedOff)

	#					sleep(1)
				except:
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
				while 1:
					command_control_check()			
					
					system("raspistill -o /home/pi/CubeSatSim/camera_out.jpg -w 320 -h 256") #  > /dev/null 2>&1")
					print("Photo taken")

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
					draw.text((122, 12), telem_string, font=font2, fill='black')
					draw.text((120, 10), telem_string, font=font2, fill='white')
					img.save(file)
					
					command_control_check()			
					
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/camera_out.jpg") 
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg > /dev/null 2>&1") 

					command_control_check()			

					if (command_tx == True):
						print ("Sending SSTV image")
						output(txLed, txLedOn)
#						battery_saver_check()

						if (txc):
							output(pd, 1)
							output (ptt, 0)
							system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/camera_out.jpg.wav")	
							output(ptt, 1)
							output (pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/camera_out.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, txLedOff)
#						output (ptt, 1)
					
#					output(pd, 0)
					system("sudo rm /home/pi/CubeSatSim/camera_out.jpg.wav > /dev/null 2>&1") 
					sleep(1)
			else:
				try:
					command_control_check()			
					
					file = open("/home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg")
					print("First SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg") 

					command_control_check()			
					
					if (command_tx == True):

						print ("Sending SSTV image")
						output(txLed, txLedOn)

#						battery_saver_check()

						if (txc):
							output(pd, 1)
							output (ptt, 0)
							system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav")
							output(ptt, 1)
							output (pd, 0)
						else:	
							if (debug_mode == 1):
								system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3")
							else:
								system("cat /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

						output(txLed, txLedOff)
#						output (ptt, 1)
#						output(pd, 0)
					sleep(1)
				except:
					print("image 1 did not load - copy from CubeSatSim/sstv directory")
				try:
					command_control_check()			
					
					file = open("/home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")
					print("Second SSTV stored image detected")
					system("/home/pi/PiSSTVpp/pisstvpp -r 48000 -p s2 /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg")

					while 1:

						command_control_check()		
	
						if (command_tx == True):
							print ("Sending SSTV image")
							output(txLed, txLedOn)
#							battery_saver_check()

							if (txc):
								output(pd, 1)
								output (ptt, 0)
								system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav")
								output(ptt, 1)
								output (pd, 0)
							else:
								if (debug_mode == 1):
									system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3") 
								else:
									system("cat /home/pi/CubeSatSim/sstv_image_2_320_x_256.jpg.wav | csdr convert_i16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 > /dev/null 2>&1")

							output(txLed, txLedOff)
#							output (ptt, 1)
#							output(pd, 0)
						sleep(5)
				except:	
					print("image 2 did not load - copy from CubeSatSim/sstv directory")
					if (txc == False):
						if (command_tx == True):
							system("(while true; do (sleep 5 && cat /home/pi/CubeSatSim/wav/sstv.wav); done) | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f " + tx + "e3 &")
					while 1:
						if (command_tx == True):
							command_control_check()	
							output(txLed, txLedOn)
#							battery_saver_check()
						
							if (txc):
								output(pd, 1)
								output (ptt, 0)		
								system("aplay -D hw:CARD=Headphones,DEV=0 /home/pi/CubeSatSim/sstv.wav")
								output(ptt, 1)
								output (pd, 0)		
							else:
								sleep(60)
							
							output(txLed, txLedOff)
#							output (ptt, 1)
#							output(pd, 0)
						sleep(1)
						
		elif (mode == 'b'):
			command_control_check()	
			print("BPSK")
			print("turn on FM rx")
			output(pd, 1)
			output(ptt, 1)
			if (command_tx == True):
#				system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.9e6 -t float &")
				system("sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f " + tx + "e6 -t float &")
			print("Turning LED on/off and listening for carrier")
			while 1:
				output(txLed, txLedOff)
				sleep(0.4)
#				if (command_tx == False):
#					output(txLed, txLedOn)
#					sleep(0.03)
#					output(txLed, txLedOff)
				command_control_check()
				
				if (command_tx == True):		
					output(txLed, txLedOn)
				sleep(4.2)
		else:
			print("FSK") 
			print("turn on FM rx")
			output(pd, 1)
			output(ptt, 1)
			if (command_tx == True):
				system("sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f " + tx + "e3 &")
			print("Turning LED on/off and listening for carrier")
			while 1:
				output(txLed, txLedOff)
				sleep(0.4)
#				if (command_tx == False):
#					output(txLed, txLedOn)
#					sleep(0.03)
#					output(txLed, txLedOff)
				command_control_check()
				if (command_tx == True):		
					output(txLed, txLedOn)
				sleep(4.2)
	else:
		print("No Low Pass Filter so no telemetry transmit.  See http://cubesatsim.org/wiki for instructions on how to build the BPF.")
		while 1:
			sleep(5)
