#!/bin/bash
# script to decode SSTV using rtl_fm and QSSTV from the CubeSatSim

echo "Script to decode SSTV from the CubeSatSim for ARISS Radio Pi"

echo

sudo systemctl stop openwebrx

sleep 2

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

echo "ARG1=s" >> .mode

qsstv &

#sudo systemctl restart cubesatsim

rtl_fm -M fm -f 434.9M -s 48k | aplay -D hw:3,0,0 -r 48000 -t raw -f S16_LE -c 1 

$SHELL
