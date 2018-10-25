SHUNT_OHMS = 0.01
MAX_EXPECTED_AMPS = 2.5
SHUNT_OHMS45 = 0.1
MAX_EXPECTED_AMPS45 = 0.6
ina40v = 0
ina40i = 0
ina40p = 0
ina41v = 0
ina41i = 0
ina41p = 0
ina44v = 0
ina44i = 0
ina44p = 0
ina45v = 0
ina45i = 0
ina45p = 0
ina4av = 0
ina4ai = 0
ina4ap = 0

FAIL = -1

try:
    from ina219 import INA219
    from ina219 import DeviceRangeError
    INA219DISABLE=-1
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
        ina40.sleep();
    except:
        FAIL = 1
    try:
        ina41 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina41.wake();
        ina41.configure(ina41.RANGE_16V)
        ina41v = ina41.voltage()
        ina41i = ina41.current()
        ina41p = ina41.power()
        ina41.sleep();
    except:
        FAIL = 1
    try:
        ina44 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina44.wake();
        ina44.configure(ina44.RANGE_16V)
        ina44v = ina44.voltage()
        ina44i = ina44.current()
        ina44p = ina44.power()
        ina44.sleep();
    except:
        FAIL = 1
    try:
        ina45 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina45.wake();
        ina45.configure(ina45.RANGE_16V)
        ina45v = ina45.voltage()
        ina45i = ina45.current()
        ina45p = ina45.power()
        ina45.sleep();
    except:
        FAIL = 1
    try:
        ina4a = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)        
        ina4a.wake();
        ina4a.configure(ina4a.RANGE_16V)
        ina4av = ina4a.voltage()
        ina4ai = ina4a.current()
        ina4ap = inaaa.power()
        ina4a.sleep();
    except:
        FAIL = 1
print ina40v, ina40i, ina40p, ina41v, ina41i, ina41p, ina44v, ina44i, ina44p, ina45v, ina45i, ina45p, ina4av, ina4ai, ina4ap

