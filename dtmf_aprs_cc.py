import sys
from os import system
# import RPi.GPIO as GPIO
# from RPi.GPIO import output
from time import sleep
import logging
logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')

def blink(times):
	for i in range(times):
		system("gpio write 27 0")
#		GPIO.output(27, 0) 
		sleep(0.1)
		system("gpio write 27 1")
#		GPIO.output(27, 1)
		sleep(0.1)

if __name__ == "__main__":
	powerPin = 16
	txLed = 17
	change_mode = False
	debug_mode = False
	counter = 1
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

		if ((line.find("MODE=a")) > 0):
			system("echo '\nAPRS Mode!!\n'")
			mode = 'a'
			change_mode = True
		if ((line.find("DTMF>APDW15:t1#")) > 0):
			system("echo '\nAPRS Mode!!\n'")
			mode = 'a'
			change_mode = True
		if ((line.find("MODE=f")) > 0):
			system("echo '\nFSK Mode!!\n'")
			mode = 'f'
			change_mode = True
		if ((line.find("DTMF>APDW15:t2#")) > 0):
			system("echo '\nFSK Mode!!\n'")
			mode = 'f'
			change_mode = True
		if ((line.find("MODE=b")) > 0):
			system("echo '\nBPSK Mode!!\n'")
			mode = 'b'
			change_mode = True
		if ((line.find("DTMF>APDW15:t3#")) > 0):
			system("echo '\nBPSK Mode!!\n'")
			mode = 'b'
			change_mode = True
		if ((line.find("MODE=s")) > 0):
			system("echo '\nSSTV Mode!!\n'")
			mode = 's'
			change_mode = True
		if ((line.find("DTMF>APDW15:t4#")) > 0):
			system("echo '\nSSTV Mode!!\n'")
			mode = 's'
			change_mode = True
		if ((line.find("MODE=m")) > 0):
			system("echo '\nCW Mode!!\n'")
			mode = 'm'
			change_mode = True
		if ((line.find("DTMF>APDW15:t5#")) > 0):
			system("echo '\nCW Mode!!\n'")
			mode = 'm'
			change_mode = True
		if ((line.find("MODE=e")) > 0):
			system("echo '\nRepeater Mode!!\n'")
			mode = 'e'
			change_mode = True
		if ((line.find("DTMF>APDW15:t6#")) > 0):
			system("echo '\nRepeater Mode!!\n'")
			mode = 'e'
			change_mode = True
		if ((line.find("MODE=j")) > 0):
			system("echo '\nFUNcube Mode!!\n'")
			mode = 'j'
			change_mode = True
		if ((line.find("DTMF>APDW15:t7#")) > 0):
			system("echo '\nFUNcube Mode!!\n'")
			mode = 'j'
			change_mode = True				
		if ((line.find("MODE=n")) > 0):
			system("echo '\nTransmit Commands Mode!!\n'")
			mode = 'n'
			change_mode = True
		if ((line.find("DTMF>APDW15:t11#")) > 0):
			system("echo '\nTransmit Commands Mode!!\n'")
			mode = 'n'
			change_mode = True
		if ((line.find("MODE=o")) > 0):
			counter = (counter + 1) % 2 # Direwolf prints it twice, only do once
			if (counter == 1):
				system("echo '\nBeacon Mode toggle!!\n'")
				mode = 'o'
				change_mode = True
				counter = 1		
		if ((line.find("DTMF>APDW15:t10#")) > 0):
			system("echo '\nBeacon Mode toggle!!\n'")
			mode = 'o'
			change_mode = True

#		if ((debug_mode == False) and (change_mode == True) and (counter == 1): # skip every other APRS command since Direwolf prints them twice
		if (debug_mode == False)  and (change_mode == True): # skip every other APRS command since Direwolf prints them twice
			if (mode == 'f'):
				blink(2)
				sleep(1)
		
			elif (mode == 'b'):
				blink(3)
				sleep(1)
			
			elif (mode == 's'):
				blink(4)
				sleep(1)
		
			elif (mode == 'm'):
				blink(5)
				sleep(1)

			elif (mode == 'e'):
				blink(6)
				sleep(1)

			elif (mode == 'j'):
				blink(7)
				sleep(1)			
			
			elif (mode == 'a'):
				mode = 'a'
				blink(1)
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
			
#			GPIO.output(txLed, 0)
#			GPIO.output(powerPin, 0)
			system("gpio write 27 1")
			system("gpio write 0 0")
			system("gpio write 2 0")
		
			system("sudo systemctl stop transmit")
#			system("sudo systemctl stop cubesatsim")
			
			print("\n/home/pi/CubeSatSim/config -" + mode)
			system("/home/pi/CubeSatSim/config -" + mode)

			
			change_mode = False
	
	print("Waiting 5 seconds to allow unplug and plug of soundcard")
	sleep(5)
	print("Done")
	
