#!/bin/bash
#

python3 /home/pi/CubeSatSim/groundstation/loc-foxtelem.py

sudo killall -9 gpredict &>/dev/null

/usr/bin/gpredict
