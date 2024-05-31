import sys
from os import system

if __name__ == "__main__":

	change_mode = False
	debug_mode = False
	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if ('d' == sys.argv[1]):
			debug_mode = True
			
	for line in sys.stdin:
		if (debug_mode):
			print(line)
			
#			if '^c' == line.rstrip():
#				break

		if (line.find("MODE=a")) > 0:
			system("echo 'APRS Mode!!'")
			mode = 'a'
			change_mode = True
		if (line.find("MODE=f")) > 0:
			system("echo 'FSK Mode!!'")
			mode = 'f'
			change_mode = True
		if (line.find("MODE=b")) > 0:
			system("echo 'BPSK Mode!!'")
			mode = 'b'
			change_mode = True
		if (line.find("MODE=s")) > 0:
			system("echo 'SSTV Mode!!'")
			mode = 's'
			change_mode = True
		if (line.find("MODE=m")) > 0:
			system("echo 'CW Mode!!'")
			mode = 'm'
			change_mode = True
		if (debug_mode == True)  and (change_mode == True):
			print("/home/pi/CubeSatSim/config -" + mode)
			system("/home/pi/CubeSatSim/config -" + mode)
			change_mode = False
	print("Done")
