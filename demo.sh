#!/bin/bash

echo -e "\nDemo of CubeSatSim sends FSK and BPSK telemetry alternately at 434.9 MHz continuously\n\n"

sudo killall rpitx
sudo killall sendiq
sudo fuser -k 8080/tcp

while true; do
      sleep 1;
      echo -e "\n Changing mode ********************************************************************************\n\n"
      
      timeout 33 /home/pi/CubeSatSim/radioafsk fsk 3
      sudo killall rpitx
      sudo killall sendiq
      sudo fuser -k 8080/tcp

      sleep 1;
      echo -e "\n Changing mode ********************************************************************************\n\n"
      
      timeout 36 /home/pi/CubeSatSim/radioafsk bpsk 3
      sudo killall rpitx
      sudo killall sendiq
      sudo fuser -k 8080/tcp     
done
