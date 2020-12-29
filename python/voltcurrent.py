"""Sample code and test for adafruit_in219"""
# Reads all voltage and current sensors for two I2C buses
import time
import sys
#import board
import busio
from adafruit_extended_bus import ExtendedI2C as I2C
from adafruit_ina219 import INA219

if __name__ == "__main__":
#	    print 'Length: ', len(sys.argv)
  
  buses = [1, 3] # default I2C buses
  config = False
  
  if (len(sys.argv)) > 1:
#   print("There are arguments!")
#   if (('a' == sys.argv[1]) or ('afsk' in sys.argv[1])):
    buses[0] = int(sys.argv[1], base=10)
    if (len(sys.argv)) > 2:
      buses[1] = int(sys.argv[2], base=10)
      if (len(sys.argv)) > 3:
        if sys.argv[3] == "c":
          config = True
          from adafruit_ina219 import ADCResolution, BusVoltageRange

  addresses = [0x40, 0x41, 0x44, 0x45] #INA219 addresses on the bus
#  print("buses: ", buses, " addr: ", addresses)

# config

  for x in buses:
    try:
      i2c_bus = I2C(x) # Device is /dev/i2c-x
      for y in addresses:
 #     print(x,y)
        try:
  # Create library object using  Extended Bus I2C port
#        print("bus: ", x, " addr: ", y)
          if x == 0 and y == 0x45:
#           print("Reading INA219 in MoPower Board")
            i2c_bus = I2C(1)
            ina219 = INA219(i2c_bus, 0x4a)
          else:
            ina219 = INA219(i2c_bus, y)
#         print("ina219 test")
  
#         print("Configuring")
# optional : change configuration to use 32 samples averaging for both bus voltage and shunt voltage
          ina219.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S   # 1S
          ina219.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S     # 1S
# optional : change voltage range to 16V
          ina219.bus_voltage_range = BusVoltageRange.RANGE_16V
  
# No try checking yet

  i2c_one = I2C(buses[0])
  i2c_two = I2C(buses[1])
  
  if (buses[0] == 0) and (addresses[0] == 0x45)):
#    print("Reading INA219 in MoPower Board")
    ina219_one = INA219(I2C(1), 0x4a) 
  else
    ina219_one = INA219(i2c_one, addresses[0])
  ina219_two = INA219(i2c_one, addresses[1])
  ina219_three = INA219(i2c_one, addresses[2])
  ina219_four= INA219(i2c_one, addresses[3])
  ina219_five= INA219(i2c_two, addresses[4])  
  ina219_six = INA219(i2c_two, addresses[5]) 
  ina219_seven = INA219(i2c_two, addresses[6]) 
  ina219_eight = INA219(i2c_two, addresses[7]) 

  while (True):
    
    print("{:6.3f} ".format(ina219_one.bus_voltage), "{:6.3f} ".format(ina219_one.current) , end = '') 
    print("{:6.3f} ".format(ina219_two.bus_voltage), "{:6.3f} ".format(ina219_two.current) , end = '') 
    print("{:6.3f} ".format(ina219_three.bus_voltage), "{:6.3f} ".format(ina219_three.current) , end = '') 
    print("{:6.3f} ".format(ina219_four.bus_voltage), "{:6.3f} ".format(ina219_four.current) , end = '') 
    print("{:6.3f} ".format(ina219_five.bus_voltage), "{:6.3f} ".format(ina219_five.current) , end = '') 
    print("{:6.3f} ".format(ina219_six.bus_voltage), "{:6.3f} ".format(ina219_six.current) , end = '') 
    print("{:6.3f} ".format(ina219_seven.bus_voltage), "{:6.3f} ".format(ina219_seven.current) , end = '') 
    print("{:6.3f} ".format(ina219_eight.bus_voltage), "{:6.3f} ".format(ina219_eight.current) , end = '') 

    print(" ")
    inp = input()
#   print(inp)

