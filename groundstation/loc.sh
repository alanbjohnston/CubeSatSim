#!/bin/bash
#

python3 /home/pi/CubeSatSim/groundstation/loc.py

sudo killall -9 gpredict &>/dev/null

/usr/bin/gpredict
