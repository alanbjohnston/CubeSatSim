This code is for the STM32 or Pro Micro acting as a payload to the CubeSatSim.

Code that runs on both boards is in this directory.  Code that only runs on a particular board is in the directory for that board.

Code includes:

i2c_scanner.ino  A handy utility for scanning the I2C bus that displays the results in the Serial Monitor at 9600 baud.  If the sensors are installed and the jumpers set to the correct position, the two sensors should be displayed.

blink_stm32.ino and blink_pro_micro.ino  The usual Arduino "Hello World" application that blinks the LED.

bme280test.ino Displays the results of reading the BME280 sensor: Temperature, Pressure, Altitude, and Humidity and displays on Serial Monitor 9600 baud.  Requires the following libraries: Adadruit Unified Sensor (specify version 1.0.3), Adafruit_BME280 (specify version 1.1.0)

GetAllData.ino Displays the results of reading the MPU6050 sensor: Temperature, X, Y, and Z Axes: Acceleration, Gyor (Rotation Rate), and Angle.  Requires the following libraries: MPU6050_tockn.

The STM32 can be programmed using the Arduino IDE with the Generic STM32F103C series board and STM32duino bootloader, Maple Mini port.

The Sparkfun Pro Micro can also be programed using the Arduino IDE with the Sparkfun Pro Micro board and AVRISP mkII Programmer.

See the Wiki page for more information:

See https://github.com/alanbjohnston/CubeSatSim/wiki/Arduino-Payload 



