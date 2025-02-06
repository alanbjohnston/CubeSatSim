import sys
from os import system
from time import sleep
import logging
logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')

FC_EPS = 1
FC_BOB = 25
FC_SW = 50
FC_PAYLOAD = 55
extended = 1

if __name__ == "__main__":
	debug_mode = False
	counter = 1
	if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if ('d' == sys.argv[1]):
			debug_mode = True
	print(debug_mode)
	for line in sys.stdin:
		if (debug_mode):
			print(line, end =" ")
			logging.warning(line)
			
		if ((line.find("data: ")) > 0):
			print("\ndata block found!\n")
			data_block_string = line.split()
			print(data_block_string)
			data_block = [int(number_string,16) for number_string in data_block_string[7:]]
			print("\n")
			print(data_block)
			print("\n")
			if (data_block[0] == 0xE0) or (data_block[0] == 0xE1):
				if (data_block[0] == 0xE0):
					print("CubeSatSim Frametype RT1+IMG1")
				if (data_block[0] == 0xE1):
					print("CubeSatSim Frametype RT2+IMG2")	
				print("Sequence number: ")
				print(data_block[extended + 51] + data_block[extended + 50] * 2^16 + data_block[extended + 49] * 2^32)
				print("Vx (mV): ")
				print((data_block[extended + FC_EPS + 0] << 2) + (0xfc & data_block[extended + FC_EPS + 1]))
				print(" ")
				
			else:
				print("Unknown Sat Id or Frame")
