#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run the PacSat Ground Station for FIAB v4"

echo 

source /home/pi/venv/bin/activate

sudo killall -9 java &>/dev/null

sudo killall -9 zenity &>/dev/null

#FILE=/home/pi/.pacsatprofile  
#if [ ! -f "$FILE" ]; then

   profile=$(zenity --text="Choose what you want to do:" --list 2>/dev/null --width=410 --height=120 --title="PacSat Ground Station" --column="Choice" --column="Result" "PacSat" "Run PacSat Ground Station" "Configure" "Configure the Pacsat Ground Station" "Loopback" "Run a Locally Simulated PacSat")

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

    if [ "$ANS" = "1" ] || [ "$profile" = "PacSat" ] ; then

		echo "You have chosen to run the PacSat Ground Station"
	#        echo "p" > /home/pi/.pacsatprofile 
		echo 
		
		/home/pi/CubeSatSim/groundstation/pacsat.sh


    elif [ "$ANS" = "2" ] || [ "$profile" = "Configure" ] ; then

        echo "You have chosen to configure the PacSat Ground Station"
		echo

		/home/pi/CubeSatSim/groundstation/pacsat-config.sh

	elif [ "$ANS" = "3" ] || [ "$profile" = "Loopback" ] ; then

         echo "You have chosen the Loopback Simulated PacSat Ground Station"
		 
		sleep 1

		/usr/bin/x-terminal-emulator --geometry=120x40 -e "bash /home/pi/CubeSatSim/pacsatsim.sh l"

		sleep 1

		 /home/pi/CubeSatSim/groundstation/pacsat.sh l
	
    else
    
       echo "Please enter only 1 or 2 or 3"
       
    fi  
    

sleep 10

#$SHELL
