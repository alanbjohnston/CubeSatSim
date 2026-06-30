#!/bin/bash

sudo killall -9 ft857d &>/dev/null
sudo killall -9 socat &>/dev/null
sudo killall -9 rigctld &>/dev/null

echo "Starting virtual serial ports /tmp/vttyA and /tmp/vttyB"

socat -d -d PTY,raw,echo=0,link=/tmp/vttyA PTY,raw,echo=0,link=/tmp/vttyB &

echo "Starting CubeSatSim FT857 emulation"

/home/pi/FT857d/examples/FT857d/ft857d &

echo "Starting rigctl emulating FT857 rig using virtual serial port /tmp/vttyB"

rigctld -m 1022 -r /tmp/vttyB -t 4532 -vv

echo "rigctld ended"
