#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf and run Pacsat

sudo modprobe snd-aloop

/home/pi/CubeSatSim/config -I

sudo systemctl stop command &>/dev/null

sudo systemctl stop openwebrx &>/dev/null

sudo systemctl stop rtl_tcp &>/dev/null

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 zenity &>/dev/null

echo

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

echo "Receive frequency is $8 MHz"	
echo "Transmit frequency is $7 MHz"	
echo 
echo "To change, quit and type CubeSatSim/config -F"
echo

frequency="$8e6"

#echo "Note that the 'Tuned to' frequency will be different from the chosen frequency due to the way SDRs work."

#echo

if [[ $(arecord -l | grep "USB Audio Device") ]] ; then

	echo -e "Direwolf using USB Sound Card (FM Transceiver) on $frequency Hz"
	
	direwolf -P+ -D1 -qd -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-jp14.conf -t 0 &

else

	echo -e "Direwolf using RTL-SDR on $frequency Hz"
	
	direwolf -P+ -D3 -r 48000 -qd -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat.conf -t 0 &
	
	sleep 5
	
	value=`aplay -l | grep "Loopback"`
	echo "$value" > /dev/null
	set -- $value
	
	#rtl_fm -M fm -f 144.39M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1
	rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1 &

fi

cd /home/pi/Desktop/PacSatGround_0.46m_linux/

sudo setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" 



