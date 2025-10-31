#!/bin/bash

sudo modprobe snd-aloop

direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-jp14-half.conf -t 0 
