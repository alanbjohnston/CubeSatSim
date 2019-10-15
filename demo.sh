#!/bin/bash

echo -e "\nDemo of CubeSatSim sends FSK, BPSK, and AFSK telemetry alternately at 434.9 MHz continuously\n\n"

#exit

sudo killall -9 rpitx
sudo killall -9 sendiq
sudo fuser -k 8080/tcp

while true; do
      sleep 1;
      echo -e "\n Changing mode ********************************************************************************\n\n"
      
      timeout 25 /home/pi/CubeSatSim/radioafsk c 3
      sudo killall -9 rpitx
      sudo killall -9 sendiq
      sudo fuser -k 8080/tcp
done
