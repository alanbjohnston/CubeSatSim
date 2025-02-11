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


def fstr(template):
    return eval(f"f'{template}'")
	
FC_EPS = 1
FC_BOB = 25
FC_SW = 50
FC_PAYLOAD = 55
extended = 1

sequence, image_id, image_count  = 0, 0, 0
Vx, Vy, Vz, Vb = 0, 0, 0, 0
Ix, Iy, Iz, Ic, Ib = 0, 0, 0, 0, 0
frame_count, frame_type = 0, " "
frequency_string, errors = " ", 0

# html_dir = "/home/pi/CubeSatSim/groundstation/public_html/"
html_dir = "/home/pi/fctelem/public_html/"
image_dir = "/home/pi/fctelem/"
image = "image_file"
ssdv = "/home/pi/ssdv/ssdv -d -J "

head_string = '<HEAD><meta http-equiv="refresh" content="5"></HEAD>\n<HTML>\n<H2>FunCube CubeSatSim Telemetry</H2>' + \
		'<p><pre>  <img height="256" width="320" src="' + image + '.jpeg"><br>'
foot_string = '<A HREF="images" target="_blank"> All images </a></HTML>'
telem_string_format = "           Image: {image_id:3d} count: {image_count:2d}<p>" + \
		" Vx(mV): {Vx:5d}   Vy(mV): {Vy:5d}   Vz(mV): {Vz:5d}<p>" + \
		" Ix(mA): {Ix:5d}   Iy(mA): {Iy:5d}   Iz(mA): {Iz:5d}<p>" + \
  		"     Vbat(mV): {Vb:5d}   Ibat(mA): {Ib:5d}<p></pre>" + \
    		" Freq: {frequency_string} errors: {errors} Seq: {sequence:d} {frame_type} frames: {frame_count:d}"

telem_string = fstr(telem_string_format)
with open(html_dir + "index.html", "w") as html_file:
	html_file.write(head_string)
	html_file.write(telem_string)
	html_file.write(foot_string)

image_id = 256 		# set illegal image ID for null # random.randint(0, 255)
image_count = 1;
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
			frame_count += 1
			data_block_string = line.split()
#			print(data_block_string)
			frequency_string = data_block_string[2]
			errors = int(data_block_string[5])
			data_block = [int(number_string,16) for number_string in data_block_string[7:]]
#			print("\n")
#			print(data_block)
#			print("\n")
			if (data_block[0] == 0xE0) or (data_block[0] == 0xE1):
				if (data_block[0] == 0xE0):
					frame_type = "RT1+IMG1"
				if (data_block[0] == 0xE1):
					frame_type = "RT2+IMG2"
				print(frame_type)	
				print(data_block[extended + 51], data_block[extended + 50], data_block[extended + 49])
				sequence = data_block[extended + 51] + data_block[extended + 50] * 2**8 + data_block[extended + 49] * 2**16	
				print("Sequence number: {:d}".format(sequence))
				Vx = (data_block[extended + FC_EPS + 0] * 2**6) + (data_block[extended + FC_EPS + 1] >> 2)
				Vy = (0x03 & data_block[extended + FC_EPS + 1]) * 2**12 + data_block[extended + FC_EPS + 2] * 2**4 + (data_block[extended + FC_EPS + 3] >> 4)
				Vz = (0x0f & data_block[extended + FC_EPS + 3]) * 2**10 + data_block[extended + FC_EPS + 4] * 2**2 + (data_block[extended + FC_EPS + 5] >> 6)
				Vb = (0x3f & data_block[extended + FC_EPS + 5]) * 2**8 + data_block[extended + FC_EPS + 6]
				Ix = data_block[extended + FC_EPS + 7] * 2**2 + (data_block[extended + FC_EPS + 8] >> 6)
				Iy = (0x3f & data_block[extended + FC_EPS + 8]) * 2**4 + (data_block[extended + FC_EPS + 9] >> 4)
				Iz = (0x0f & data_block[extended + FC_EPS + 9]) * 2**6 + (data_block[extended + FC_EPS + 10] >> 2)
				Ic = (0x03 & data_block[extended + FC_EPS + 10]) * 2**8 + data_block[extended + FC_EPS + 11] 
				Ib = data_block[extended + FC_EPS + 12] * 2**2 + ((0xc0 & data_block[extended + FC_EPS + 13]) >> 6)
				if (Ib == 0):
					Ib = 0 - Ic					
				print("Vx: {:d} mV Vy: {:d} mV Vz: {:d} mV".format(Vx, Vy, Vz))
				print('Payload {:x} {:x} \n'.format(data_block[FC_PAYLOAD + extended], data_block[FC_PAYLOAD + extended + 1]))
				if (data_block[FC_PAYLOAD + extended] == 0x55) and (data_block[FC_PAYLOAD + extended + 1] == 0x68):
					try:
						print("Writing payload to file")
						immutable_payload = bytes(bytearray(data_block[(FC_PAYLOAD + extended):]))   # payload)
#						print(immutable_payload)
						with open(image_dir + image + "_payload", "wb") as binary_file:
    							binary_file.write(immutable_payload)
					except:
						print("File error")
#					try:
					system(ssdv + image_dir + image + "_payload " + 
					       image_dir + image + "_payload.jpeg 2>&1 | sudo tee /home/pi/fctelem/ssdv_output")
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
										newfilename = image_dir + image + str(new_image_id) + ".jpeg"
	#									system(ssdv + image_dir + image + " " + filename)
										system("mv " + filename + " " + newfilename)
										system("mv " + image_dir + image + " " + image_dir + image + str(image_id))					
									else:
										system("sudo rm " + image_dir + image)
									print("New Image ID: ")
									print(new_image_id)
									image_id = new_image_id
	#								image_count = (image_count + 1) % 256
									image_count = 1								
								else:
									image_count += 1
									print("new image_count:")
									print(image_count)
								with open(image_dir + image, "ab") as binary_file:
    									binary_file.write(immutable_payload)
								filename = image_dir + image + str(image_id) + "." + str(image_count) + ".jpeg"	
								system(ssdv + image_dir + image + " " + filename)	
								system("cp " + filename + " " + html_dir + "image_file.jpeg")
								system("cp " + filename + " " + html_dir + "images/" + image + str(image_id) + ".jpeg")
								telem_string = fstr(telem_string_format)
								with open(html_dir + "index.html", "w") as html_file:
									html_file.write(head_string)
									html_file.write(telem_string)
									html_file.write(foot_string)
				else:
					print("Payload not an image!")
					image_id = 256 # set illegal image_id to force new image
			else:
				print("Unknown Sat Id or Frame")

						
