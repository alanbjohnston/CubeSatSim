#!/bin/bash
# script to run FoxTelem 

echo "Script to run FoxTelem for ARISS Radio Pi"

echo 


sudo killall -9 foxtelem &>/dev/null

/home/pi/Downloads/FoxTelem/FoxTelem &


$SHELL
