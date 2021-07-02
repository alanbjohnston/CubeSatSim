#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run FoxTelem for FIAB v3"

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
        echo "b" > /home/pi/.profile 
	echo 
	
	echo "Enter your CALLSIGN.  If you don't have a callsign, enter a text string that will be displayed on the FoxTelem leader board at https://amsat.org/tlm"
	read callsign
	sudo sed -i "s/callsign=NONE/callsign=$callsign/g" /home/pi/FoxTelemetryData/FoxTelem.properties
	echo
		
	sudo sed -i "s/uploadToServer=false/uploadToServer=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	
	
	sudo sed -i "s/foxTelemCalcsDoppler=false/foxTelemCalcsDoppler=true/g" /home/pi/FoxTelemetryDataFoxTelem.properties	
	
	sudo sed -i "s/foxTelemCalcsPosition=false/foxTelemCalcsPosition=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	
		
	sudo sed -i "s/uploadToServer=false/uploadToServer=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	
		
	sudo sed -i "s/whenAboveHorizon=false/whenAboveHorizon=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	
	
	sudo sed -i "s/soundCard=NONE/soundCard=RTL SDR/g" /home/pi/FoxTelemetryData/FoxTelem.properties	# change to FunCube
	
	
	python3 /home/pi/CubeSatSim/groundstation/loc-foxtelem.py
	
#	echo "Enter your Maidenhead grid square.  It is two letters followed by two numbers followed by two letters with no spaces.  If you don't know your gridsquare, you can look it up here https://dxcluster.ha8tks.hu/hamgeocoding/"
#	read grid
#	sudo sed -i "s/maidenhead=XX00xx/maidenhead=$grid/g" /home/pi/Documents/FITB/FoxTelem.properties
#	echo

    elif [ "$ANS" = "2" ]; then

        echo "You have chosen the CubeSatSim Ground Station profile."
        echo "c" > /home/pi/.profile 
        
    else
    
       echo "Please enter only 1 or 2"
       
    fi  
    
fi    

value=`cat /home/pi/.profile`
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
	/home/pi/Desktop/FoxTelem_1.11b_linux /home/pi/FoxTelemetryData
fi

$SHELL
