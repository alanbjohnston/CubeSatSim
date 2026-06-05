#!/bin/bash

# script to run PacsatSim

loopback=0
vox=0
safe=0
card=0
pwm=0
reset=0

if [ "$1" = "l" ] ; then

  loopback=1

elif [ "$1" = "v" ] ; then

  vox=1  

elif [ "$1" = "c" ] ; then

  card=1  

elif [ "$1" = "r" ] ; then

  reset=1  

else

  pwm=1  
  
fi  

FILE=/home/pi/CubeSatSim/battery_saver
if [ -f "$FILE" ]; then

  safe=1
  
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

#timeout 1 rtl_test &> out.txt
#if [[ $(grep "No supported" out.txt) ]] ; then
#  echo "No RTL-SDR detected"
#  rtl=0
#else
#  echo "RTL-SDR detected."
#  rtl=1
#fi

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"
txfrequency="$7e3"
rxfrequency="$8e3"

echo -n "PacSat callsign is "
echo $callsign
echo -n "Transmit Frequency is "
echo $txfrequency
echo -n "Receive Frequency is "
echo $rxfrequency
echo

sleep 2

if [ "$reset" = "0" ] ; then

  if [ ! -d "/home/pi/PacSat" ]; then
  
    echo "Setting up PacSatSim default configuration"
    echo
  
    cd
    sudo rm pacsat-dir.zip
    mkdir /home/pi/PacSat
    mkdir /home/pi/PacSat/pacsat
    mkdir /home/pi/PacSat/pacsat/dir
  #  wget https://github.com/alanbjohnston/pi_pacsat/releases/download/v0.2/pacsat-dir.zip
    cp /home/pi/pi_pacsat/zip/pacsat-dir.zip .
    unzip pacsat-dir.zip 
    sudo rm pacsat-dir.zip
  
    cd
    sudo rm /home/pi/pi_pacsat/Debug/pacsat.config
    sudo rm /home/pi/pi_pacsat/Debug/pacsat.state
    sudo rm /home/pi/pi_pacsat/Debug/pacsat_upload_table.dat
    sudo rm pi_pacsat.zip
  #  wget https://github.com/alanbjohnston/pi_pacsat/releases/download/v0.2/pi_pacsat.zip
    cp /home/pi/pi_pacsat/zip/pi_pacsat.zip .
    unzip pi_pacsat.zip 
    sudo rm pi_pacsat.zip
  
  fi

else

  echo "Resetting pi_pacsat to empty configuration"
  echo
  mkdir /home/pi/PacSat
  mkdir /home/pi/PacSat/pacsat
  mkdir /home/pi/PacSat/pacsat/dir
  
  cd /home/pi/pi_pacsat/Debug
  
  sudo rm pacsat_last_command_time.dat
  sudo rm pacsat_upload_table.dat
  sudo rm pacsat.state
  
  echo "bit_rate=9600" > pacsat.config
  echo "bbs_callsign=AMSAT-12" >> pacsat.config
  echo "broadcast_callsign=AMSAT-11" >> pacsat.config
  echo "digi_callsign=AMSAT-1" >> pacsat.config
  echo "max_frames_in_tx_buffer=5" >> pacsat.config
  
  echo "pb_open=1" > pacsat.state
  echo "uplink_open=1" >> pacsat.state
  echo "pb_max_period_for_client_in_seconds=60" >> pacsat.state
  echo "uplink_max_period_for_client_in_seconds=60" >> pacsat.state
  
  touch /home/pi/pi_pacsat/Debug/pacsat_upload_table.dat

  exit
fi

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"
frequency="$7e3"

echo "Configured callsign is "
echo $callsign

oldcallsign=$(grep -oP '(?<=bbs_callsign=).*(?=-)' /home/pi/pi_pacsat/Debug/pacsat.config)

echo "Callsign in pacsat.config is "
echo $oldcallsign

if [ ! "$callsign" = "$oldcallsign" ] ; then

  sudo sed -i "s/bbs_callsign=$oldcallsign/bbs_callsign=$callsign/g" /home/pi/pi_pacsat/Debug/pacsat.config
  sudo sed -i "s/broadcast_callsign=$oldcallsign/broadcast_callsign=$callsign/g" /home/pi/pi_pacsat/Debug/pacsat.config
  sudo sed -i "s/digi_callsign=$oldcallsign/digi_callsign=$callsign/g" /home/pi/pi_pacsat/Debug/pacsat.config

  echo "New pacsat.confg is"
  echo

  cat /home/pi/pi_pacsat/Debug/pacsat.config
  
fi

sudo /etc/init.d/alsa-utils stop
sudo /etc/init.d/alsa-utils start

# export LD_LIBRARY_PATH=/mnt/usb-disk/ariss/lib:/usr/local/lib/iors_common:$LD_LIBRARY_PATH

export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

#sudo systemctl stop cubesatsim

#sudo systemctl stop transmit

#sudo systemctl stop command &>/dev/null

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

sudo killall -9 pacsat_telem &>/dev/null

sudo killall -9 pi_pacsat &>/dev/null

echo


#choice=2

#fi


#  frequency=434900000
  echo
  echo "If your Pacsat Ground Station is transmitting packets, you will see them here"
  echo


#echo

#echo "Note that the 'Tuned to' frequency will be different from the chosen frequency due to the way SDRs work."


#  echo -e "Auto decoding APRS Pacsat packets on $frequency Hz"

#  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-loopback.conf -t 0 &

 # /usr/bin/x-terminal-emulator --geometry=120x40 -e "/home/pi/CubeSatSim/pacsatsim-df.sh"

sudo usermod -a -G gpio pi

if [ "$loopback" = "1" ] ; then

  echo "Using audio loopback"
  ADEVICE="ADEVICE plughw:CARD=Loopback,DEV=0"
  PTT="PTT GPIOD gpiochip0 17"

  value=`cat /home/pi/CubeSatSim/.mode`
  echo "$value" > /dev/null
  set -- $value
  
  MODE=$1
  
  if [ ! "$MODE" = "P" ] ; then

    /home/pi/CubeSatSim/config -I n

 fi   
 
#  sudo /home/pi/CubeSatSim/pacsatsim-d.sh &

#  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-loopback.conf -t 0 &

elif [ "$safe" = "1" ] ; then

  echo "Safe mode - battery saver"
#  sudo /home/pi/CubeSatSim/pacsatsim-d.sh &

  ADEVICE="ADEVICE shared_mic plughw:CARD=Loopback,DEV=0"
  PTT="PTT GPIOD gpiochip0 17"

#  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-pwm-loopback.conf -t 0 &

#  arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434900 &
#  arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f $frequency &


elif [ "$vox" = "1" ]; then

  echo "Using Soundcard Audio TX and RX (VOX, no PTT)"
  ADEVICE="ADEVICE plughw:CARD=Device,DEV=0" 
  PTT="PTT GPIOD gpiochip0 17" 
#  sudo /home/pi/CubeSatSim/pacsatsim-dj.sh &

#  direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-jp14-half.conf -t 0 &

elif [ "$pwm" = "1" ] ; then  
  
    echo "FM TXC using Soundcard input (JP13), PWM output"
    ADEVICE="ADEVICE shared_mic plughw:CARD=Headphones,DEV=0" 
    PTT="PTT GPIOD gpiochip0 -20" 

#    direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-pwm.conf -t 0 &

else

    echo "FM TXC using Soundcard input (JP13) and output (JP14)"
    ADEVICE="ADEVICE shared_mic plughw:CARD=Device,DEV=0" 
    PTT="PTT GPIOD gpiochip0 -20" 
#    direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim-jp14.conf -t 0 &

fi

DIREWOLF_CONF="/home/pi/CubeSatSim/direwolf-pacsatsim-tmp.conf"

echo "$ADEVICE" > $DIREWOLF_CONF
echo "MYCALL $callsign-1" >> $DIREWOLF_CONF
echo "$PTT" >> $DIREWOLF_CONF
cat /home/pi/CubeSatSim/direwolf/direwolf-pacsatsim.conf >> $DIREWOLF_CONF

echo
echo "$DIREWOLF_CONF"
echo
cat $DIREWOLF_CONF
echo

#direwolf -P+ -D1 -qd -dp -r 48000 -c $DIREWOLF_CONF -t 0 &
direwolf -qd -r 48000 -c $DIREWOLF_CONF &

#  arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434900 &
## arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434900 &

##  echo "Don't close the direwolf window or the Pacsatsim will stop running."

export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

/home/pi/pacsat_telem/Debug/pacsat_telem -v -d /home/pi/PacSat/pacsat &

sleep 5

value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1 &

cd /home/pi/pi_pacsat/Debug

./pi_pacsat -c pacsat.config -d /home/pi/PacSat

sleep 60
