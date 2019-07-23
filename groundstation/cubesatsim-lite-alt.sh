#!/bin/bash
# script to run CubeSat Simulator Lite
#
# on SDR application, listen at 107.9 

echo -e "\nScript to run CubeSat Simulator Lite \n"

sudo /home/pi/CubeSatSim/PiFmRds/src/pi_fm_rds -audio /home/pi/CubeSatSim/wav/afsk2.wav -freq 107.5 
