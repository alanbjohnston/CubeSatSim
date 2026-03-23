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

    echo "Here are the PacSat configuration choices:"
    echo 
    echo " 1. PacSat Ground Station Configuration"
    echo
    echo " 2. PacSat Ground Station Simulation Configuration"
    echo
	echo " 3. Set Frequencies"
    echo
    echo " 4. Reset PacSat Satellite"
    echo
	echo "Which do you choose?  Enter 1 - 4"
	echo
	
    read -r ANS

   if [ -z "$ANS" ]; then 
   
	    echo "No choice made.  Exiting."
	    
	    sleep 3
	    
	    exit

   fi

	if [ "$ANS" = "1" ]  || [ "$ANS" = "2" ]  ; then

		if [ "$ANS" = "1" ] ; then
			echo "Here are your PacSat Ground Station Configuration choices:"
		else	
			echo "Here are your PacSat Ground Station Simulation Configuration choices:"
		fi	
		echo
	    echo " 1. PacSat Ground Station Callsign"
	    echo
	    echo " 2. Set Remote PacSat Satellite Callsign in Ground Station configuration"
	    echo
	    echo " 3. Reset PacSat Ground Station Configuration"
		echo
	
		read -r CH

		
	    if [ -z "$CH" ]; then 
	   
		    echo "No choice made.  Exiting."
		    
		    sleep 3
		    
		    exit
	
	    fi

		if [ "$CH" = "1" ] ; then

			echo
			if [ "$ANS" = "1" ] ; then
				oldcallsign=$(grep -oP '(?<=callsign=).*$' /home/pi/PacSatGround/PacSatGround.properties)
			else
				oldcallsign=$(grep -oP '(?<=callsign=).*$' /home/pi/PacSatGroundLoop/PacSatGround.properties)
			fi
			echo "Callsign in PacSatGround.properties is "
			echo $oldcallsign
					
			echo "Enter new callsign in all capitals: "
			echo
			read callsign
		
			if [ -z $callsign ] ; then
		
				callsign="$1"
				echo "Keeping value of" $oldcallsign
	
			else

				echo "Configured callsign is "
				echo $callsign
						
				if [ ! "$callsign" = "$oldcallsign" ] ; then

					if [ "$ANS" = "1" ] ; then
					  	sudo sed -i "s/callsign=$oldcallsign/callsign=$callsign/g" /home/pi/PacSatGround/PacSatGround.properties
					  	cat /home/pi/PacSatGround/PacSatGround.properties
					else
					  	sudo sed -i "s/callsign=$oldcallsign/callsign=$callsign/g" /home/pi/PacSatGroundLoop/PacSatGround.properties
					  	cat /home/pi/PacSatGroundLoop/PacSatGround.properties
					fi
				  
				fi
			fi	

		elif [ "$CH" = "2" ] ; then
	
			echo "You have chosen to set the remote CubeSatSim PacSat Satellite callsign in ground station configuration"
			echo
			
			PROPERTIES=/home/pi/PacSatGround/spacecraft/PacSatSim.properties
			PROPERTIES_L=/home/pi/PacSatGroundLoop/spacecraft/PacSatSim.properties

			if [ "$ANS" = "1" ] ; then
				oldcallsign=$(grep -oP '(?<=bbsCallsign=).*(?=-)' $PROPERTIES )
			else
				oldcallsign=$(grep -oP '(?<=bbsCallsign=).*(?=-)' $PROPERTIES_L )
			fi
			echo "Current value of remote PacSat callsign is"	
			echo $oldcallsign
			echo
				
			echo "Enter new callsign in all capitals: "
			echo
			read callsign
		
			if [ -z $callsign ] ; then
		
				callsign="$1"
				echo "Keeping value of" $oldcallsign
	
			else
				if [ "$ANS" = "1" ] ; then
					sudo sed -i "s/$oldcallsign/$callsign/g" $PROPERTIES 
				else
					sudo sed -i "s/$oldcallsign/$callsign/g" $PROPERTIES_L 
				fi
				echo
				echo "Changing callsign to "
				echo $callsign
				echo 
				echo "You will see the change next time you run the PacSat Ground Station"
				echo
			fi
	
	    elif [ "$CH" = "3" ] ; then
	
	         echo "You have chosen to reset the PacSat Ground Station Configuration"
	         echo 
#			 echo "Next time you run the Ground Station you will need to Add the PacSatSim spacecraft"
#			 echo
			 
			 if [ "$ANS" = "1" ] ; then
			 	sudo rm -r /home/pi/PacSatGround
	#			cd
	#			sudo rm PacSatGround.zip
	#			wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b-p-s/spacecraft/PacSatGround_0.46o/PacSatGround.zip
	#			unzip PacSatGround.zip -d PacSatGround
	#			sudo rm PacSatGround.zip
			 else
			 	sudo rm -r /home/pi/PacSatGroundLoop
	#			cd
	#			sudo rm PacSatGround.zip
	#			wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b-p-s/spacecraft/PacSatGround_0.46o/PacSatGround.zip
	#			unzip PacSatGround.zip -d PacSatGroundLoop
	#			sudo rm PacSatGround.zip				
			 fi
		else

			echo "Please choose an option 1-3"
			echo

		fi	 

    elif [ "$ANS" = "3" ] ; then

         echo "You have chosen to set the Frequency of the PacSat Ground Station"
         echo 

		 /home/pi/CubeSatSim/config -F n

	elif [ "$ANS" = "4" ] ; then

         echo "You have chosen to reset the PacSat Satellite"
         echo 

		 /home/pi/CubeSatSim/config -k n
		 		 
    else
    
       echo "Please enter only 1 to 4"
       
    fi  
    

# sleep 10

#$SHELL
