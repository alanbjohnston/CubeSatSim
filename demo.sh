#!/bin/bash

echo -e "\nDemo of CubeSatSim sends CW telemetry once then AFSK continuously\n\n"
sleep 20

echo 'sleep over' >> /home/pi/CubeSatSim/log.txt

# /home/pi/CubeSatSim/radio.cw.once

/home/pi/CubeSatSim/radioafsk >> /home/pi/CubeSatSim/log.txt

echo 'afsk over' >> /home/pi/CubeSatSim/log.txt

