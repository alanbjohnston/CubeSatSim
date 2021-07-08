#!/bin/bash

echo -e "\nScript to take pictures every 30 seconds\n"

while true; do

    python3 /home/pi/payload/take-photo.py
    echo -e "\nTaking photo!\n"
    sleep 28

done
