#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf and run Pacsat Ground Station

loopback=0
if [ "$1" = "l" ] ; then

  loopback=1
  
fi  

if [ ! -d "/home/pi/PacSatGround" ] ; then

  mkdir /home/pi/PacSatGround
  
  echo
  echo "You will need to install the Pacsatsim spacecraft file and set the Delay to 750ms and Port to 8100 and restart the Pacsat Ground Station"
  
  sleep 10

fi

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"

oldcallsign=$(grep -oP '(?<=callsign=).*(?=-)' /home/pi/PacSatGround/PacSatGround.properties)

if [ ! "$callsign" = "$oldcallsign" ] 

  sudo sed -i "s/callsign=$oldcallsign/callsign=$callsign/g" /home/pi/PacSatGround/PacSatGround.properties
  
fi

sudo modprobe snd-aloop

#sudo systemctl stop cubesatsim >/dev/null 2>&1

#sudo systemctl stop transmit >/dev/null 2>&1

sudo systemctl stop command >/dev/null 2>&1

#/home/pi/CubeSatSim/config -I

sudo systemctl stop command >/dev/null 2>&1

sudo systemctl stop openwebrx >/dev/null 2>&1

sudo systemctl stop rtl_tcp >/dev/null 2>&1

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

#sudo killall -9 direwolf &>/dev/null

sudo killall -9 sdrpp &>/dev/null

#sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 zenity &>/dev/null

echo

#sudo systemctl restart pacsatsim

#sudo /etc/init.d/alsa-utils stop
#sudo /etc/init.d/alsa-utils start


#echo "Waiting 10 seconds for Pacsatsim to start"

#sleep 10

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

echo 
echo "The Pacsat Ground Station is running on this Pi using FM receiver or RTL-SDR"
echo

#cd /home/pi/Desktop/PacSatGround_0.46m_linux/

#setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &

#direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat-loopback.conf -t 0  # &
#/usr/bin/x-terminal-emulator --geometry=120x40 -e "direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat-loopback.conf -t 0"

sudo usermod -a -G gpio pi

if [ "$loopback" = "1" ]; then
#/usr/bin/x-terminal-emulator --geometry=120x40 -e "/home/pi/CubeSatSim/groundstation/pacsat-df.sh"

  echo "Using Audio Loopback"
  /home/pi/CubeSatSim/groundstation/pacsat-d.sh &

else

  echo "Using TXC FM Transceiver"
  /home/pi/CubeSatSim/groundstation/pacsat-df.sh &

fi

# arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 435045 &
##arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 435045 &

# echo "Don't close the direwolf window or the Pacsatsim will stop running."

cd /home/pi/Desktop/PacsatGround/

setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &


#cd /home/pi/Desktop/PacSatGround_0.46m_linux/

#sudo setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" 

cd

#sudo systemctl stop pacsatsim 

sleep 10

#echo "Stopping Pacsatsim"

#$SHELL



