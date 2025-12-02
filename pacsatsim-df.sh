#!/bin/bash

pwm=1

sudo modprobe snd-aloop

if [ "$pwm" = "1" ] ; then  

  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-pwm.conf -t 0 

else

  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-jp14.conf -t 0 

fi
