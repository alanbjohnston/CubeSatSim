SHUNT_OHMS = 0.01
MAX_EXPECTED_AMPS = 2.5
SHUNT_OHMS4 = 0.1
MAX_EXPECTED_AMPS4 = 0.6 

try:
    from ina219 import INA219
    from ina219 import DeviceRangeError
    INA219DISABLE=0
    ina = INA219(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x4a)
    ina4 = INA219(SHUNT_OHMS4, MAX_EXPECTED_AMPS4, 0x45)
except:
    print "Error"
try:
    ina.wake()
except:
    print "Error2"
try:
    ina.configure(ina.RANGE_16V)
    ina.voltage()
    ina.current()
    ina.power()
    ina4.configure(ina4.RANGE_16V)
    ina4.voltage()
    ina4.current()
    ina4.power()
except:
    print "1 INA219 libraries not found or hardware INA219 not found at address 0x4a, defaulting to non-INA219 output"
    INA219DISABLE=1

if INA219DISABLE !=1:
    ina = INA219(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x4a)
    ina.wake();
    ina.configure(ina.RANGE_16V)

    ina1 = INA219(SHUNT_OHMS4, MAX_EXPECTED_AMPS4, 0x40)
    ina1.wake();
    ina1.configure(ina1.RANGE_16V)

    ina2 = INA219(SHUNT_OHMS4, MAX_EXPECTED_AMPS4, 0x41)
    ina2.wake();
    ina2.configure(ina1.RANGE_16V)

    ina3 = INA219(SHUNT_OHMS4, MAX_EXPECTED_AMPS4, 0x44)
    ina3.wake();
    ina3.configure(ina1.RANGE_16V)

    ina4 = INA219(SHUNT_OHMS4, MAX_EXPECTED_AMPS4, 0x45)
    ina4.wake();
    ina4.configure(ina4.RANGE_16V)

    print ina.voltage(), ina.current(), ina.power(), ina1.voltage(), ina1.current(), ina1.power(), ina2.voltage(), ina2.current(), ina2.power(), ina3.voltage(), ina3.current(), ina3.power(), ina4.voltage(), ina4.current(), ina4.power()

    ina.sleep();
    ina1.sleep();
    ina2.sleep();
    ina3.sleep();
    ina4.sleep();
