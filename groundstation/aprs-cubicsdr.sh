#!/bin/bash
# script to run CubicSDR and Direwolf to decode APRS

echo "Script to run CubicSDR and Direwolf for ARISS Radio Pi"

echo 

echo "Note: Run CubicSDR, Select Generic RTL2832 device then click Start to begin"

echo "Then tune to 144.39 MHz NBFM and Audio Output hw:Loopback,0

echo 

echo

sudo modprobe snd-aloop

sleep 5

direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf.conf -t 0

sleep 10

#$SHELL
