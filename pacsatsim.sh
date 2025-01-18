#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf and run Pacsat

export LD_LIBRARY_PATH=/mnt/usb-disk/ariss/lib:/usr/local/lib/iors_common:$LD_LIBRARY_PATH

#sudo systemctl stop cubesatsim

#sudo systemctl stop transmit

sudo systemctl stop command &>/dev/null

sudo modprobe snd-aloop

#sudo systemctl stop openwebrx

#sudo systemctl stop rtl_tcp &>/dev/null

#pkill -o chromium &>/dev/null

#sudo killall -9 rtl_fm &>/dev/null

#sudo killall -9 direwolf &>/dev/null

#udo killall -9 aplay &>/dev/null

#sudo killall -9 qsstv &>/dev/null

#sudo killall -9 rtl_tcp &>/dev/null

#sudo killall -9 java &>/dev/null

#sudo killall -9 CubicSDR &>/dev/null

#sudo killall -9 zenity &>/dev/null

echo

value=`cat /home/pi/CubeSatSim/pacsat.cfg`
echo "$value" > /dev/null
set -- $value

if [ "$1" =  "l" ]; then
    echo
#    sudo systemctl restart pacsatsim
#    echo 
    echo "The Pacsat and Pacsat Ground Station are running on this Pi using audio loopback"
    echo
#    echo "Waiting 10 seconds for Pacsatsim to start"
#    sleep 10
    CONFIG=direwolf-pacsatsim-loopback.conf
    
elif [ "$1" = "c" ]; then
    echo "Pacsat from USB Card Device"
    CONFIG=direwolf-pacsatsim-soundcard.conf
    
else
    echo "Pacsat from Radio through USB Sound Card Device"
    CONFIG=direwolf-pacsatsim.conf
fi

 echo
  echo "If your Pacsat Ground Station is transmitting packets, you will see them here"
  echo

  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/$CONFIG -t 0 &


sleep 5

#value=`aplay -l | grep "Loopback"`
#echo "$value" > /dev/null
#set -- $value

#rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1 &

cd /home/pi/pi_pacsat/Debug

./pi_pacsat -c pacsat.config -d /home/pi/PacSat

sleep 5
