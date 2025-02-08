import sys
from os import system
from time import sleep
import logging
import random
from PIL import Image, ImageDraw, ImageFont, ImageColor
import subprocess
import io

logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')

FC_EPS = 1
FC_BOB = 25
FC_SW = 50
FC_PAYLOAD = 55
extended = 1

sequence = 0
image_id = 0
image_count = 0
Vx = 0
Vy = 0
Vz = 0
Ix = 0
Iy = 0
Iz = 0

head_string = '<HEAD><meta http-equiv="refresh" content="5"></HEAD>\n<HTML>\nFunCube CubeSatSim Telemetry\n<p>\n<pre>  <img height="256" width="320" src="image_file.jpeg"><br>'
foot_string = "</pre></p>\n</HTML>"
telem_string = f"\nSequence number: {sequence:5d} Image ID: {image_id:3d} count: {image_count:2d}<p>Vx: {Vx:5d} mV  Vy: {Vy:5d} mV  Vz: {Vz:5d} mV<p>Ix: {Ix:5d} mA  Iy: {Iy:5d} mA  Iz: {Iz:5d} mA"
with open("/home/pi/CubeSatSim/groundstation/public_html/index.html", "w") as html_file:
	html_file.write(head_string)
	html_file.write(telem_string)
	html_file.write(foot_string)

image_id = 256 		# set illegal image ID for null # random.randint(0, 255)
image_count = 0;
system("sudo rm image_file")

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
#			print(data_block_string)
			data_block = [int(number_string,16) for number_string in data_block_string[7:]]
#			print("\n")
#			print(data_block)
#			print("\n")
			if (data_block[0] == 0xE0) or (data_block[0] == 0xE1):
				if (data_block[0] == 0xE0):
					print("CubeSatSim Frametype RT1+IMG1")
				if (data_block[0] == 0xE1):
					print("CubeSatSim Frametype RT2+IMG2")	
				print(data_block[extended + 51], data_block[extended + 50], data_block[extended + 49])
				sequence = data_block[extended + 51] + data_block[extended + 50] * 2**8 + data_block[extended + 49] * 2**16	
				print("Sequence number: {:d}".format(sequence))
				Vx = (data_block[extended + FC_EPS + 0] * 2**6) + (data_block[extended + FC_EPS + 1] >> 2)
				Vy = (0x03 & data_block[extended + FC_EPS + 1]) * 2**12 + data_block[extended + FC_EPS + 2] * 2**4 + (data_block[extended + FC_EPS + 3] >> 4)
				Vz = (0x0f & data_block[extended + FC_EPS + 3]) * 2**10 + data_block[extended + FC_EPS + 4] * 2**2 + (data_block[extended + FC_EPS + 5] >> 6)
				Vb = (0x3f & data_block[extended + FC_EPS + 5]) * 2**8 + data_block[extended + FC_EPS + 6]
				Ix = data_block[extended + FC_EPS + 7] * 2**2 + data_block[extended + FC_EPS + 8] >> 6
				Iy = (0x3f & data_block[extended + FC_EPS + 8]) * 2**4 + data_block[extended + FC_EPS + 9] >> 4
				Iz = (0x0f & data_block[extended + FC_EPS + 9]) * 2**6 + data_block[extended + FC_EPS + 10] >> 2
				
				print("Vx: {:d} mV Vy: {:d} mV Vz: {:d} mV".format(Vx, Vy, Vz))
				print('Payload {:x} {:x} \n'.format(data_block[FC_PAYLOAD + extended], data_block[FC_PAYLOAD + extended + 1]))
				if (data_block[FC_PAYLOAD + extended] == 0x55) and (data_block[FC_PAYLOAD + extended + 1] == 0x68):
					try:
						print("Writing payload to file")
						immutable_payload = bytes(bytearray(data_block[(FC_PAYLOAD + extended):]))   # payload)
#						print(immutable_payload)
						with open("image_file_payload", "wb") as binary_file:
    							binary_file.write(immutable_payload)

					except:
						print("File error")
#					try:
					system("/home/pi/ssdv/ssdv -d -J /home/pi/fctelem/image_file_payload /home/pi/fctelem/image_file_payload.jpeg 2>&1 | sudo tee /home/pi/fctelem/ssdv_output")
					with open("/home/pi/fctelem/ssdv_output", "r") as file:
						for line in file:
#							print("line:")
#							print(line)
							if ((line.find("mage ID:")) > 0):
#								print("\nImage ID found!\n")
								image_id_string = line.split()
#								print(image_id_string)
								new_image_id = int(image_id_string[2], 16)
#								print(new_image_id)
								if (new_image_id != image_id):
									print("End of image")
									if (image_id != 256):
										newfilename = "/home/pi/fctelem/image_file" + str(new_image_id) + ".jpeg"
	#									system("/home/pi/ssdv/ssdv -d -J /home/pi/fctelem/image_file " + filename)
										system("mv " + filename + " " + newfilename)
										system("mv /home/pi/fctelem/image_file /home/pi/fctelem/image_file" + str(image_id))					
									else:
										system("sudo rm /home/pi/fctelem/image_file")
									print("New Image ID: ")
									print(new_image_id)
									image_id = new_image_id
	#								image_count = (image_count + 1) % 256
									image_count = 0								
								else:
									image_count += 1
									print("new image_count:")
									print(image_count)
								with open("image_file", "ab") as binary_file:
    									binary_file.write(immutable_payload)
								filename = "/home/pi/fctelem/image_file" + str(image_id) + "." + str(image_count) + ".jpeg"	
								system("/home/pi/ssdv/ssdv -d -J /home/pi/fctelem/image_file " + filename)	
								system("cp " + filename + " /home/pi/CubeSatSim/groundstation/public_html/image_file.jpeg")
								telem_string = f"\nSequence number: {sequence:5d} Image ID: {image_id:3d} count: {image_count:2d}<p>Vx: {Vx:5d} mV  Vy: {Vy:5d} mV  Vz: {Vz:5d} mV<p>Ix: {Ix:5d} mA  Iy: {Iy:5d} mA  Iz: {Iz:5d} mA"
								with open("/home/pi/CubeSatSim/groundstation/public_html/index.html", "w") as html_file:
									html_file.write(head_string)
									html_file.write(telem_string)
									html_file.write(foot_string)
				else:
					print("Payload not an image!")
					image_id = 256 # set illegal image_id to force new image
			else:
				print("Unknown Sat Id or Frame")
