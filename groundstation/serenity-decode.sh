#!/bin/bash
# script to run CubicSDR and Direwolf to decode Serenity

echo "Script to run CubicSDR and Direwolf for ARISS Radio Pi"

echo 

echo "Note: Select Generic RTL2832 device then click Start to begin"

echo

echo "Then tune 437.1 MHz NBFM"

echo

sudo modprobe snd-aloop

sudo systemctl stop openwebrx

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 rtl_tcp &>/dev/null

sleep 5

direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf-4800.conf -t 0

sleep 5

#setsid CubicSDR
setsid CubicSDR

sleep 10

$SHELL
