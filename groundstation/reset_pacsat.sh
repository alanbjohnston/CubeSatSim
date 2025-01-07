#!/bin/bash

# script to clear Pacsat Ground Station and Pacsatsim state

echo "Clearing all Pacsat Ground Station and Pacsatsim state"

echo

sudo systemctl stop pacsatsim 

#sudo killall -9 direwolf &>/dev/null

sudo killall -9 java &>/dev/null

cd

cd pi_pacsat

cd Debug

sudo rm -r /home/pi/PacSat

mkdir /home/pi/PacSat
mkdir /home/pi/PacSat/pacsat
mkdir /home/pi/PacSat/pacsat/dir

#value=`cat /home/pi/CubeSatSim/sim.cfg`
#echo "$value" > /dev/null
#set -- $value

echo "bit_rate=9600" > pacsat.config
echo "bbs_callsign=AMSAT-12" >> pacsat.config
echo "broadcast_callsign=AMSAT-11" >> pacsat.config
echo "digi_callsign=AMSAT-1" >> pacsat.config
echo "max_frames_in_tx_buffer=5" >> pacsat.config

sudo rm pacsat_last_command_time.dat
sudo rm pacsat_upload_table.dat
sudo rm pacsat.state

echo "pb_open=1" > pacsat.state
echo "uplink_open=1" >> pacsat.state
echo "pb_max_period_for_client_in_seconds=60" >> pacsat.state
echo "uplink_max_period_for_client_in_seconds=60" >> pacsat.state

touch /home/pi/pi_pacsat/Debug/pacsat_upload_table.dat

sudo rm -r /home/pi/PacSatGround

mkdir /home/pi/PacSatGround

echo

echo "You will need to install the Pacsatsim spacecraft file and set the Delay to 750ms and Port to 8100 and restart the Pacsat Ground Station"

