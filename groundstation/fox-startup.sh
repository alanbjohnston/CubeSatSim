#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run FoxTelem for FIAB v3"

echo 

sudo killall -9 FoxTelem &>/dev/null

FILE=/home/pi/FoxTelemetryData/.foxprofile  
#if [ ! -f "$FILE" ]; then
if [ -f "$FILE" ]; then

value=`cat /home/pi/FoxTelemetryData/.foxprofile`
echo "$value" > /dev/null
set -- $value

sudo killall -9 java &>/dev/null

if [ "$1" = "c" ]; then
	echo "CubeSatSim Ground Station profile is set!"	
	echo
	/home/pi/CubeSatSim/groundstation/fox.sh
else
	echo "Fox-in-a-box profile is set!"
	echo
#	/home/pi/FoxTelem/FoxTelem /home/pi/FoxTelemetryData
	SCRIPTPATH=$(dirname "$0")
	cd /home/pi/FoxTelem
	echo Starting $SCRIPTPATH/current_foxtelem/FoxTelem.jar
	setsid java -Xmx512M -jar FoxTelem.jar "/home/pi/FoxTelemetryData" < /dev/null > /dev/null &

fi

sleep 10

fi    

#$SHELL
