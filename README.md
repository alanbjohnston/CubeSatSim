# CubeSatSim

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that runs on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org).

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

Add the following two lines under it:

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

Where <branch> is the name of this branch of the software, type:

`git checkout universal`

Create a sim.cfg configuration file with your amateur radio callsign by typing:
     
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

After rebooting, tune your radio or SDR to 434.9 MHz FM, and you should receive telemetry from the CubeSatSim!

You can stop the service: 

`sudo systemctl stop cubesatsim`

After rebooting, it will start unless you disable it:

`sudo systemctl disable cubesatsim`

See the Wiki for more details https://github.com/alanbjohnston/CubeSatSim/wiki
