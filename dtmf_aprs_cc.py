import sys
from os import system
import RPi.GPIO as GPIO
from RPi.GPIO import output
from time import sleep
import logging
logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')

if __name__ == "__main__":
	powerPin = 16
	txLed = 27
	change_mode = False
	debug_mode = False
	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if ('d' == sys.argv[1]):
			debug_mode = True
			
	for line in sys.stdin:
#		if (debug_mode):
		print(line, end =" ")
		logging.warning(line)
			
#			if '^c' == line.rstrip():
#				break

		if ((line.find("MODE=a")) > 0) or ((line.find("DTMF>APDW15:t1#")) > 0):
			system("echo '\nAPRS Mode!!\n'")
			mode = 'a'
			change_mode = True
		if ((line.find("MODE=f")) > 0) or ((line.find("DTMF>APDW15:t2#")) > 0):
			system("echo '\nFSK Mode!!\n'")
			mode = 'f'
			change_mode = True
		if ((line.find("MODE=b")) > 0) or ((line.find("DTMF>APDW15:t3#")) > 0):
			system("echo '\nBPSK Mode!!\n'")
			mode = 'b'
			change_mode = True
		if ((line.find("MODE=s")) > 0) or ((line.find("DTMF>APDW15:t4#")) > 0):
			system("echo '\nSSTV Mode!!\n'")
			mode = 's'
			change_mode = True
		if ((line.find("MODE=m")) > 0) or ((line.find("DTMF>APDW15:t5#")) > 0):
			system("echo '\nCW Mode!!\n'")
			mode = 'm'
			change_mode = True
		if ((line.find("MODE=o")) > 0) or ((line.find("DTMF>APDW15:t10#")) > 0):
			system("echo '\nBeacon Mode toggle!!\n'")
			mode = 'o'
			change_mode = True
#			try:
#				fm = open("/home/pi/CubeSatSim/beacon_off")
#				fm.close()
#				system("echo 'Beacon is off, turning it ON'")
#				system("sudo rm /home/pi/CubeSatSim/beacon_off > /dev/null 2>&1")
#			except:
#				system("echo 'Beacon is on, turning it OFF'")
#				system("touch /home/pi/CubeSatSim/beacon_off")
#
#			system("echo 'Restarting rpitx'")	
#			system("sudo systemctl restart rpitx")
##			system("echo 'Resuming rpitx'")
##			system("sudo python3 -u /home/pi/CubeSatSim/rpitx.py s & > /dev/null 2>&1")
#			change_mode = False
		if (debug_mode == False)  and (change_mode == True):
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
			elif:
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
			system("sudo systemctl stop rpitx")
			system("sudo systemctl stop cubesatsim")
			
			print("\n/home/pi/CubeSatSim/config -" + mode)
			system("/home/pi/CubeSatSim/config -" + mode)

			
			change_mode = False
	
	print("Waiting 5 seconds to allow unplug and plug of soundcard")
	sleep(5)
	print("Done")
	
