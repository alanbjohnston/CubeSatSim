# CubeSatSim

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that run on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org).

Requires:
- wiringpi
- git
- libasound2-dev
- pi-power-button
- Direwolf
- rpitx

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install. To build and run the software on a Raspberry Pi 3B, 3B+, or Pi Zero W:

`sudo apt-get install wiringpi, git, libasound2-dev`

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`git checkout aprs-rpitx-v4`

`cd CubeSatSim`

Edit the afsk/main.c file to set your amateur radio callsign, then 

`make rebuild`

`git clone https://www.github.com/wb2osz/direwolf`

`cd direwolf`

`make -j`

(takes a while)

`sudo make install`

`make install-rpi`

`cd ~/CubeSatSim`

`git clone https://github.com/F5OEO/rpitx.git`

`cd rpitx`

`./install.sh`

(Takes a while).  It will prompt you if you want to modify /boot/config.txt file.  Type a `y` and the script will complete.

`cd ~/CubeSatSim`

`git clone https://github.com/Howchoo/pi-power-button.git`

`cd pi-power-button`

`cp ../python/listen-for-shutdown.py listen-for-shutdown.py`

`./pi-power-button/script/install`

Now reboot for all the changes to take effect:

`sudo reboot now`

After rebooting, to hear AFSK telemetry (X.25 data), tune your radio or SDR to 434.9 MHz FM, and you should receive telemetry from the CubeSat Sim:

`CubeSatSim/radioafsk`

This code uses the Brandenburg Tech Digital Transceiver, based on DigitalTxRxRP  https://brandenburgtech.wordpress.com/ If you don't have the SPI Interface enabled and the board plugged in, you will get an error.

This repository contains:
     
  - afsk - Code that sends telemetry in 1k2 AFSK X.25 format
 - arduino - Sample Arduino sketches to show how payload sensors can be interfaced to CubeSat Simulator
 - ax5043 - Source for a library of functions to communicate with the AX5043 and configure the AX5043.
 - cw - Code that sends telemetry in CW (Morse code) using AO-7 format
 - libs - External libraries
 - python - Python code for reading I2C sensors for current and temperature
 - spreadsheet - Spreadsheets for decoding and analyzing the Simulator telemetry (see https://github.com/alanbjohnston/CubeSatSim/wiki/Decoding-Telemetry for details)
 - wav - Wave audio files of CW or AFSK telemetry for listening or transmitting usng a CubeSat Simulator Lite
 - demo.sh - a shell script to run the Simulator on boot using systemd (see https://github.com/alanbjohnston/CubeSatSim/wiki/Software-Install#autoboot-configuration for how to configure the Pi)

See the Wiki for more details https://github.com/alanbjohnston/CubeSatSim/wiki
