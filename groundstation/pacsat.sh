#!/bin/bash

# script to auto decode packets using Direwolf and FM TXC and run Pacsat Ground Station

sudo pkill -f "/home/pi/CubeSatSim/groundstation/direwolf-pacsat-tmp.conf"

loopback=0
vox=0
safe=0
card=0
pwm=0

if [ "$1" = "l" ] ; then

  loopback=1

elif [ "$1" = "v" ] ; then

  vox=1  

elif [ "$1" = "c" ] ; then

  card=1  

else

  pwm=1  
  
fi  

if [[ $(arecord -l | grep "USB Audio Device") ]] ; then
  echo "USB Sound Card detected"
  soundcard=1
else
  echo "No USB Sound Card detected"
  soundcard=0
fi  

gpio -g mode 7 up
if [[ $(gpio -g read 7 | grep 0) ]] ; then
  echo "TXC is present"
  txc=1
else
  echo "TXC not present"
  txc=0
fi  

timeout 1 rtl_test &> out.txt
if [[ $(grep "No supported" out.txt) ]] ; then
  echo "No RTL-SDR detected"
  rtl=0
else
  echo "RTL-SDR detected."
  rtl=1
fi

FILE=/home/pi/CubeSatSim/battery_saver
if [ -f "$FILE" ]; then

  safe=1
  
fi  

value=`cat /home/pi/CubeSatSim/.mode`
echo "$value" > /dev/null
set -- $value

MODE=$1

if [ ! "$MODE" = "P" ] && [ ! "$loopback" = "1" ] ; then

  echo
  echo "Mode is not PacSat Ground Station"
  echo
  echo "Would you like to change to PacSat Ground Station mode and reboot? (y/n)?"
  
  read -r ANS
  
  if [ "$ANS" = "y" ]; then
  
      /home/pi/CubeSatSim/config -I
      exit
      
  else

      echo
      echo "You can run the PacSat Ground Station after you change mode and reboot."
      sleep 10
      exit
      
  fi

elif [ "$loopback" = "1" ] ; then

  if [ ! "$MODE" = "p" ] ; then
    echo
    echo "Switching to PacSat mode for the Simulated PacSat Satellite"
    echo  
    /home/pi/CubeSatSim/config -G n
  fi  

  if [ ! -d "/home/pi/PacSatGroundLoop" ] ; then
#  if [ ! "$loopback" = "1" ] ; then # don't do this for now.
  
    cd
    sudo rm PacSatGroundLoop.zip &>/dev/null
    wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b-p/spacecraft/PacSatGround_0.46o/PacSatGround.zip
    unzip PacSatGroundLoop.zip -d PacSatGroundLoop
    sudo rm PacSatGroundLoop.zip
    
    echo
    echo "The first time you run the Ground Station, you will need to select Yes to override files, then put in your callsign"
    sleep 10
  
  fi

else

  echo
  echo "Mode is PacSat Ground Station"
  echo

  if [ ! -d "/home/pi/PacSatGround" ] ; then
  
    cd
    sudo rm PacSatGround.zip &>/dev/null
    wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b-p/spacecraft/PacSatGround_0.46o/PacSatGround.zip
    unzip PacSatGround.zip -d PacSatGround
    sudo rm PacSatGround.zip
          
    echo
    echo "The first time you run the Ground Station, you will need to select Yes to override files, then put in your callsign" 
    sleep 10
  
  fi

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

#sudo systemctl restart pacsatsim

#sudo /etc/init.d/alsa-utils stop
#sudo /etc/init.d/alsa-utils start

sudo usermod -a -G gpio pi

if [ "$loopback" = "1" ] ; then

  echo "Using Audio Loopback"
  ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1" 
  PTT="PTT GPIOD gpiochip0 17" 

elif [ "$safe" = "1" ] ; then

  ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
  PTT="PTT GPIOD gpiochip0 17"

  if [ ! "$txc" = "1" ] ; then
    echo "Safe mode - battery saver won't work since no TXC present"
    sleep 5
  elif [ ! "$soundcard" = "1" ] ; then
     echo "Safe mode - battery saver won't work since no sound card present"
     sleep 5
  else
    echo "Safe mode - battery saver"
  fi  

elif [ "$vox" = "1" ] ; then

  ADEVICE="ADEVICE plughw:CARD=Device,DEV=0" 
  PTT="PTT GPIOD gpiochip0 17" 
  
  if [ "$soundcard" = "1" ] ; then
     echo "Using Soundcard Audio TX and RX (VOX, no PTT)"
  else
    echo "Soundcard Audio TX and RX (VOX, no PTT) will not work since no sound card present"
    sleep 5
  fi  
  
elif [ "$pwm" = "1" ] ; then  
  
  ADEVICE="ADEVICE shared_mic plughw:CARD=Headphones,DEV=0" 
  PTT="PTT GPIOD gpiochip0 -20" 

  if [ ! "$txc" = "1" ] ; then
    echo "FM TXC using Soundcard input (JP13), PWM output won't work since no TXC present"
    sleep 5
  elif [ ! "$soundcard" = "1" ] ; then
     echo "FM TXC using Soundcard input (JP13), PWM output won't work since no sound card present"
     sleep 5
  else
    echo "FM TXC using Soundcard input (JP13), PWM output"
  fi
   
else
  
  echo "FM TXC using Soundcard input (JP13) and output (JP14)"
  ADEVICE="ADEVICE shared_mic plughw:CARD=Device,DEV=0" 
  PTT="PTT GPIOD gpiochip0 -20"

  if [ ! "$txc" = "1" ] ; then
    echo "FM TXC using Soundcard input (JP13) and output (JP14) won't work since no TXC present"
    sleep 5
  elif [ ! "$soundcard" = "1" ] ; then
     echo "FM TXC using Soundcard input (JP13), output (JP14) won't work since no sound card present"
     sleep 5
  else
    echo "FM TXC using Soundcard input (JP13), output JP14"
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

cd /home/pi/Desktop/PacsatGround/

if [ "$loopback" = "1" ] ; then

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGroundLoop" # removed &

else

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &

fi

sleep 10

#echo "Stopping Pacsatsim"

#$SHELL



