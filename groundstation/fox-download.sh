#!/bin/bash
# script to run FoxTelem Download

#sudo killall -9 java &>/dev/null

echo "Running FoxTelem with database download of all server data"
echo
#	/home/pi/FoxTelem/FoxTelem /home/pi/FoxTelemetryData-Download
SCRIPTPATH=$(dirname "$0")
cd /home/pi/FoxTelem
echo Starting $SCRIPTPATH/current_foxtelem/FoxTelem.jar
setsid java -Xmx512M -jar FoxTelem.jar "/home/pi/FoxTelemetryData-Download" < /dev/null > /dev/null &

sleep 10
