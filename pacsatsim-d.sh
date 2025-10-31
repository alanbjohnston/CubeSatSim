#!/bin/bash

sudo modprobe snd-aloop

direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-j14-loop.conf -t 0 
