#!/bin/bash
# script to run FoxTelem 

echo "Script to run FoxTelem for ARISS Radio Pi"

echo 

sudo systemctl stop openwebrx

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

/home/pi/Downloads/FoxTelem/FoxTelem &

#/usr/bin/chromium-browser --noerrdialogs --disable-infobars http://localhost:8073 &>/dev/null & 

$SHELL
