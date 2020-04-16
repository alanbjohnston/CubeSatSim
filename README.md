# CubeSatSim

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that run on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org).

This is not the latest version of the CubeSatSim - find the master branch.

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/vB2-Software-Install. To build and run the software on a Raspberry Pi 3B, 3B+, or Pi Zero W:

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`git checkout aprs-digitaltxrx`

Edit the afsk/main.c file to set your amateur radio callsign, then 

`make rebuild`

To hear CW telemetry (Morse code), tune your radio or SDR to 435.297 MHz and enter:

`./radiocw`

To stop, Ctrl-C.  To hear AFSK telemetry (X.25 data), your radio or SDR to 440.389 MHz FM, and you should receive telemetry from the CubeSat Sim:

`./radioafsk`

This code uses the Brandenburg Tech Digital Transceiver, based on DigitalTxRxRP  https://brandenburgtech.wordpress.com/ 

This repository contains:
     
  - afsk - Code that sends telemetry in 1k2 AFSK X.25 format
 - arduino - Sample Arduino sketches to show how payload sensors can be interfaced to CubeSat Simulator
 - ax5043 - Source for a library of functions to communicate with the AX5043 and configure the AX5043.
 - cw - Code that sends telemetry in CW (Morse code) using AO-7 format
 - libs - External libraries
 - python - Python code for reading I2C sensors for current and temperature
 - spreadsheet - Spreadsheets for decoding and analyzing the Simulator telemetry (see https://github.com/alanbjohnston/CubeSatSim/wiki/Decoding-Telemetry for details)
 - wav - Wave audio files of CW or AFSK telemetry for listening or transmitting usng a CubeSat Simulator Lite
 - demo.sh - a shell script to run the Simulator on boot using systemd (see https://github.com/alanbjohnston/CubeSatSim/wiki/vB2-Software-Install#autoboot-configuration for how to configure the Pi)

See the Wiki for more details https://github.com/alanbjohnston/CubeSatSim/wiki
