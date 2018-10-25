SHUNT_OHMS = 0.01
MAX_EXPECTED_AMPS = 2.5
SHUNT_OHMS45 = 0.1
MAX_EXPECTED_AMPS45 = 0.6
ina40v = 0
ina40i = 0
ina40p = 0
ina4av = 0
ina4ai = 0
ina4ap = 0

FAIL = -1

try:
    from ina219 import INA219
    from ina219 import DeviceRangeError
    INA219DISABLE=1
except:
    INA219DISABLE=1
    
if INA219DISABLE !=1:
    try:
        ina40 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina40.wake();
        ina40.configure(ina40.RANGE_16V)
        ina40v = ina40.voltage()
        ina40i = ina40.current()
        ina40p = ina40.power()
        ina4a = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        
        ina4a.wake();
        ina4a.configure(ina4a.RANGE_16V)
        ina4av = ina4a.voltage()
        ina4ai = ina4a.current()
        ina4ap = ina4a.power()
    except:
        FAIL = 1
print ina40v, ina40i, ina40p, ina4av, ina4ai, ina4ap

if INA219DISABLE !=1:
    ina4A = INA219(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x4a)
    ina4A.wake();
    ina4A.configure(ina4A.RANGE_16V)

    ina40 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
    ina40.wake();
    ina40.configure(ina40.RANGE_16V)

    ina41 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x41)
    ina41.wake();
    ina41.configure(ina41.RANGE_16V)

    ina44 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x44)
    ina44.wake();
    ina44.configure(ina44.RANGE_16V)

    ina45 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x45)
    ina45.wake();
    ina45.configure(ina45.RANGE_16V)

    print  ina40.voltage(), ina40.current(), ina40.power(), ina41.voltage(), ina41.current(), ina41.power(), ina44.voltage(), ina44.current(), ina44.power(), ina45.voltage(), ina45.current(), ina45.power(), ina4A.voltage(), ina4A.current(), ina4A.power()

    ina4A.sleep();
    ina40.sleep();
    ina41.sleep();
    ina44.sleep();
    ina45.sleep();
