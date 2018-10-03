SHUNT_OHMS = 0.01
MAX_EXPECTED_AMPS = 2.5

try:
    from ina219 import INA219
    from ina219 import DeviceRangeError
    INA219DISABLE=0
    ina = INA219(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x4a)
    ina.wake()
    ina.configure(ina.RANGE_16V)
    ina.voltage()
    ina.current()
    ina.power()
except:
    print "1 INA219 libraries not found or hardware INA219 not found at address 0x4a, defaulting to non-INA219 output"
    INA219DISABLE=1

if INA219DISABLE !=1:
    ina = INA219(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x4a)
    ina.wake();
    ina.configure(ina.RANGE_16V)

    print ina.voltage(), ina.current(), ina.power()

    ina.sleep();
