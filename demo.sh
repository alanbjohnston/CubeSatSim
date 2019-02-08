#!/bin/bash

echo -e "\nDemo of CubeSatSim sends AFSK telemetry at 440 MHz continuously\n\n"

#sleep 1

echo $(date '+%Y %b %d %H:%M') Hostname $HOSTNAME  >> /home/pi/CubeSatSim/log.txt

echo 'sleep over' >> /home/pi/CubeSatSim/log.txt

# /home/pi/CubeSatSim/radio.cw.once

/home/pi/CubeSatSim/radioafsk >> /home/pi/CubeSatSim/log.txt

echo 'afsk over' >> /home/pi/CubeSatSim/log.txt

