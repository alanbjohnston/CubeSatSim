#!/bin/bash
# script to run FoxTelem 

echo "Script to configure the PacSat Ground Station for FIAB v4"

echo 

#source /home/pi/venv/bin/activate

sudo killall -9 java &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo killall -9 direwolf &>/dev/null

#FILE=/home/pi/.pacsatprofile  
#if [ ! -f "$FILE" ]; then

#   profile=$(zenity --text="Choice:" --list 2>/dev/null --width=410 --height=120 --title="PacSat Ground Station Choice" --column="Choice" --column="Result" "PacSat" "Receive from CubeSatSim PacSatSim" "Configure" "Configure PacSat Ground Station" "Loopback" "Run a Locally Simulated PacSat")
#   echo $profile

    echo "Here are the PacSat Ground Station configuration choices:"
    echo 
    echo "1. Set PacSat Ground Station Callsign"
    echo
    echo "2. Set PacSat Satellite Callsign"
    echo 
    echo "3. Set Frequencies"
    echo
    echo "4. Reset PacSat Ground Station"
    echo
    echo "5. Reset PacSat Satellite"
    echo
	echo "Which do you choose?  Enter 1 - 5"
    
    read -r ANS

   if [ -z "$ANS" ]; then 
   
	    echo "No choice made.  Exiting."
	    
	    sleep 3
	    
	    exit

   fi

	if [ "$ANS" = "1" ] ; then

        echo "You have chosen to set the PacSat Ground Station Callsign"
		echo 

    elif [ "$ANS" = "2" ] ; then

        echo "You have chosen to set the CubeSatSim PacSat Satellite callsign"
        echo

		/home/pi/CubeSatSim/config -c n

    elif [ "$ANS" = "3" ] ; then

         echo "You have chosen to set the Frequency of the PacSat Ground Station"
         echo 

		 /home/pi/CubeSatSim/config -F n

    elif [ "$ANS" = "4" ] ; then

         echo "You have chosen to reset the PacSat Ground Station"
         echo 
		 echo "Next time you run the Ground Station you will need to Add the PacSatSim spacecraft"
		 echo

		 /home/pi/CubeSatSim/groundstation/pacsat-reset.sh


	elif [ "$ANS" = "5" ] ; then

         echo "You have chosen to reset the PacSat Satellite"
         echo 

		 /home/pi/CubeSatSim/config -X n
		 		 
    else
    
       echo "Please enter only 1 to 5"
       
    fi  
    

sleep 10

#$SHELL
