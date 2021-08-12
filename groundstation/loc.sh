#!/bin/bash
#

sudo killall -9 java &>/dev/null

sudo killall -9 gpredict &>/dev/null

python3 /home/pi/CubeSatSim/groundstation/loc-foxtelem.py

/usr/bin/gpredict
