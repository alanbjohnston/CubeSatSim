# CubeSatSim

This is an experimental balloon payload version of the CubeSatSim software.  It has issues and is not ready to fly on a high altitude balloon.

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that runs on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org).

There are several hardware versions and software branches to go with them - see below for information.

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install. To build and run the software on a Raspberry Pi 4B, 3B, 3B+, Pi Zero or Pi Zero W:
Requires:
- Raspbian Stretch or Buster, full desktop or Lite 
- wiringpi
- git
- libasound2-dev
- pi-power-button
- Direwolf
- rpitx

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install. Runs on a Raspberry Pi 3B, 3B+, or Pi Zero W (Does NOT work on a Pi 4 since rpitx does not work on it yet).  The Pi Zero W or Pi Zero are recommended since they are draw the least power and will result in the best performance under battery power.

These instructions assume a Pi Zero W with WiFi connectivity.  If you have a Pi Zero, follow these instructions to get connectivity: https://desertbot.io/blog/headless-pi-zero-ssh-access-over-usb-windows

To begin the software install, after logging in type:

`sudo apt update -y && sudo apt dist-upgrade -y`

`sudo apt install -y wiringpi git libasound2-dev i2c-tools`

If you are installing on a Pi 4B, you will need to install the 2.52 version of wiring pi by following these steps (you can skip these steps for any other Pi version):

`cd /tmp`
`wget https://project-downloads.drogon.net/wiringpi-latest.deb`
`sudo dpkg -i wiringpi-latest.deb`
`cd`

Run raspi-config and enable the I2C bus by selecting Option 5 Interfacing Options and then Option 5 I2C and selecting Y to enable the ARM I2C bus:

`sudo raspi-config`

Select Finish at the bottom to exit raspi-config. Then type:

`sudo cp /boot/config.txt /boot/config.txt.0`

`sudo nano /boot/config.txt`

Use the down arrow key to go down in the file until you find this line:

`# Additional overlays and parameters are documented /boot/overlays/README `

Add the following two lines under it: (NOTE: there seems to be an issue with the i2c-gpio.dtbo file in the latest kernal.  I'm investigating this temporary fix: https://github.com/raspberrypi/firmware/issues/1401.  If adding dtoverlay=i2c-gpio in this step causes your Pi not to boot, then you are encountering this issue)

`dtoverlay=i2c-gpio,bus=3,i2c_gpio_delay_us=1,i2c_gpio_sda=23,i2c_gpio_scl=24`

`dtoverlay=pi3-miniuart-bt`

Press Ctrl-X then type `y` then hit Enter to save the file and exit the editor. You should back at the pi@... prompt.

Edit the cmdline.txt file by typing:

`sudo cp /boot/cmdline.txt /boot/cmdline.txt.0`

`sudo nano /boot/cmdline.txt`

Remove the following text in cmdline.txt to prevent a console from running on the serial port:

`console=serial0,115200`

Press Ctrl-X then type `y` then hit Enter to save the file and exit the editor. You should back at the pi@... prompt.

Now install the python packages:

`sudo apt install -y python3-pip python-smbus`

`sudo pip3 install --upgrade setuptools`

`sudo pip3 install adafruit-blinka RPI.GPIO adafruit-extended-bus adafruit-circuitpython-ina219` 

Reboot by typing:

`sudo reboot now`

after logging back into the Pi, type:

`ls -a /dev/i2c*`

you should see two I2C buses:

`i2c-1  i2c-3`

Note, you might see these two I2C buses - this is fine to:

`i2c-1  i2c-11`

Continue the install by typing:

`cd`

Then type:

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`git checkout master`

Create a sim.cfg configuration file with your amateur radio callsign (in all caps, no lower case letters!) by typing:
     
`echo "callsign" >> sim.cfg`

`echo "ARG1=f" >> .mode`

This will set the telemetry mode to FSK.  To set it to AFSK or BPSK, change it to ARG1=a or ARG1=b 
Compile the code:

`make rebuild`

`cd`

`git clone https://github.com/alanbjohnston/direwolf.git`

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

`sudo cp ~/CubeSatSim/systemd/rpitx.service /etc/systemd/system/rpitx.service`

`sudo systemctl enable rpitx`

Now reboot for all the changes to take effect:

`sudo reboot now`

After rebooting, tune your radio or SDR to 434.9 MHz FM, and you should get a signal from the CubeSatSim!  If you just have a Pi, you will only hear your Morse Code (CW) callsign - no telemetry signal will be transmitted, since there is no Band Pass Filter installed to block interfering transmissions.  If you have built the Main Board or the TFB and it is plugged into your Pi, you will hear telemetry readio signals.

On the Main Board, the green LED will be on when the CubeSatSim software is running.  The red LED when charging is occuring either through the micro USB or through the solar panels.  The blue LED will illuminate when the CubeSatSim is transmitting.

The push button with the pi-power-button software will cause the Pi to reboot, change telemetry mode, or shutdown.  Pressing and holding the pushbutton will make the green power LED blink first once, then two times, then three times, then blinks slowly.  Depending on when you release the button, different things will happen.  Here's what happens if you:

Press and release (don't hold button in at all): reboots CubeSatSim.  The green LED will go out, and after 30 seconds, the CubeSatSim will be transmitting again.

Press and release after one blink of green LED: switches to AFSK telemetry mode. After about 5 seconds, the telemetry mode will switch to AFSK.

Press and release after two blinks of green LED: switches to FSK mode. After about 5 seconds, the telemetry mode will switch to FSK.

Press and release after three blinks of green LED: switches to BPSK mode. After about 5 seconds, the telemetry mode will switch to BPSK.

Press and release after four blinks of green LED: switches to SSTV mode. After about 5 seconds, the telemetry mode will switch to SSTV transmitting PD120 mode SSTV.

Press and release after green LED begins slow blinking: shuts down CubeSatSim.  

Once the CubeSatSim shuts down, the RBF pin can then be safely inserted.  Removing the RBF pin or pressing the push button will cause the CubeSatSim to start up again.  It will use the same mode it was running when it was shutdown.

You can also change the telemetry mode using the command line.  Edit the CubeSatSim/.mode file and change the value to change the mode. A value of ARG1=a will give you AFSK, ARG1=f will give you FSK, ARG1=b gives BPSK, and ARG1=s gives SSTV. After saving the .mode file, restart the cubesatsim service to switch the mode by typing:

`sudo systemctl restart cubesatsim`

Note that to get FoxTelem to decode BPSK, you need to be in BPSK Fox/Husky mode (depending on which version of FoxTelem).  Also, you usually need to click on the center of the FFT.  For the first 30 seconds, it is just a carrier, so there will be no lock.  After that, it should lock and the Phasor will show a line that jumps around, and the Frame count should start increasing at the bottom of the FoxTelem window.

You can stop the service when it is running by SSH into the Pi and typing:

`sudo systemctl stop cubesatsim`

After rebooting, it will start unless you disable it:

`sudo systemctl disable cubesatsim`

See the Wiki for more details https://github.com/alanbjohnston/CubeSatSim/wiki

Older Versions

There are several versions of the hardware and software to go with them:

- The original design used a Digital Transceiver Board for the Raspberry Pi (DigitalTxRxRPi) and APRS AFSK 1200 bps telemetry only.  Use the aprs-digitaltxrx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-digitaltxrx for this and the vB2 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB2-Home
- An updated design that used the Transmitter/Filter Board (TFB) instead of the the Transceiver Board.  For the same APRS AFSK telemetry, use the aprs-rpitx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-rpitx and the vB3 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB3-Home.  Note that this hardware can still use the master code branch.
