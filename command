#!/bin/bash

echo -e "\nCommand and Control script for CubeSatSim v1.3.2\n"

	FILE=/home/pi/CubeSatSim/command_control
	if [ -f "$FILE" ]; then
    		echo "Radio command and control is ON"
#			echo "Turning Command and control to OFF"
#			sudo rm /home/pi/CubeSatSim/command_control
#			echo "rebooting"
#			sudo systemctl stop rpitx
#			sudo reboot now	
#		fi

	else 
    		echo "Radio command and control is OFF"

		while true
		do
			sleep 60
		done

#		exit 1
	fi

FILE=/home/pi/CubeSatSim/command_control_direwolf
if [[ $(arecord -l | grep card) ]] && [ -f "$FILE" ]; then

	echo "Starting direwolf after 20 second delay"

	sleep 20

	if [ "$1" = "d" ]; then
	
		echo "debug mode"
	
		direwolf -c /home/pi/CubeSatSim/direwolf-cc.conf -t 0l | python3 /home/pi/CubeSatSim/aprs_in.py d
	
	else
	
		direwolf -c /home/pi/CubeSatSim/direwolf-cc.conf -t 0l | python3 /home/pi/CubeSatSim/aprs_in.py
	
	fi
else
	echo "Starting squelch C&C"

	if [ "$1" = "d" ]; then
	
		echo "debug mode"
	
		python3 /home/pi/CubeSatSim/squelch_cc.py d
	
	else
	
		python3 /home/pi/CubeSatSim/squelch_cc.py
	
	fi	 
fi
