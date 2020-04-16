This code is used on the STM32 acting a payload to the CubeSat Simulator.

The interface is via the Serial1 port on the STM32 (pins PA9 and PA10) and the Raspberry Pi UART (pins 8 and 10)

The STM32 can be programmed using the Arduino IDE with the Generic STM32F103C series board and STM32duino bootloader, Maple Mini port.

On the Raspberry Pi, to enable the UART:

sudo nano /boot/config.txt

Add the following line:

dtoverlay=pi3-miniuart-bt

Edit cmdline.txt:

sudo nano /boot/cmdline.txt

Remove the following text in cmdline.txt to prevent a console from running on the serial port:

console=serial0,115200

then reboot.  You can test it with minicom:

sudo apt-get install minicom

minicom -b 9600 -o -D /dev/serial0

Type Control-A, then X, then Return to exit.

See https://github.com/alanbjohnston/CubeSatSim/wiki/Arduino-Payload for ideas on using the Arduino as a payload for the CubeSat Simulator.

