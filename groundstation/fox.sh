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

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 zenity &>/dev/null

sleep 5

#/home/pi/FoxTelem/FoxTelem /home/pi/FoxTelemetryData-CubeSatSim

SCRIPTPATH=$(dirname "$0")
cd /home/pi/FoxTelem
echo Starting $SCRIPTPATH/current_foxtelem/FoxTelem.jar
setsid java -Xmx512M -jar FoxTelem.jar "/home/pi/FoxTelemetryData-CubeSatSim" < /dev/null > /dev/null &

sleep 10

#$SHELL
