#!/bin/bash
# script to run CubicSDR and Direwolf to decode APRS

echo "Script to run CubicSDR and Direwolf for ARISS Radio Pi"

echo 

sudo killall -9 CubicSDR &>/dev/null

sleep 5

sudo modprobe snd-aloop

echo "Now run CubicSDR (Under Pi/Hamradio), Select Generic RTL2832 device then click Start to begin."

echo "Then select File/Session/Open Session then load cubicsdr-aprs-direwolf.xml then Open."

echo 

echo "Hit enter only when CubicSDR is running"

read input

echo

direwolf -r 48000 -c direwolf.conf -t 0

sleep 10

#$SHELL
