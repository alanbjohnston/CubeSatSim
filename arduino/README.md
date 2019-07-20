This code is used on an Arduino acting as a payload to the CubeSat Simulator.

The Arduino is on the Raspberry Pi i2c bus 0, address 0x4b

The 2c_master_register_read.ino code emulates the register reading of the Raspberry Pi.  You can connect two Arduinos back-to-back to show this.

The i2c_slave_register_read.ino code responds to register reads from a bus master such as another Arduino or the Raspberry Pi

The i2c_slave_with_sensor_reading.ino code responds to register reads from a bus master and has placeholders for reading a sensor.

See https://github.com/alanbjohnston/CubeSatSim/wiki/Arduino-Payload for ideas on using the Arduino as a payload for the CubeSat Simulator.

