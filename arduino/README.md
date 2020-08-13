This code is for the STM32 or Pro Micro acting as a payload to the CubeSatSim.

Code includes (in rough order of testing):

Blink_STM32_PC13.ino and Blink_Pro_Micro.ino  The usual Arduino "Hello World" application that blinks the LED for each board.

i2c_scanner.ino  A handy utility for scanning the I2C bus that displays the results in the Serial Monitor at 9600 baud.  If the sensors are installed and the jumpers set to the correct position, the two sensors should be displayed.

STEM_Payload_Test_STM32.ino and STEM_Payload_Test_Pro_Micro.ino Blinks the Blue and Green LEDs on the STEM Payload Board and also reads the Temperature and Voltage sensors.

bme280test.ino Displays the results of reading the BME280 sensor: Temperature, Pressure, Altitude, and Humidity and displays on Serial Monitor 9600 baud.  Requires the following libraries: Adadruit Unified Sensor (specify version 1.0.3), Adafruit_BME280 (specify version 1.1.0)

GetAllData.ino Displays the results of reading the MPU6050 sensor: Temperature, X, Y, and Z Axes: Acceleration, Gyor (Rotation Rate), and Angle.  Requires the following libraries: MPU6050_tockn.

PayloadOK_STM32_PC13.ino and PayloadOK_Pro_Micro.ino  This code answers the query from the Raspberry Pi CubeSatSim software over the UART so that the STEM Payload is marked "OK" in the FoxTelem CubeSatSim-FSK Health tab. 

The STM32 can be programmed using the Arduino IDE with the Generic STM32F103C series board and STM32duino bootloader, Maple Mini port.

The Sparkfun Pro Micro can also be programed using the Arduino IDE with the Sparkfun Pro Micro board and AVRISP mkII Programmer.

See the Wiki page for more information:

See https://github.com/alanbjohnston/CubeSatSim/wiki/Arduino-Payload 



