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
			print(line, end =" ")
			
#			if '^c' == line.rstrip():
#				break

		if ((line.find("MODE=a")) > 0) or ((line.find("DTMF>APDW15:t1#")) > 0):
			system("echo '\nAPRS Mode!!\n'")
			mode = 'a'
			change_mode = True
		if (line.find("MODE=f")) > 0:
			system("echo '\nFSK Mode!!\n'")
			mode = 'f'
			change_mode = True
		if (line.find("MODE=b")) > 0:
			system("echo '\nBPSK Mode!!\n'")
			mode = 'b'
			change_mode = True
		if (line.find("MODE=s")) > 0:
			system("echo '\nSSTV Mode!!\n'")
			mode = 's'
			change_mode = True
		if (line.find("MODE=m")) > 0:
			system("echo '\nCW Mode!!\n'")
			mode = 'm'
			change_mode = True
		if (debug_mode == False)  and (change_mode == True):
			print("\n/home/pi/CubeSatSim/config -" + mode)
			system("/home/pi/CubeSatSim/config -" + mode)
			change_mode = False
	print("Done")
