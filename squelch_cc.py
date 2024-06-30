import RPi.GPIO as GPIO
from RPi.GPIO import output

def command_control_check():

#	global command_control
#	global no_command
#	global debug_mode	
	command_count = 0
	
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
	else:
		print("No carrier received!")
#	output(pd, 0)
#					sleep(1)
				
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

while True:
	command_control_check()
