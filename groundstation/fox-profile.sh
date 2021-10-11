#!/bin/bash
# script to change FoxTelem default profile 

echo "Script to change your default FoxTelem profile for FIAB v3"

echo 

sudo rm /home/pi/FoxTelemetryData/.foxprofile  

/home/pi/CubeSatSim/groundstation/fox-run.sh

sleep 5

#$SHELL
