# CubeSatSim

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that runs on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org).

There are several hardware versions and software branches to go with them - see below for information.

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install. To build and run the software on a Raspberry Pi 3B, 3B+, or Pi Zero W (doesn't work on Pi 4 since rpitx doesn't work on it yet):
Requires:
- Raspbian Stretch or Buster, full desktop or Lite 
- wiringpi
- git
- libasound2-dev
- pi-power-button
- Direwolf
- rpitx

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install. To build and run the software on a Raspberry Pi 3B, 3B+, or Pi Zero W (Does NOT work on a Pi 4 since rpitx does not work on it yet):

`sudo apt-get install -y wiringpi git libasound2-dev i2c-tools`

Run raspi-config and enable the I2C bus by selecting Option 5 Interfacing Options and then Option 5 I2C and selecting Y to enable the ARM I2C bus:

`sudo raspi-config`

Select Finish at the bottom to exit raspi-config. Then type:

`sudo cp /boot/config.txt /boot/config.txt.0`

`sudo nano /boot/config.txt`

Use the down arrow key to go down in the file until you find this line:

`# Additional overlays and parameters are documented /boot/overlays/README `

Add the following two lines under it: (NOTE: there seems to be an issue with the i2c-gpio.dtbo file in the latest kernal.  I'm investigating this temporary fix: https://github.com/raspberrypi/firmware/issues/1401.  If adding dtoverlay=i2c-gpio in this step causes your Pi not to boot, then you are encountering this issue)

`dtoverlay=i2c-gpio`

`dtoverlay=pi3-miniuart-bt`

Press Ctrl-X then type `y` then hit Enter to save the file and exit the editor. You should back at the pi@... prompt.

Edit the cmdline.txt file by typing:

`sudo cp /boot/cmdline.txt /boot/cmdline.txt.0`

`sudo nano /boot/cmdline.txt`

Remove the following text in cmdline.txt to prevent a console from running on the serial port:

`console=serial0,115200`

Press Ctrl-X then type `y` then hit Enter to save the file and exit the editor. You should back at the pi@... prompt.

Reboot by typing:

`sudo reboot now`

after logging back into the Pi, type:

`ls -a /dev/i2c*`

you should see two I2C buses:

`i2c-1  i2c-3`

Continue the install by typing:

`cd`

Then type:

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`git checkout master`

Create a sim.cfg configuration file with your amateur radio callsign (in all caps, no lower case letters!) by typing:
     
`echo "callsign" >> sim.cfg`

Compile the code:

`make rebuild`

`cd`

`git clone https://www.github.com/wb2osz/direwolf`

`cd direwolf`

`make -j`

(takes a while)

`sudo make install`

`make install-rpi`

Note that this last command may fail if you are using Raspbian Lite since there is no Desktop (you may get an error such as this "ln: failed to create symbolic link '/home/pi/Desktop/direwolf.desktop': No such file or directory
make: *** [Makefile.linux:727: install-rpi] Error 1"

`cd`

`git clone https://github.com/F5OEO/rpitx.git`

`cd rpitx`

`./install.sh`

(Takes a while).  It will prompt you if you want to modify /boot/config.txt file.  Type a `y` and the script will complete.

`cd`

`git clone https://github.com/alanbjohnston/pi-power-button.git`

`cd pi-power-button`

`./script/install`

To make the demo.sh script run automatically on boot:

`sudo cp ~/CubeSatSim/systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service`

`sudo systemctl enable cubesatsim`

Now reboot for all the changes to take effect:

`sudo reboot now`

After rebooting, tune your radio or SDR to 434.9 MHz FM, and you should receive telemetry from the CubeSatSim!  The green LED will be on when the CubeSatSim software is running.  The red LED when charging is occuring either through the micro USB or through the solar panels.  The blue LED will illuminate when the CubeSatSim is transmitting.

The demo.sh script alternates between two modes:
1. Continuous FSK telmetry, decodeable by FoxTelem
2. Alternativing between APRS AFSK, FSK, and BPSK telemetry

Pressing and releasing the push button will cause the Pi to reboot and change mode.  The green LED will go off as it reboots.

Pressing and holding the pushbutton for 3 seconds will cause the green LED to flash, then the Pi will shutdown.  The RBF pin can then be safely inserted.  Removing the RBF pin or pressing the push button will cause the Pi to start.

You can stop the service when it is running by SSH into the Pi and typing:

`sudo systemctl stop cubesatsim`

After rebooting, it will start unless you disable it:

`sudo systemctl disable cubesatsim`

See the Wiki for more details https://github.com/alanbjohnston/CubeSatSim/wiki

Older Versions

There are several versions of the hardware and software to go with them:

- The original design used a Digital Transceiver Board for the Raspberry Pi (DigitalTxRxRPi) and APRS AFSK 1200 bps telemetry only.  Use the aprs-digitaltxrx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-digitaltxrx for this and the vB2 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB2-Home
- An updated design that used the Transmitter/Filter Board (TFB) instead of the the Transceiver Board.  For the same APRS AFSK telemetry, use the aprs-rpitx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-rpitx and the vB3 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB3-Home.  Note that this hardware can still use the master code branch.
