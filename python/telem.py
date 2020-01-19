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
ina403v = 0
ina403i = 0
ina403p = 0

FAIL = -1

try:
    from ina219 import INA219
    from ina219 import DeviceRangeError
    INA219DISABLE=-1
    
except:
    INA219DISABLE=1
    print"python package ina219 import failed! Install by typing: sudo pip install pi-ina219==1.1.0"
    
if INA219DISABLE !=1:
    try:
        ina40 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina40.wake();
        ina40.configure(ina40.RANGE_16V)
        ina40.busnum = 0;
        ina40v = ina40.voltage()
        ina40i = ina40.current()
        ina40p = ina40.power()
        ina40.sleep();
    except:
        FAIL = 1
    try:
        ina41 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x41)
        ina41.wake();
        ina41.configure(ina41.RANGE_16V)
        ina41.busnum = 1;
        ina41v = ina41.voltage()
        ina41i = ina41.current()
        ina41p = ina41.power()
        ina41.sleep();
    except:
        FAIL = 1
    try:
        ina44 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x44)
        ina44.wake();
        ina44.configure(ina44.RANGE_16V)
        ina44.busnum = 1;
        ina44v = ina44.voltage()
        ina44i = ina44.current()
        ina44p = ina44.power()
        ina44.sleep();
    except:
        FAIL = 1
    try:
        ina45 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x45)
        ina45.wake();
        ina45.configure(ina45.RANGE_16V)
        ina45.busnum = 1;
        ina45v = ina45.voltage()
        ina45i = ina45.current()
        ina45p = ina45.power()
        ina45.sleep();
    except:
        FAIL = 1
    try:
        ina403 = INA219(SHUNT_OHMS45, MAX_EXPECTED_AMPS45, 0x40)
        ina45.wake();
        ina45.configure(ina403.RANGE_16V)
        ina45.busnum = 3;
        ina403v = ina403.voltage()
        ina403i = ina403.current()
        ina403p = ina403.power()
        ina403.sleep();
    except:
        FAIL = 1
if INA219DISABLE !=1:
	print
	print "+X (0x40) v=",ina40v, "V i=", ina40i, "mA p=", ina40p, "mW "
	print "+Y (0x41) v=",ina41v, "V i=", ina41i, "mA p=", ina41p, "mW "
	print "5V Supply (0x44) v=",ina44v, "V i=", ina44i, "mA p=", ina44p, "mW "
	print "Battery (0x45) v=",ina45v, "V i=", ina45i, "mA p=", ina45p, "mW "
	print "+Z (0x40 3) v=",ina403v, "V i=", ina403i, "mA p=", ina403p, "mW "
	print
