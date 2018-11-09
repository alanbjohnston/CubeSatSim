# CubeSatSim

See the Wiki for more details. To build and run the software:

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`make rebuild`

To hear CW telemetry (Morse code), tune your radio or SDR to 435.297 MHz and enter:

`./radiocw`

To stop, Ctrl-C.  To hear AFSK telemetry (X.25 data), your radio or SDR to 440.389 MHz FM, and you should receive telemetry from the CubeSat Sim:

`./radioafsk`

Raspberry Pi code using the Brandenburg Tech Digital Transceiver 
     https://brandenburgtech.wordpress.com/- 
     
 - cw - Code that sends telemetry in CW (Morse code) using AO-7 format
 - afsk - Code that sends telemetry in 1k2 AFSK X.25 format

based on DigitalTxRxRP

- ax5043 - Source for a library of functions to communicate with the AX5043 and configure the AX5043.

