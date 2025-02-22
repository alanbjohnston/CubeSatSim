import sys
from os import system
from time import sleep
import logging
import random
from PIL import Image, ImageDraw, ImageFont, ImageColor
import subprocess
import io
import datetime

logging.basicConfig(format='%(message)s')
# logging.warning('CC-Warning!')


def fstr(template):
	return eval(f"f'{template}'")	

def system_and_print(string):
	print(string)
	system(string)
	
FC_EPS = 1
FC_BOB = 25
FC_SW = 50
FC_PAYLOAD = 55
extended = 1

sequence, image_id, image_count  = 0, 0, 0
Vx, Vy, Vz, Vb = 0, 0, 0, 0
Ix, Iy, Iz, Ic, Ib = 0, 0, 0, 0, 0
frame_count, frame_type = 0, " "
frequency_string, frequency, errors, first_byte = " ", 0, 0, 0

#html_dir  = "/home/pi/CubeSatSim/groundstation/public_html/"
html_dir = "/home/pi/fctelem/public_html/"
image_dir = "/home/pi/fctelem/"
image = "image_file"
ssdv = "/home/pi/ssdv/ssdv -d -J "

print(datetime.datetime.now())

system_and_print("sudo rm " + image_dir + image)
#system_and_print("sudo rm " + html_dir + "*")
system_and_print("sudo rm " + html_dir + "/images/*")

#system_and_print("cp /home/pi/CubeSatSim/sstv/sstv_image_1_320_x_256.jpg " + html_dir + "image_file.jpeg")

head_string = '<HEAD><meta http-equiv="refresh" content="5"><title>FunCube CubeSatSim Telemetry</title></HEAD>\n<HTML>\n<H2>FunCube CubeSatSim Telemetry</H2>' + \
		'<p><pre>  <img height="256" width="320" src="' + image + '.jpeg"><br>                 <A HREF="images" target="_blank">All images</a><br>'
foot_string = '</HTML>'
telem_string_format = "           Image: {image_id:3d} count: {image_count:2d}<p>" + \
		" Vx(mV): {Vx:5d}   Vy(mV): {Vy:5d}   Vz(mV): {Vz:5d}<p>" + \
		" Ix(mA): {Ix:5d}   Iy(mA): {Iy:5d}   Iz(mA): {Iz:5d}<p>" + \
  		"     Vbat(mV): {Vb:5d}   Ibat(mA): {Ib:5d}<br><br></pre>" + \
    		" Freq: {frequency:10.1f} errors: {errors} Seq: {sequence:d} {frame_type} frames: {frame_count:d}"
label_string = " frm,   freq off, err,   h,   seq,     frame, img,  c,    Vx,    Vy,    Vz,    Ix,    Iy,    Iz,    Vb,    Ib "
label_string_html = label_string.replace(" ","&nbsp;")
csv_format = "{frame_count:4d}, {frequency:10.1f}, {errors:3d}, {first_byte: 2x}, {sequence:5d}, {frame_type:9s}, {image_id:3d}, {image_count:2d}, " + \
		"{Vx:5d}, {Vy:5d}, {Vz:5d}, {Ix:5d}, {Iy:5d}, {Iz:5d}, {Vb:5d}, {Ib:5d}"


telem_string = fstr(telem_string_format)
with open(html_dir + "index.html", "w+") as html_file:
	html_file.write(head_string)
	html_file.write(telem_string)
	html_file.write(foot_string)

with open(html_dir + "/images/telem.csv.txt", "w+") as csv_file:
#	csv_file.write(csv_format)
	csv_file.write(label_string)
	csv_file.write("\n")

image_id = 256 		# set illegal image ID for null # random.randint(0, 255)
image_count = 1;

# sys.exit()

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
			print(frequency_string)
			frequency = float(frequency_string[:len(frequency_string) - 2])
			print(frequency)
			errors = int(data_block_string[5])
			data_block = [int(number_string,16) for number_string in data_block_string[7:]]
			first_byte = data_block[0]
			if (first_byte == 0xE0) or (first_byte == 0xE1):
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
						print("Writing this image payload block to file " + image + "_payload")
						immutable_payload = bytes(bytearray(data_block[(FC_PAYLOAD + extended):]))   # payload)
#						print(immutable_payload)
						with open(image_dir + image + "_payload", "wb") as binary_file:
    							binary_file.write(immutable_payload)
					except:
						print("File error")
#					try:
					print("Processing payload with ssdv, saving image to " + image + "_paylad.jpeg and log to ssdv_output")
					system_and_print(ssdv + image_dir + image + "_payload " + 
					       image_dir + image + "_payload.jpeg 2>&1 | sudo tee /home/pi/fctelem/ssdv_output")
					print("Parsing ssdv_output")
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
										print("Saving complete image")
										system_and_print("cp " + html_dir + "image_file.jpeg " + html_dir + "images/" + image + str(image_id) + "." + str(image_count) + ".jpeg")
										system_and_print("sudo rm " + image_dir + image)
									else:
										system_and_print("sudo rm " + image_dir + image)
									print("New Image ID: ")
									print(new_image_id)
									image_id = new_image_id
									image_count = 1								
								else:
									image_count += 1
									print("new image_count:")
									print(image_count)
								print("Appending block to file " + image)	
								with open(image_dir + image, "ab") as binary_file:
    									binary_file.write(immutable_payload)
								print("Running ssdv with all blocks in file " + image + " saving as " + html_dir + "image_file.jpeg")
								system_and_print(ssdv + image_dir + image + " " + html_dir + "image_file.jpeg")
								telem_string = fstr(telem_string_format)
								print("Writing index.html file")
								with open(html_dir + "index.html", "w") as html_file:
									html_file.write(head_string)
									html_file.write(telem_string)
									html_file.write("<br><pre>")
									with open(html_dir + "/images/telem.csv.txt", "r") as csv_file:
										for line in csv_file:
											html_file.write(line)
											html_file.write("<br>")
									tlm_string = fstr(csv_format)		
									html_file.write(tlm_string)
									html_file.write("</pre><br>")
									html_file.write(foot_string)
				else:
					print("Payload not an image!")
		#			image_id = 256 # set illegal image_id to force new image
			else:
				print("Unknown Sat Id or Frame")
				sequence, image_id, image_count  = 0, 0, 0
				Vx, Vy, Vz, Vb = 0, 0, 0, 0
				Ix, Iy, Iz, Ic, Ib = 0, 0, 0, 0, 0
				frame_count, frame_type = 0, " "
				frequency_string, errors = " ", 0
				
			tlm_string = fstr(csv_format)	
			with open(html_dir + "/images/telem.csv.txt", "a") as csv_file:
				csv_file.write(tlm_string)
				csv_file.write("\n")

						
