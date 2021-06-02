#!/bin/bash

echo -e "\nDemo of CubeSatSim at 434.9 MHz\n"

sudo systemctl restart rpitx

if [ "$1" = "c" ]; then
    echo "Mode cycling"
    y=$(last reboot | grep ^reboot | wc -l)
    echo $y
    echo $(($y % 4))    
fi

if [[ ("$1" = "a" ) || (("$1" = "c") && ("$(($y %4))" = 3)) ]]; then
   	echo "Mode is continuous AFSK"
    	/home/pi/CubeSatSim/radioafsk afsk
elif [[ ("$1" = "b" ) || (("$1" = "c") && ("$(($y %4))" = 1)) ]]; then
   	echo "Mode is continuous BPSK" 
    sudo cpulimit -l 2.5 -b -P radioafsk
    /home/pi/CubeSatSim/radioafsk bpsk
elif [[ ("$1" = "s" ) || (("$1" = "c") && ("$(($y %4))" = 2)) ]]; then
   	echo "Mode is continuous SSTV"  
    	while true; do sleep 5; done
else
    echo "Mode is continuous FSK"
    /home/pi/CubeSatSim/radioafsk fsk
fi

