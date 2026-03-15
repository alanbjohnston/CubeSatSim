#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf and run Pacsat Ground Station

loopback=0
vox=0
if [ "$1" = "l" ] ; then

  loopback=1
  echo "PacSat Ground Station with Loopback"  

elif [ "$1" = "v" ] ; then

  vox=1 
  echo "PacSat Ground Station with Soundcard (VOX)"  
  
fi  

FILE=/home/pi/CubeSatSim/battery_saver
if [ -f "$FILE" ]; then

  safe=1
  
fi  

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"
frequency="$7e3"

echo -n "Callsign is "
echo $callsign
echo -n "Transmit Frequency is "
echo $frequency
echo

if [ ! -d "/home/pi/PacSatGround" ] ; then

  mkdir /home/pi/PacSatGround
  
  echo
  echo "The first time you run the Ground Station, you will need to put in your callsign, add the Pacsatsim spacecraft then restart the Pacsat Ground Station"
  
  sleep 10

fi

if [ ! -d "/home/pi/PacSatGroundLoop" ] ; then

  mkdir /home/pi/PacSatGroundLoop
  
  echo
  echo "The first time you run the Ground Station, you will need to put in your callsign, add the Pacsatsim spacecraft then restart the Pacsat Ground Station"
  
  sleep 10

fi

sudo sed -i "s/TNC_TX_DELAY=.*$/TNC_TX_DELAY=750/g" /home/pi/PacSatGround/PacSatGround.properties
sudo sed -i "s/TNC_TX_DELAY=.*$/TNC_TX_DELAY=750/g" /home/pi/PacSatGroundLoop/PacSatGround.properties

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
#  /home/pi/CubeSatSim/groundstation/pacsat-d.sh &

#  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat-loopback.conf -t 0 

    ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1" 
    PTT="PTT GPIOD gpiochip0 17" 

elif [ "$vox" = "1" ]; then

  echo "Using Soundcard Audio TX RX (VOX)"
  /home/pi/CubeSatSim/groundstation/pacsat-dj.sh &

else

  echo "Using TXC FM Transceiver"
  pwm=1
#  /home/pi/CubeSatSim/groundstation/pacsat-df.sh &

  if [ "$pwm" = "1" ] ; then  
  
#    direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-pwm.conf -t 0 &

    echo "FM TXC using Soundcard input (JP13), PWM output"
    ADEVICE="ADEVICE shared_mic plughw:CARD=Headphones,DEV=0" 
    PTT="PTT GPIOD gpiochip0 -20" 
   
  else
  
    direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-fm-pacsat-jp14.conf -t 0 &
    
  fi  

fi

DIREWOLF_CONF="/home/pi/CubeSatSim/groundstation/direwolf-pacsat-tmp.conf"

echo "$ADEVICE" > $DIREWOLF_CONF
echo "MYCALL $callsign-1" >> $DIREWOLF_CONF
echo "$PTT" >> $DIREWOLF_CONF
cat /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat.conf >> $DIREWOLF_CONF

echo
echo "$DIREWOLF_CONF"
echo
cat $DIREWOLF_CONF
echo

direwolf -r 48000 -c $DIREWOLF_CONF -t 0 &

# arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 435045 &
##arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 435045 &

# echo "Don't close the direwolf window or the Pacsatsim will stop running."

cd /home/pi/Desktop/PacsatGround/

if [ "$loopback" = "1" ]; then

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGroundLoop" # removed &

else

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &

fi
#cd /home/pi/Desktop/PacSatGround_0.46m_linux/

#sudo setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" 

cd

#sudo systemctl stop pacsatsim 

sleep 10

#echo "Stopping Pacsatsim"

#$SHELL



