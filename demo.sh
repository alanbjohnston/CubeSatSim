#!/bin/bash

echo -e "\nDemo of CubeSatSim sends AFSK telemetry at 434.9 MHz continuously\n\n"

#exit

sudo killall -9 rpitx
sudo killall -9 sendiq
sudo fuser -k 8080/tcp

/home/pi/CubeSatSim/radioafsk a 3
