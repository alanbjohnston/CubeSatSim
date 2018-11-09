# CubeSatSim

See the Wiki for more details. To build and run the software on a Raspberry Pi 3B, 3B+, or Pi Zero W:

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`make rebuild`

To hear CW telemetry (Morse code), tune your radio or SDR to 435.297 MHz and enter:

`./radiocw`

To stop, Ctrl-C.  To hear AFSK telemetry (X.25 data), your radio or SDR to 440.389 MHz FM, and you should receive telemetry from the CubeSat Sim:

`./radioafsk`

This code uses the Brandenburg Tech Digital Transceiver, based on DigitalTxRxRP 
     https://brandenburgtech.wordpress.com/- 
     
 - cw - Code that sends telemetry in CW (Morse code) using AO-7 format
 - afsk - Code that sends telemetry in 1k2 AFSK X.25 format
 - ax5043 - Source for a library of functions to communicate with the AX5043 and configure the AX5043.
 - libs - External libraries
 - python - Python code for reading I2C sensors for current and temperature
 - spreadsheet - Spreadsheets and macros for analyzing the Simulator telemetry
 - wav - Wave audio files of CW or AFSK telemetry for listening or transmitting usng a CubeSat Simulator Lite

See the Wiki for more details
