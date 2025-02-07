import sys
from os import system
from time import sleep
import logging
import random
from PIL import Image, ImageDraw, ImageFont, ImageColor

logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')

FC_EPS = 1
FC_BOB = 25
FC_SW = 50
FC_PAYLOAD = 55
extended = 1

image_count = random.randint(0, 255)
image_index = 0;

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
				print('Payload 0:{:x}, Payload 1:{:x}'.format(data_block[FC_PAYLOAD + extended], data_block[FC_PAYLOAD + extended + 1]))
				print(" ")
				if (data_block[FC_PAYLOAD + extended] == 0x55) and (data_block[FC_PAYLOAD + extended + 1] == 0x68):
					try:
						print("Writing payload to file")
						immutable_payload = bytes(bytearray(data_block[(FC_PAYLOAD + extended):]))   # payload)
						print(immutable_payload)
						with open("image_file", "ab") as binary_file:
    							binary_file.write(immutable_payload)

					except:
						print("File error")
					try:
						system("/home/pi/ssdv/ssdv -d -J image_file image_file" + str(image_count) + "." + str(image_index) + ".jpeg")
						image = Image.open("image_file" + str(image_count) + "." + str(image_index) + ".jpeg")
						image.show()
					except:
						print("Image display error")
					image_index += 1	
				else:
					print("End of image")

					system("/home/pi/ssdv/ssdv -d -J image_file image_file" + str(image_count) + ".jpeg")
					image = Image.open("image_file" + str(image_count) + ".jpeg")
					image.show()
#					system("sudo rm image_file")
					system("sudo mv image_file image_file" + str(image_count))					
					print("Image count: ")
					print(image_count)
					image_count = (image_count + 1) % 256
			else:
				print("Unknown Sat Id or Frame")
