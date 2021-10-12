#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run FoxTelem for FIAB v3"

echo 

sudo killall -9 FoxTelem &>/dev/null

sudo killall -9 zenity &>/dev/null

FILE=/home/pi/FoxTelemetryData/.foxprofile  
if [ ! -f "$FILE" ]; then

   profile=$(zenity --text="Choose your default FoxTelem profile:" --list 2>/dev/null --width=410 --height=120 --title="Set FoxTelem Profile" --column="Profile" --column="Receive and decode telemetry from" Fox-in-a-Box "AMSAT Fox satellites" "CubeSatSim" "AMSAT CubeSat Simulator")

   echo $profile

   if [ -z "$profile" ]; then 
   
    echo "No choice made.  Exiting."
    
    sleep 3
    
    exit

    #echo "You need to choose your default FoxTelem profile."
    #echo 
    #echo "The choices are:"
    #echo
    #echo "1. Fox-in-a-Box.  Use this profile if you want to receive and decode telemetry from the AMSAT Fox satellites.  If you enter a callsign and a grid square, you can upload to the AMSAT telemetry server."
    #echo 
    #echo "2. CubeSatSim Ground Station.  Use this profile if you want to receive and decode telemetry from an AMSAT CubeSatSim or CubeSatSim Lite."
    #echo
    #echo "Which profile do you choose?  Enter 1 or 2"
    
    #read -r ANS
    
    fi

    if [ "$ANS" = "1" ] || [ "$profile" = "Fox-in-a-Box" ] ; then

        echo "You have chosen the Fox-in-a-Box profile."
        echo "b" > /home/pi/FoxTelemetryData/.foxprofile 
	echo 
	
	sudo killall -9 java &>/dev/null

	sudo killall -9 gpredict &>/dev/null
	
	FILE=/home/pi/FoxTelemetryData/FoxTelem.properties.0
	if [ ! -f "$FILE" ]; then
		cp /home/pi/FoxTelemetryData/FoxTelem.properties /home/pi/FoxTelemetryData/FoxTelem.properties.0
	fi
	
	python3 /home/pi/CubeSatSim/groundstation/loc-foxtelem.py
	
	if [ $? -eq 1 ]; then
	
		echo "Enter your Groundstation Name. If you upload telemetry data to the AMSAT servers, this name will be displayed on the FoxTelem leader board at https://amsat.org/tlm"
		read name

		if [ -n "$name" ]; then
			sudo sed -i "s/callsign=.*/callsign=$name/g" /home/pi/FoxTelemetryData/FoxTelem.properties
		fi

		echo

		sudo sed -i "s/uploadToServer=false/uploadToServer=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/foxTelemCalcsDoppler=false/foxTelemCalcsDoppler=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/foxTelemCalcsPosition=false/foxTelemCalcsPosition=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/uploadToServer=false/uploadToServer=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/whenAboveHorizon=false/whenAboveHorizon=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/retuneCenterFrequency=false/retuneCenterFrequency=true/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

		sudo sed -i "s/stationDetails=.*/stationDetails=FoxInABox/g" /home/pi/FoxTelemetryData/FoxTelem.properties	

	#	sudo sed -i "s/soundCard=NONE/soundCard=RTL SDR/g" /home/pi/FoxTelemetryData/FoxTelem.properties	# change to FunCube
		sudo sed -i "s/soundCard=NONE/soundCard=Direct Audio Device\: FUNcube Dongle V2.0, USB Audi/g" /home/pi/FoxTelemetryData/FoxTelem.properties	# change to FunCube



	#	echo "Enter your Maidenhead grid square.  It is two letters followed by two numbers followed by two letters with no spaces.  If you don't know your gridsquare, you can look it up here https://dxcluster.ha8tks.hu/hamgeocoding/"
	#	read grid
	#	sudo sed -i "s/maidenhead=XX00xx/maidenhead=$grid/g" /home/pi/Documents/FITB/FoxTelem.properties
	#	echo

 	fi

    elif [ "$ANS" = "2" ] || [ "$profile" = "CubeSatSim" ] ; then

        echo "You have chosen the CubeSatSim Ground Station profile."
        echo "c" > /home/pi/FoxTelemetryData/.foxprofile 

	cp /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties.0
	
	sudo sed -i "s/useCostas=false/useCostas=true/g" /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties	
	
	sudo sed -i "s/findSignal=false/findSignal=true/g" /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties	
	
    else
    
       echo "Please enter only 1 or 2"
       
    fi  
    
fi    

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

#$SHELL
