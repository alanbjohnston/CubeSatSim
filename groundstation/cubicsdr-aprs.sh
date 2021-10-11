#!/bin/sh

sudo modprobe snd-aloop

#if [ ! -d "./direwolf.conf" ]; then

FILE=direwolf.conf
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/direwolf.conf 

fi

FILE=cubicsdr-aprs-direwolf.xml
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/cubicsdr/cubicsdr-aprs-direwolf.xml 

fi

sudo killall -9 CubicSDR &>/dev/null
sudo killall -9 direwolf &>/dev/null

sleep 5

echo

echo "Opening CubicSDR... (this will take a few seconds)"

nohup CubicSDR </dev/null >/dev/null 2>&1 & 

echo

echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

echo

echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr/cubicsdr-aprs-direwolf.xml then Open."

echo 

echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

read input

direwolf -c direwolf.conf -r 48000 -t 0

sleep 5



