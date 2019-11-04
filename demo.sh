#!/bin/bash

echo -e "\nDemo of CubeSatSim sends FSK, BPSK, and AFSK telemetry alternately at 434.9 MHz continuously\n\n"

#exit

sudo killall -9 rpitx
sudo killall -9 sendiq
sudo fuser -k 8080/tcp

y=$(last reboot | grep ^reboot | wc -l)
echo $y

if (( $y % 2 == 0 )) 
then
	echo -e "\n Continuous FSK Mode\n\n"
	/home/pi/CubeSatSim/radioafsk f 7
else

	echo -e "\n Alternating FSK, BPSK, and AFSK telemetry Mode \n\n"
	while true; do
      		sleep 1;
      		echo -e "\n Changing mode ********************************************************************************\n\n"
      
      		timeout 25 /home/pi/CubeSatSim/radioafsk c 3
      		sudo killall -9 rpitx
      		sudo killall -9 sendiq
      		sudo fuser -k 8080/tcp
	done
fi
