#!/bin/bash

echo -e "\nDemo of CubeSatSim sends FSK and BPSK telemetry alternately at 434.9 MHz continuously\n\n"

sudo killall rpitx
sudo killall sendiq
sudo fuser -k 8080/tcp

while true; do
      timeout 33 /home/pi/CubeSatSim/radioafsk.sh fsk 3
      sudo killall rpitx
      sudo killall sendiq
      sudo fuser -k 8080/tcp
      timeout 33 /home/pi/CubeSatSim/radioafsk.sh bpsk 3
      sudo killall rpitx
      sudo killall sendiq
      sudo fuser -k 8080/tcp     
done
