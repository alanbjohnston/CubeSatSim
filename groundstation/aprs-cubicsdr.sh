#!/bin/bash
# script to run CubicSDR and Direwolf to decode APRS

echo "Script to run CubicSDR and Direwolf for ARISS Radio Pi"

echo 

sudo killall -9 CubicSDR

sleep 5

sudo modprobe snd-aloop

echo "Run CubicSDR, Select Generic RTL2832 device then click Start to begin"

echo "Then select File/Session/Open Session then load cubicsdr-aprs-direwolf.xml"

echo 

echo "Hit enter only when CubicSDR is running"

read input

echo

direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf.conf -t 0

sleep 10

#$SHELL
