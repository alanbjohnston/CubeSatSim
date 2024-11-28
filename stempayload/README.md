This code is for the Raspberry Pi Pico W on the Main PCB.

It is for use with the Arduino IDE. It can be modified to use other IDEs.

To compile this code or add new sensors, see https://github.com/alanbjohnston/CubeSatSim/wiki/Adding-New-Sensors

You don't need to compile this code, you can install a binary image by following these steps: https://github.com/alanbjohnston/CubeSatSim/wiki/2.-Software-Install#raspberry-pi-pico-software

This code is for the Rasbperry Pi Pico W micro controller (it also supports v1.2 Sparkfun Pro Micro or STM32 microcontrollers) acting as a payload to the CubeSatSim in the Main board.  Using the Pro Micro is stongly recommended, as driver and board installation for the STM32 can be problematic, and fake STM32 boards may not work.

Code includes (in rough order of testing):

Blink  The usual Arduino "Hello World" application that blinks the LED for each board.

i2c_scanner  A handy utility for scanning the I2C bus that displays the results in the Serial Monitor at 9600 baud.  If the sensors are installed, the two sensors should be displayed.

STEM_Payload_Test  Blinks the White and Yellow LEDs on the Main Board and also reads the Temperature and Voltage sensors (Was the Green and Blue LEDs on the STEM Payload board).

bme280test Displays the results of reading the BME280 sensor: Temperature, Pressure, Altitude, and Humidity and displays on Serial Monitor 9600 baud.  Requires the following libraries: Adadruit Unified Sensor (specify version 1.0.3), Adafruit_BME280 (specify version 1.1.0)

GetAllData  Displays the results of reading the MPU6050 sensor: Temperature, X, Y, and Z Axes: Acceleration, Gyor (Rotation Rate), and Angle.  Requires the following libraries: MPU6050_tockn.

Payload_BME280_MPU6050_XS  This code answers the query from the Raspberry Pi CubeSatSim software over the UART so that the STEM Payload is marked "OK" in the FoxTelem CubeSatSim-FSK or CubeSatSim-BPSK Health tab and also replies with BME280 and MPU6050 sensor data.  In FoxTelem, this is displayed as the X, Y, and Z Gyro (dps) and in AFSK mode, it is appended to the telemetry string.

For information about programming the Sparkfun Pro Micro or STM32, see the v1.2 wiki instructions: https://github.com/alanbjohnston/CubeSatSim/wiki/v1.2-8.-STEM-Payload-Board



