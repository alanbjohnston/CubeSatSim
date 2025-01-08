#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf and run Pacsat

sudo modprobe snd-aloop

sudo systemctl stop cubesatsim >/dev/null 2>&1

sudo systemctl stop transmit >/dev/null 2>&1

sudo systemctl stop command >/dev/null 2>&1

#/home/pi/CubeSatSim/config -I

sudo systemctl stop command >/dev/null 2>&1

sudo systemctl stop openwebrx >/dev/null 2>&1

sudo systemctl stop rtl_tcp >/dev/null 2>&1

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

#sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 zenity &>/dev/null

echo

sudo systemctl restart pacsatsim

sudo /etc/init.d/alsa-utils stop
sudo /etc/init.d/alsa-utils start


echo "Waiting 10 seconds for Pacsatsim to start"

sleep 10

#value=`cat /home/pi/CubeSatSim/sim.cfg`
#echo "$value" > /dev/null
#set -- $value

#echo "Receive frequency is $8 MHz"	
#echo "Transmit frequency is $7 MHz"	
#echo 
#echo "To change, quit and type CubeSatSim/config -F"
#echo

#frequency="$8e6"

#echo "Note that the 'Tuned to' frequency will be different from the chosen frequency due to the way SDRs work."

#echo

direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat-loopback.conf -t 0  &

echo 
echo "The Pacsat and Pacsat Ground Station are running on this Pi using audio loopback."
echo


cd /home/pi/Desktop/PacSatGround_0.46m_linux/

sudo setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" 

cd

#sudo systemctl stop pacsatsim 

sleep 10

#echo "Stopping Pacsatsim"

$SHELL



