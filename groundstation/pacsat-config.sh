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
    echo "2. Set Remote PacSat Satellite Callsign in Ground Station configuration"
    echo 
    echo "3. Set Frequencies"
    echo
    echo "4. Reset PacSat Ground Station Configuration"
	echo
    echo "5. Reset PacSat Ground Station Loopback Configuration"
	echo    
    echo "6. Reset PacSat Satellite"
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

		/home/pi/CubeSatSim/config -c n

    elif [ "$ANS" = "2" ] ; then

        echo "You have chosen to set the remote CubeSatSim PacSat Satellite callsign in ground station configuration"
        echo

		oldcallsign=$(grep -oP '(?<=bbsCallsign=).*(?=-)' /home/pi/PacSatGround/spacecraft/PacSatSim.properties)

		echo "Current value of remote PacSat callsign is"	
		echo $oldcallsign
		echo
			
		echo "Enter new callsign in all capitals: "
		read callsign
	
		if [ -z $callsign ] ; then
	
			callsign="$1"
			echo "Keeping value of" $oldcallsign

		else
	
			sudo sed -i "s/$oldcallsign/$callsign/g" /home/pi/PacSatGround/spacecraft/PacSatSim.properties
			echo
			echo "Changing callsign to "
			echo $callsign
			echo 
			echo "You will see the change next time you run the PacSat Ground Station"
			echo
		fi
		

    elif [ "$ANS" = "3" ] ; then

         echo "You have chosen to set the Frequency of the PacSat Ground Station"
         echo 

		 /home/pi/CubeSatSim/config -F n

    elif [ "$ANS" = "4" ] ; then

         echo "You have chosen to reset the PacSat Ground Station Configuration"
         echo 
		 echo "Next time you run the Ground Station you will need to Add the PacSatSim spacecraft"
		 echo

		 sudo rm -r /home/pi/PacSatGround

    elif [ "$ANS" = "5" ] ; then

         echo "You have chosen to reset the PacSat Ground Station Loopback Configuration"
         echo 
		 echo "Next time you run the Ground Station you will need to Add the PacSatSim spacecraft"
		 echo

		 sudo rm -r /home/pi/PacSatGroundLoop

	elif [ "$ANS" = "6" ] ; then

         echo "You have chosen to reset the PacSat Satellite"
         echo 

		 /home/pi/CubeSatSim/config -k n
		 		 
    else
    
       echo "Please enter only 1 to 5"
       
    fi  
    

# sleep 10

#$SHELL
