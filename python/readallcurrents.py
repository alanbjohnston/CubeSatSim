from time import sleep
from ina219 import INA219

ina1 = INA219(shunt_ohms=0.1,
             max_expected_amps = 0.6,
             address=0x40)

ina1.configure(voltage_range=ina1.RANGE_16V,
              gain=ina1.GAIN_AUTO,
              bus_adc=ina1.ADC_128SAMP,
              shunt_adc=ina1.ADC_128SAMP)

ina2 = INA219(shunt_ohms=0.1,
             max_expected_amps = 0.6,
             address=0x41)

ina2.configure(voltage_range=ina2.RANGE_16V,
              gain=ina2.GAIN_AUTO,
              bus_adc=ina2.ADC_128SAMP,
              shunt_adc=ina2.ADC_128SAMP)

ina3 = INA219(shunt_ohms=0.1,
             max_expected_amps = 0.6,
             address=0x44)

ina3.configure(voltage_range=ina3.RANGE_16V,
              gain=ina3.GAIN_AUTO,
              bus_adc=ina3.ADC_128SAMP,
              shunt_adc=ina3.ADC_128SAMP)

ina4 = INA219(shunt_ohms=0.1,
             max_expected_amps = 0.6,
             address=0x45)

ina4.configure(voltage_range=ina4.RANGE_16V,
              gain=ina4.GAIN_AUTO,
              bus_adc=ina4.ADC_128SAMP,
              shunt_adc=ina4.ADC_128SAMP)

while 1:
    v1 = ina1.voltage()
    i1 = ina1.current()
    p1 = ina1.power()
    print('1 {0:0.1f}V {1:0.1f}mA'.format(v1, i1))
    print('{0:0.1f} Watts\n\n'.format(p1/1000))

    v2 = ina2.voltage()
    i2 = ina2.current()
    p2 = ina2.power()
    print('2 {0:0.1f}V {1:0.1f}mA'.format(v2, i2))
    print('{0:0.1f} Watts\n\n'.format(p2/1000))

    v3 = ina3.voltage()
    i3 = ina3.current()
    p3 = ina3.power()
    print('3 {0:0.1f}V {1:0.1f}mA'.format(v3, i3))
    print('{0:0.1f} Watts\n\n'.format(p3/1000))

    v4 = ina4.voltage()
    i4 = ina4.current()
    p4 = ina4.power()
    print('4 {0:0.1f}V {1:0.1f}mA'.format(v4, i4))
    print('{0:0.1f} Watts\n\n\n'.format(p4/1000))

    sleep(1)
