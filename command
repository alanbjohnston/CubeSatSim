#!/bin/bash

echo -e "\nCommand and Control script for CubeSatSim v2.1\n"

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

if [ "$1" = "d" ]; then

	echo "debug mode"

	debug=1

else

	debug=0

fi

uptime=`cat /proc/uptime | awk '{printf "%0.f", $1}'`

echo -n "Uptime since boot is "
echo $value

if [[ "$uptime" -lt  "60" ]]; then

	echo "Waiting 20 seconds for USB"

	sleep 20
fi

FILE=/home/pi/CubeSatSim/command_control_direwolf
if [[ $(arecord -l | grep "USB Audio Device") ]] && [ -f "$FILE" ]; then

	echo "Starting Direwolf DTMF and APRS Command and Control"

	if [ "$debug" = "1" ]; then
	
		echo "debug mode"
	
		direwolf -c /home/pi/CubeSatSim/direwolf-cc.conf -t 0l | python3 /home/pi/CubeSatSim/dtmf_aprs_cc.py d
	
	else
	
		direwolf -c /home/pi/CubeSatSim/direwolf-cc.conf -t 0l | python3 /home/pi/CubeSatSim/dtmf_aprs_cc.py
	
	fi
else

	if [ -f "$FILE" ]; then

		echo "Direwolf mode set but no USB soundcard detected!"

#		echo "Trying RTL-FM"

		timeout 1 rtl_test &> out.txt
		if [[ $(grep "No supported" out.txt) ]] ; then

			echo "No RTL-SDR detected.  Command and control is OFF"
#			sleep 60
			exit

		else

			echo "RTL-SDR detected."
			echo "Command and control is OFF"
#			sudo modprobe snd-aloop
	
#			value=`aplay -l | grep "Loopback"`
#			echo "$value" > /dev/null
#			set -- $value

#			card=${2:0:1}

#			value=`cat /home/pi/CubeSatSim/sim.cfg`
#			echo "$value" > /dev/null
#			set -- $value

#			freq=$8
		
#			echo "Current value of rx is"	
#			echo $freq
	
#			rtl_fm -M fm -f ${freq}M -s 48k | aplay -D plughw:${card},0,0 -r 48000 -t raw -f S16_LE -c 1 &
	
#			if [ "$debug" = "1" ]; then
			
#				echo "debug mode"
			
#				direwolf -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf.conf -t 0l | python3 /home/pi/CubeSatSim/dtmf_aprs_cc.py d
			
#			else
			
#				direwolf -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf.conf -t 0l | python3 /home/pi/CubeSatSim/dtmf_aprs_cc.py
			
#			fi

			sleep 5
		fi
		rm out.txt
	else

		echo "Starting Carrier (squelch) Command and Control"

	fi

	if [ "$1" = "d" ]; then
	
		echo "debug mode"
	
		python3 /home/pi/CubeSatSim/squelch_cc.py d
	
	else
	
		python3 /home/pi/CubeSatSim/squelch_cc.py
	
	fi	 
fi

sudo killall -9 direwolf  &>/dev/null
sudo killall -9 rtl_fm  &>/dev/null

