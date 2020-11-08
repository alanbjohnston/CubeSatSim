#!/bin/bash

echo -e "\nrpitx for CubeSatSim at 434.9 MHz using python\n"

if [ "$1" = "c" ]; then
    echo "Mode cycling"
    y=$(last reboot | grep ^reboot | wc -l)
    echo $y
else
    python -u /home/pi/CubeSatSim/rpitx.py $1
fi

