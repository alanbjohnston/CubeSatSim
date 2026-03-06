#!/bin/bash

pwm=1

sudo modprobe snd-aloop

#gpio -g mode 20 out

#gpio -g write 20 1

#direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-jp14.conf -t 0 

if [ "$pwm" = "1" ] ; then  

  direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-pwm.conf -t 0 
 
else

  direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-jp14.conf -t 0 
  
fi
