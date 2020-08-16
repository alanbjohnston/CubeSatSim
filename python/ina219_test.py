"""Sample code and test for adafruit_in219"""

import time
import board
from adafruit_ina219 import ADCResolution, BusVoltageRange, INA219


i2c_bus = board.I2C()

ina219 = INA219(i2c_bus)

print("ina219 test")

# display some of the advanced field (just to test)
print("Config register:")
print("  bus_voltage_range:    0x%1X" % ina219.bus_voltage_range)
print("  gain:                 0x%1X" % ina219.gain)
print("  bus_adc_resolution:   0x%1X" % ina219.bus_adc_resolution)
print("  shunt_adc_resolution: 0x%1X" % ina219.shunt_adc_resolution)
print("  mode:                 0x%1X" % ina219.mode)
print("")

# optional : change configuration to use 32 samples averaging for both bus voltage and shunt voltage
ina219.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S
ina219.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S
# optional : change voltage range to 16V
ina219.bus_voltage_range = BusVoltageRange.RANGE_16V

# measure and display loop
while True:
    bus_voltage = ina219.bus_voltage  # voltage on V- (load side)
    shunt_voltage = ina219.shunt_voltage  # voltage between V+ and V- across the shunt
    current = ina219.current  # current in mA

    # INA219 measure bus voltage on the load side. So PSU voltage = bus_voltage + shunt_voltage
    print("PSU Voltage:   {:6.3f} V".format(bus_voltage + shunt_voltage))
    print("Shunt Voltage: {:9.6f} V".format(shunt_voltage))
    print("Load Voltage:  {:6.3f} V".format(bus_voltage))
    print("Current:       {:9.6f} A".format(current / 1000))
    print("")

    time.sleep(2)
