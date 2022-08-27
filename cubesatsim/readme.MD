This directory has experimental CubeSatSim code for the Raspberry Pi Pico.  It is functionally equivalent to the CubeSatSim code that runs on the Raspberry Pi Zero, but on runs on the cheaper and easier to find Raspberry Pi Pico microcontroller.

Here's how to use it.

If all you want is a binary file to program your Pico, then go to the Releases t

You will need the Arduino IDE (Integrated Development Environment) application which you can download here: https://www.arduino.cc/en/software for different OSes. 
I am using version 1.18.19

You will need the cubesatsim code on your computer.  If you have git installed, you can clone the repository by typing:

git clone https://github.com/alanbjohnston/CubeSatSim.git
git checkout pico
git pull

If you don't, you can download a ZIP file with the code https://github.com/alanbjohnston/CubeSatSim/archive/refs/heads/pico.zip

Unzip the pico.zip file

Run the Arduino IDE
In the menu, select File Open then move to the CubeSatSim-pico/cubesatsim directory and you will see cubesatsim.ino.  Select this file then click Open.

The "sketch" will open and you should see a view like this.

Next, you need to install the Raspberry Pi Pico Arduino core, called 'Arduino-Pico'. From their documentation: "Arduino-Pico is a community port of the RP2040 (Raspberry Pi Pico processor) to the Arduino ecosystem, intended to make it easier and more fun to use and program the Raspberry Pi Pico / RP2040 based boards."

Here is the documentation for Arduino-Pico: https://arduino-pico.readthedocs.io/en/latest/

Follow the installation instructions listed here: https://arduino-pico.readthedocs.io/en/latest/install.html

If you use Windows, pay attention to this: "Note for Windows Users: Please do not use the Windows Store version of the actual Arduino application because it has issues detecting attached Pico boards. Use the “Windows ZIP” or plain “Windows” executable (EXE) download direct from https://arduino.cc. and allow it to install any device drivers it suggests. Otherwise the Pico board may not be detected."

Once you have installed it, you can now select the Pico board so you can compile the code.

Under the menu Tools / Board select "Raspbery Pi RP2040 Boards(2.1.1)" then select "Raspberry Pi Pico" which is the first one in the list.  You can use a Pico W board, but select it as "Raspberry Pi Pico" anyway.

Next you need to install some libraries.

You need to download the Arduino-APRS library and the SSTV-Arduino-Scottie1 library and add to your Arduino libraries.

Go to the release page for each: https://github.com/alanbjohnston/Arduino-APRS/releases and https://github.com/alanbjohnston/SSTV-Arduino-Scottie1/archive/refs/heads/pico-lib.zip

Select the latest release then download the zip files Arduino-APRS-Library.zip and SSTV-Arduino-Scottie-Library.zip

In the Arduino IDE, under the menu select Sketch / Include Library then select "Add .ZIP library" find the ZIP files you downloaded and add both of them.

Next, install some libraries by going under the menu Tools/ Manage Libraries.  At the search box at the top, type MPU6050_tockn and hit enter.  Mouse over the library desciption and click on the Install button.

Do the same for the following libraries:

Adafruit INA219
Adafruit BME280
Pico timer

Find your
