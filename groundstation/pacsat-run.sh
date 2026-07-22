#!/bin/bash
# script to run FoxTelem 

echo "Startup script to run the PacSat Ground Station for FIAB v4"

echo 

# source /home/pi/venv/bin/activate

sudo killall -9 java &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

if [ ! $(systemctl get-default | grep graphical) ]; then

	echo "Graphical User Interface not running!"
	echo 
	echo "Do you want to reboot and switch to GUI so you can run the Pacsat Ground Station"
	read answer

	if [ "$answer" = "y" ] || [ "$answer" = "yes" ]; then
		echo "Rebooting!"
		sudo raspi-config nonint do_boot_behaviour B4
		sudo reboot now
	else
		echo "You can run the Pacsat Ground Station next time you run the GUI."
		exit
	fi
fi

#FILE=/home/pi/.pacsatprofile  
#if [ ! -f "$FILE" ]; then

   profile=$(zenity --timeout=10 --text="Choose what you want to do:" --list 2>/dev/null --width=410 --height=220 --title="PacSat Ground Station" --column="Choice" --column="Result" "PacSat" "Run PacSat Ground Station" "Configure" "Configure the Pacsat Ground Station" "Simulate" "Run a Locally Simulated PacSat")

   echo $profile

   if [ -z "$profile" ]; then 
   
	    echo "No choice made.  Default to PacSat Ground Station."
	    
	    profile="PacSat"
    
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

	elif [ "$ANS" = "3" ] || [ "$profile" = "Simulate" ] ; then

		sudo systemctl stop transmit

        echo "You have chosen the PacSat Ground Station with Local Simulated Satellite"
		 
		sleep 1

		/usr/bin/x-terminal-emulator --geometry=120x40 -e "bash /home/pi/CubeSatSim/pacsatsim.sh l"

		sleep 1

		/home/pi/CubeSatSim/groundstation/pacsat.sh l
	
    else
    
       echo "Please enter only 1 or 2 or 3"
       
    fi  
    
# sudo killall -9 direwolf &>/dev/null

sleep 10

#$SHELL
