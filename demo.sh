#!/bin/bash

echo -e "\nDemo of CubeSatSim at 434.9 MHz continuously\n\n"

#exit

y=$(last reboot | grep ^reboot | wc -l)
echo $y

if (( $y % 2 == 0 )) 
then
	echo -e "\n Continuous FSK Mode\n\n"
	/home/pi/CubeSatSim/radioafsk f
else

	echo -e "\n Alternating FSK, BPSK, and AFSK telemetry Mode \n\n"
	while true; do
       		echo -e "\n Changing mode ******\n\n"
    
      		/home/pi/CubeSatSim/radioafsk a 5
		/home/pi/CubeSatSim/radioafsk f 5
		/home/pi/CubeSatSim/radioafsk b 5
	done
fi
