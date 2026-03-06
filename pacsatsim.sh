#!/bin/bash

# script to run PacsatSim

loopback=0
vox=0
if [ "$1" = "l" ] ; then

  loopback=1

elif [ "$1" = "v" ] ; then

  vox=1  
  
fi  

if [ ! -d "/home/pi/PacSat" ]; then

  echo "Setting up PacSatSim default configuration"
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

fi

value=`cat /home/pi/CubeSatSim/sim.cfg`
echo "$value" > /dev/null
set -- $value

callsign="$1"

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



if [ "$loopback" = "1" ]; then

  echo "Using audio loopback"
  sudo /home/pi/CubeSatSim/pacsatsim-d.sh &

elif [ "$vox" = "1" ]; then

  echo "Using Soundcard Audio TX and RX (VOX)"
  sudo /home/pi/CubeSatSim/pacsatsim-dj.sh &

else

  echo "Using TXC FM transceiver"
  sudo /home/pi/CubeSatSim/pacsatsim-df.sh &

fi

#  arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 14000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434900 &
##  arecord -D plughw:CARD=Loopback,DEV=1 -f S16_LE -r 48000 -c 1 | csdr convert_s16_f | csdr gain_ff 4000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434900 &

##  echo "Don't close the direwolf window or the Pacsatsim will stop running."


sudo /home/pi/pacsat_telem/Debug/pacsat_telem -v -d /home/pi/PacSat/pacsat &

sleep 5

value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1 &

cd /home/pi/pi_pacsat/Debug

./pi_pacsat -c pacsat.config -d /home/pi/PacSat

sleep 5
