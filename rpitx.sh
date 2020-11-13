#!/bin/bash

echo -e "\nrpitx for CubeSatSim at 434.9 MHz using python\n"

if [ "$1" = "c" ]; then
    echo "Mode cycling"
    y=$(last reboot | grep ^reboot | wc -l)
    echo $y
          if [ $(($y % 4)) = 3 ]; then
   	   echo "Mode is continuous AFSK"
    	   python -u /home/pi/CubeSatSim/rpitx.py a
      elif [ $(($y % 4)) = 1 ]; then
   	   echo "Mode is continuous BPSK"  
    	   python -u /home/pi/CubeSatSim/rpitx.py b
      elif [ $(($y % 4)) = 2 ]; then
   	   echo "Mode is continuous SSTV"  
    	   python -u /home/pi/CubeSatSim/rpitx.py s
      else
         echo "Mode is continuous FSK"
         python -u /home/pi/CubeSatSim/rpitx.py f
      fi
else
    python -u /home/pi/CubeSatSim/rpitx.py $1
fi

