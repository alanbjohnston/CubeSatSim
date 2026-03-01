#!/bin/bash
# script to run FoxTelem 

echo "Startup script to configure the PacSat Ground Station for FIAB v4"

echo 

source /home/pi/venv/bin/activate

sudo killall -9 java &>/dev/null

sudo killall -9 zenity &>/dev/null

#FILE=/home/pi/.pacsatprofile  
#if [ ! -f "$FILE" ]; then

   profile=$(zenity --text="Choice:" --list 2>/dev/null --width=410 --height=120 --title="PacSat Ground Station Choice" --column="Choice" --column="Result" "PacSat" "Receive from CubeSatSim PacSatSim" "Configure" "Configure PacSat Ground Station" "Loopback" "Run a Locally Simulated PacSat")
   echo $profile

   if [ -z "$profile" ]; then 
   
    echo "No choice made.  Exiting."
    
    sleep 3
    
    exit

   fi

    if [ "$ANS" = "1" ] || [ "$profile" = "PacSat" ] ; then

        echo "You have chosen the PacSat profile."
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

        echo "You have chosen the CubeSatSim PacSat Ground Station profile."
        echo "c" > /home/pi/.pacsatprofile 

	cp /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties.0
	
	sudo sed -i "s/useCostas=false/useCostas=true/g" /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties	
	
	sudo sed -i "s/findSignal=false/findSignal=true/g" /home/pi/FoxTelemetryData-CubeSatSim/FoxTelem.properties	

    elif [ "$ANS" = "3" ] || [ "$profile" = "Loopback" ] ; then

         echo "You have chosen the Loopback Simulated PacSat Ground Station profile."
         echo "l" > /home/pi/.pacsatprofile 
	
    else
    
       echo "Please enter only 1 or 2 or 3"
       
    fi  
    
#fi    

#value=`cat /home/pi/FoxTelemetryData/.foxprofile`
#echo "$value" > /dev/null
#set -- $value

sudo killall -9 java &>/dev/null

if [ "$1" = "c" ]; then
	echo "CubeSatSim Ground Station profile is set!"	
	echo
	/home/pi/CubeSatSim/groundstation/pacsat.sh
elif [ "$1" = "l" ]; then
	echo "Loopback Ground Station profile is set!"	
	echo
	/home/pi/CubeSatSim/groundstation/pacsat.sh l  
else
	echo "AMSAT profile is set!"
	echo
#	/home/pi/FoxTelem/FoxTelem /home/pi/FoxTelemetryData
	cd /home/pi/Desktop/PacsatGround/

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &

fi

sleep 10

#$SHELL
