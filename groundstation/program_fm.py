import serial	
from os import system
import sys
from time import sleep

if __name__ == "__main__":
	txf = 434.9e6
	rxf = 435e6
	if (len(sys.argv)) > 1:
		print("There are arguments!")
		txf = float(sys.argv[1])
	if (len(sys.argv)) > 2:
		print("There are more arguments!")
		rxf = float(sys.argv[2])
	print("Programming FM module!\n")
	tx = "{:.4f}".format(txf/1e6)
	rx = "{:.4f}".format(rxf/1e6)
	print(f"Transmit frequency: {tx}")
	print(f"Receive frequency: {rx}")

	system("gpio -g mode 20 out")
	system("gpio -g write 20 1")
	system("gpio -g mode 21 out  # set PD high")
	system("gpio -g write 21 1")

	try:
		ser = serial.Serial("/dev/ttyAMA0", 9600)
		print(ser.portstr)
		uhf_string = "AT+DMOSETGROUP=0," + rx +"," + tx + ",0,3,0,0\r\n"
#		uhf_string = "AT+DMOSETGROUP=0," + rx + "," + tx + "," + rxpl_value + "," + sq + "," + txpl_value + ",0\r\n"
		print(uhf_string)
		for i in range(6):
#			ser.write(b"AT+DMOSETGROUP=0,435.0000,434.9000,0,3,0,0\r\n")
			ser.write(uhf_string.encode())
			sleep(0.1)
		ser.close()
		ser = serial.Serial("/dev/ttyAMA0", 115200) # reset back to 115200 for cubesatsim code for payload sensor data
	except:
		print("Error in serial write")
		
	system("gpio -g write 21 0")
