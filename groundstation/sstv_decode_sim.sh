#!/bin/bash
# script to decode SSTV using rtl_fm and QSSTV from the CubeSatSim

echo "Script to decode SSTV from the CubeSatSim for ARISS Radio Pi"

echo

sudo systemctl stop openwebrx

sudo modprobe snd-aloop

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

#echo "s" >> .mode

qsstv &

sleep 5

#sudo systemctl restart cubesatsim

value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f 434.9M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1 
rtl_fm -M fm -f 434.9M -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1

sleep 5

#$SHELL
