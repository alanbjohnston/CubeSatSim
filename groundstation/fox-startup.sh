#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run FoxTelem for ARISS Radio Pi"

echo 

FILE=/home/pi/CubeSatSim/groundstation/.profile  
if [ ! -f "$FILE" ]; then
    echo "You need to choose your default FoxTelem profile."
    echo 
    echo "The choices are:"
    echo
    echo "1. Fox-in-a-Box.  Use this profile if you want to receive and decode telemetry from the AMSAT Fox satellites.  If you enter a callsign and a grid square, you can upload to the AMSAT telemetry server."
    echo 
    echo "2. CubeSatSim Ground Station.  Use this profile if you want to receive and decode telemetry from an AMSAT CubeSatSim or CubeSatSim Lite."
    echo
    echo "Which profile do choose?  Enter 1 or 2"
    
    read -r ANS

    if [ "$ANS" = "1" ]; then

        echo "You have chosen the Fox-in-a-Box profile."
        echo "b" > /home/pi/CubeSatSim/groundstation/.profile 

    elif [ "$ANS" = "2" ]; then

        echo "You have chosen the CubeSatSim Ground Station profile."
        echo "c" > /home/pi/CubeSatSim/groundstation/.profile 
        
    else
    
       echo "Please enter only 1 or 2"
       
    fi  
    
fi    

value=`cat /home/pi/CubeSatSim/groundstation/.profile`
echo "$value" > /dev/null
set -- $value

sudo killall -9 java &>/dev/null

if [ "$1" = "c" ]; then
	echo "CubeSatSim Ground Station profile is set!"	
	/home/pi/Downloads/FoxTelem/FoxTelem &
else
	echo "Fox-in-a-box profile is set!"
	/home/pi/Downloads/FoxTelem-FITB/FoxTelem /home/pi/Documents/FITB

fi

$SHELL
