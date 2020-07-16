#!/bin/bash

echo -e "\nDemo of CubeSatSim at 434.9 MHz\n"

sudo systemctl stop rpitx
sudo systemctl start rpitx

	echo -e "\n Continuous Mode\n\n"
#	/home/pi/CubeSatSim/radioafsk f

	if [ "$1" = "a" ]; then
    		echo "Mode is continuous AFSK"
    		/home/pi/CubeSatSim/radioafsk afsk
	elif [ "$1" = "b" ]; then
   		 echo "Mode is continuous BPSK"  
    		/home/pi/CubeSatSim/radioafsk bpsk
	else 
    		echo "Mode is continuous FSK"
    		/home/pi/CubeSatSim/radioafsk fsk
	fi
