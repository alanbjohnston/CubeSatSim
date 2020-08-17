"""Sample code and test for adafruit_in219"""

import time
import board
import busio
from adafruit_extended_bus import ExtendedI2C as I2C
from adafruit_ina219 import ADCResolution, BusVoltageRange, INA219

if __name__ == "__main__":
#	    print 'Length: ', len(sys.argv)
    
  if (len(sys.argv)) > 1:
#   print("There are arguments!")
#   if (('a' == sys.argv[1]) or ('afsk' in sys.argv[1])):
    bus = int(sys.argv[1], base=10)
    if (len(sys.argv)) > 2:
      address = int(sys.argv[2], base=16)
    else:
      address = 0x40
  else:
    bus = 1
    address = 0x40
    
  # Create library object using  Extended Bus I2C port
  i2c_bus = I2C(bus) # 1 Device is /dev/i2c-1
   
  ina219 = INA219(i2c_bus, 0x45)

#   print("ina219 test")

# optional : change configuration to use 32 samples averaging for both bus voltage and shunt voltage
  ina219.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S
  ina219.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S
# optional : change voltage range to 16V
  ina219.bus_voltage_range = BusVoltageRange.RANGE_16V

   bus_voltage = ina219.bus_voltage  # voltage on V- (load side)
   shunt_voltage = ina219.shunt_voltage  # voltage between V+ and V- across the shunt
#    current = ina219.current  # current in mA

    # INA219 measure bus voltage on the load side. So PSU voltage = bus_voltage + shunt_voltage
    print("{:6.3f}".format(bus_voltage + shunt_voltage))
#    print("Shunt Voltage: {:9.6f} V".format(shunt_voltage))
#    print("Load Voltage:  {:6.3f} V".format(bus_voltage))
#    print("Current:       {:9.1f} mA".format(current))
#    print("")

#   time.sleep(2)
