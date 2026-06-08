#!/bin/bash

function start-rtl {

  value=`aplay -l | grep "Loopback"`
  echo "$value" > /dev/null
  set -- $value
  
  #rtl_fm -M fm -f 144.39M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1
  #rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -r 48000 -t raw -f S16_LE -c 1
  
#  rtl_fm -g 12 -M fm -f $frequency -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1 &
  rtl_fm -M fm -f $frequency -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1 &

}

# script to auto decode packets using Direwolf and FM TXC and run Pacsat Ground Station

sudo pkill -f "/home/pi/CubeSatSim/groundstation/direwolf-pacsat-tmp.conf"

loopback=0
vox=0
#rpitx=0
#card=0
#pwm=0

if [ "$1" = "l" ] ; then
  loopback=1
elif [ "$1" = "v" ] ; then
  vox=1  
#elif [ "$1" = "c" ] ; then
#  card=1  
#else
#  pwm=1  
fi  

if [[ $(lsusb | grep "RTL") ]] ; then
  echo "RTL-SDR detected"
  rtl=1
else
  echo "No RTL-SDR detected"
  rtl=0
#  pwm=1
fi

if [[ $(arecord -l | grep "USB Audio Device") ]] ; then
  echo "USB Sound Card detected"
  soundcard=1
else
  echo "No USB Sound Card detected"
  soundcard=0
fi  

gpio -g mode 12 up
  if [[ $(gpio -g read 12 | grep 0) ]] ; then
  echo "LPF is present"
  lpf=1
else
  echo "LPF not present"
  lpf=0
fi  

gpio -g mode 7 up
if [[ $(gpio -g read 7 | grep 0) ]] ; then
  echo "TXC is present"
  txc=1
else
  echo "TXC not present"
  txc=0
fi  

if [[ $(gpio -v | grep "Pi 5") ]] ; then
  echo "Since Pi 5, setting TXC to LPF setting"
  txc=$lpf
fi

FILE=/home/pi/CubeSatSim/battery_saver
if [ -f "$FILE" ]; then
  rpitx=1
elif [ "$txc" = "0" ] && [ "$lpf" = "1" ]; then
  rpitx=1
else  
  rpitx=0
fi  


if [ "$soundcard" = "0" ] && [ "$rtl" = "0" ]; then

  echo "Won't work since can't receive with Sound Card or RTL-SDR"
  sleep 10
  exit

fi    

value=`cat /home/pi/CubeSatSim/.mode`
echo "$value" > /dev/null
set -- $value

MODE=$1

if [ ! "$MODE" = "P" ] && [ ! "$loopback" = "1" ] ; then
    echo
    echo "Changing to PacSat Ground Station Mode."
    echo "You might reboot"
    echo
    sleep 5
    /home/pi/CubeSatSim/config -I
fi

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"
txfrequency="$7e3"
rxfrequency="$8e3"
frequency="$8e6"

echo -n "Callsign is "
echo $callsign

if [ "$loopback" = "1" ] ; then

#  if [ ! "$MODE" = "p" ] ; then
    echo
    echo "Switching to PacSat mode for the Simulated PacSat Satellite"
    echo  
    /home/pi/CubeSatSim/config -G n
#  fi  

  if [ ! -d "/home/pi/PacSatGroundLoop" ] ; then
#  if [ ! "$loopback" = "1" ] ; then # don't do this for now.
  
    cd
    sudo rm PacSatGroundLoop.zip &>/dev/null
#    wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b/spacecraft/PacSatGround_0.46o/PacSatGround.zip
    cp /home/pi/CubeSatSim/spacecraft/PacSatGround_0.46o/PacSatGround.zip .
    unzip PacSatGround.zip -d PacSatGroundLoop
    sudo rm PacSatGround.zip

    mkdir PacSatGroundLoop/spacecraft
    mv PacSatGroundLoop/PacSatGround/spacecraft/PacSatSim.properties PacSatGroundLoop/spacecraft/PacSatSim.properties
    mv PacSatGroundLoop/PacSatGround/stp.dat PacSatGroundLoop/stp.dat
    mv PacSatGroundLoop/PacSatGround/seq.dat PacSatGroundLoop/seq.dat
    mv PacSatGroundLoop/PacSatGround/PacSatGround.properties PacSatGroundLoop/PacSatGround.properties

    sudo sed -i 's/logfile_dir=\/home\/pi\/PacSatGround/logfile_dir=\/home\/pi\/PacSatGroundLoop/g' /home/pi/PacSatGroundLoop/PacSatGround.properties

    FILE=/home/pi/Desktop/PacsatGround/spacecraft/PacSatSim.properties
    if [ -f "$FILE" ]; then
      FILE2=/home/pi/PacSatGroundLoop/spacecraft/PacSatSim.properties
      sudo rm $FILE2
      cp $FILE $FILE2
    else       
      echo
      echo "The first time you run the Ground Station, you will need to select Yes to override files, then put in your callsign"
      sleep 10
    fi
  fi
  sudo sed -i "s/TNC_TX_DELAY=.*$/TNC_TX_DELAY=750/g" /home/pi/PacSatGroundLoop/PacSatGround.properties

else

  echo -n "Transmit Frequency is "
  echo $txfrequency
  echo -n "Receive Frequency is "
  echo $rxfrequency
  sleep 2
  
  echo
  echo "Mode is PacSat Ground Station"
  echo

  if [ ! -d "/home/pi/PacSatGround" ] ; then
  
    cd
    sudo rm PacSatGround.zip &>/dev/null
#    wget https://github.com/alanbjohnston/CubeSatSim/raw/refs/heads/master-b/spacecraft/PacSatGround_0.46o/PacSatGround.zip
    cp /home/pi/CubeSatSim/spacecraft/PacSatGround_0.46o/PacSatGround.zip .
    unzip PacSatGround.zip 
    sudo rm PacSatGround.zip

    FILE=/home/pi/Desktop/PacsatGround/spacecraft/PacSatSim.properties
    if [ -f "$FILE" ]; then
      FILE2=/home/pi/PacSatGround/spacecraft/PacSatSim.properties
      sudo rm $FILE2
      cp $FILE $FILE2
    else    
      echo
      echo "The first time you run the Ground Station, you will need to select Yes to override files, then put in your callsign" 
      sleep 10
    fi
  
  fi
  sudo sed -i "s/TNC_TX_DELAY=.*$/TNC_TX_DELAY=750/g" /home/pi/PacSatGround/PacSatGround.properties
fi

sudo modprobe snd-aloop

sudo systemctl stop command >/dev/null 2>&1

sudo systemctl stop command >/dev/null 2>&1

sudo systemctl stop openwebrx >/dev/null 2>&1

sudo systemctl stop rtl_tcp >/dev/null 2>&1

pkill -o chromium &>/dev/null

pkill -o firefox &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 sdrpp &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo usermod -a -G gpio pi


if [ "$loopback" = "1" ] ; then

  echo "1: Using Audio Loopback, No Transmit or Receive"
  ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1" 
  PTT="PTT GPIOD gpiochip0 17" 

else

  if [ "$lpf" = "1" ] ; then

    if [ "$txc" = "1" ] && [ "$soundcard" = "1" ] ; then
      
      if [ "$rpitx" = "1" ] ; then

        echo "2: Using FM for Receive and rpitx for Transmit"
        ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
        PTT="PTT GPIOD gpiochip0 17"
     
      else

        echo "3: Using FM for Receive and Transmit"
        ADEVICE="ADEVICE shared_mic plughw:CARD=Headphones,DEV=0" 
        PTT="PTT GPIOD gpiochip0 -20" 

      fi

    elif [ "$txc" = "1" ] && [ "$soundcard" = "0" ] ; then

      if [ "$rtl" = "1" ]; then
      
        if [ "$rpitx" = "1" ]; then
  
          echo "2b: Using RTL-SDR for Receive and rpitx for Transmit"
          start-rtl
 #         ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
          ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1 plughw:CARD=Loopback,DEV=0" 
          PTT="PTT GPIOD gpiochip0 17" 
       
        else
  
          echo "3b: RTL-SDR for Receive Only"
          start-rtl
          ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1" 
          PTT="#PTT GPIOD gpiochip0 17" 
  
        fi  
        
      else

        echo "Won't work since can't receive with Sound Card or RTL-SDR"
        sleep 10
        exit

      fi
    else

      if [ "$rtl" = "1" ]; then

        echo "4: Using RTL-SDR for Receive and rpitx for Transmit"
        start-rtl
#        ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
        ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1 plughw:CARD=Loopback,DEV=0" 
        PTT="PTT GPIOD gpiochip0 17" 
        
      else

        echo "5: Using Sound Card for Receive and rpitx for Transmit"
        ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
        PTT="PTT GPIOD gpiochip0 17" 
     
      fi
    fi

  else

    if [ "$txc" = "1" ] ; then

      echo "6: Using FM for Receive Only"
      ADEVICE="ADEVICE shared_mic plughw:CARD=Headphones,DEV=0" 
      PTT="#PTT GPIOD gpiochip0 -20" 
  
    else
    
       if [ "$rtl" = "1" ] ; then

        echo "7: Using RTL-SDR for Receive Only"
        start-rtl
        ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=1" 
        PTT="#PTT GPIOD gpiochip0 17" 

       else

        echo "8: Using Sound Card for Receive and Transmit"
        ADEVICE="ADEVICE plughw:CARD=Device,DEV=0" 
        PTT="PTT GPIOD gpiochip0 17" 
        
       fi
    fi
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

direwolf -P+ -D1 -qd -r 48000 -c $DIREWOLF_CONF &

cd /home/pi/Desktop/PacsatGround/

if [ "$loopback" = "1" ] ; then

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGroundLoop" # removed &

else

  setsid java -Xmx512M -jar  PacSatGround.jar "/home/pi/PacSatGround" # removed &

fi

sleep 10

#echo "Stopping Pacsatsim"

#$SHELL



