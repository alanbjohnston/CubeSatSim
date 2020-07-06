#!/bin/bash

echo -e "\nDemo of CubeSatSim at 434.9 MHz\n"

#exit

y=$(last reboot | grep ^reboot | wc -l)
echo $y

#if (( $y % 2 == 0 )) 
#then
	echo -e "\n Continuous Mode\n\n"
#	/home/pi/CubeSatSim/radioafsk f

	if [ "$1" = "a" ]; then
    		echo "Mode is continuous AFSK"
    		sleep 1 
    		sudo killall -9 sendiq
    		sudo killall -9 sendiq
    		sleep 1    
    		/home/pi/CubeSatSim/radioafsk afsk
	elif [ "$1" = "b" ]; then
   		 echo "Mode is continuous BPSK"
    		sleep 1 
    		sudo killall -9 sendiq
    		sudo killall -9 sendiq
    		sleep 1   
    		/home/pi/CubeSatSim/radioafsk bpsk
	else 
    		echo "Mode is continuous FSK"
    		sleep 1 
    		sudo killall -9 sendiq
    		sudo killall -9 sendiq
    		sleep 1
    		/home/pi/CubeSatSim/radioafsk fsk
	fi
