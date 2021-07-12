#!/bin/bash

echo -e "\nScript to manage HAB flight transmissions\n"

sudo systemctl restart camera

while true; do

    /home/pi/CubeSatSim/config -a 
    sleep 120

    /home/pi/CubeSatSim/config -s
    sleep 105 

done
