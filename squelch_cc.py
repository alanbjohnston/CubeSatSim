import RPi.GPIO as GPIO
from RPi.GPIO import output
from time import sleep
from os import system
import sys

def blink(times):
	blink_time = 0.1
	powerPin = 16
	for i in range(times):	# blink times
		GPIO.output(powerPin, 0) 
		sleep(blink_time)
		GPIO.output(powerPin, 1)
		sleep(blink_time)
	sleep(0.65)

def command_control_check():

#	global command_control
#	global no_command
	global debug_mode	
	command_count = 0
	global squelch
	global txLed
	
#	output(pd, 1)
#	output(ptt, 1)
#	sleep(1)
#	if (no_command == False and GPIO.input(squelch) == False):
	if GPIO.input(squelch) == False:
		print("carrier received!")
		# command_tx = not command_tx
#			print(command_tx)

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
##	else:
##		print("No carrier received!")
#	output(pd, 0)
#					sleep(1)
				
def increment_mode():
	global debug_mode	

	print("increment mode")
	powerPin = 16
	try:
		file = open("/home/pi/CubeSatSim/.mode")
		mode = file.read(1)
	except:
#		mode = "f"
#		if (debug_mode == 1):
		print("Can't open .mode file") # , defaulting to FSK")
	file.close()
	print("Mode is: ")
	print(mode)
	if (mode == 'a'):
		mode = 'f'
		blink(2)

	elif (mode == 'f'):
		mode = 'b'
		blink(3)
	
	elif (mode == 'b'):
		mode = 's'
		blink(4)
	elif (mode == 's'):
		mode = 'm'
		blink(5)
	elif (mode == 'm'):
		mode = 'e'
		blink(6)
	elif (mode == 'e'):
		mode = 'j'
		blink(7)
	else:
		mode = 'a'
		blink(1)

	try:	
		print("/home/pi/CubeSatSim/config -" + mode)
		if (debug_mode == False):
			print("Changing mode now")
			os.system("echo 'changing mode due to C2C!' | wall")
			GPIO.setwarnings(False)
			GPIO.output(txLed, 0)
			GPIO.output(powerPin, 0)
			system("sudo systemctl stop rpitx")	
			system("sudo systemctl stop cubesatsim")
			
			system("/home/pi/CubeSatSim/config -" + mode)


#		file = open("/home/pi/CubeSatSim/.mode", "w")
#		count_string = str(command_count)
#		file.write(mode)
#		file.close()
#		print(".mode file written")
#		print("sudo reboot -h now")
#		GPIO.setwarnings(False)
#		GPIO.setup(powerPin, GPIO.OUT)
#		GPIO.output(powerPin, 0);
#		system("reboot -h now")
#		release = True;
#		system("/home/pi/CubeSatSim/config -" + mode)
#		system("reboot -h now")
		print(" ")
#		sleep(10);
	except:
		print("can't change mode")

print("Squelch Command and Control active")
debug_mode = False
if (len(sys.argv)) > 1:
#        print("There are arguments!")
	if ('d' == sys.argv[1]):
		debug_mode = True
		print("Debug mode - mode changes not made")

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
squelch = 6
powerPin = 16
txLed = 27
GPIO.setup(squelch, GPIO.IN, pull_up_down=GPIO.PUD_UP)  ## pull up in case pin is not connected
GPIO.setup(powerPin, GPIO.OUT)
GPIO.setup(txLed, GPIO.OUT)

while True:
	command_control_check()
	sleep(0.5)
