#!/bin/bash

echo -e "\nScript to manage HAB flight transmissions\n"

sudo systemctl enable camera

while true; do

    /home/pi/CubeSatSim/config -a 
    sleep 60

    /home/pi/CubeSatSim/config -s
    sleep 120 

done
