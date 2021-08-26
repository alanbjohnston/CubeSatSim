#!/bin/sh

sudo modprobe snd-aloop

#if [ ! -d "./direwolf.conf" ]; then

FILE=direwolf.conf
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/direwolf.conf 

fi

FILE=direwolf-4800.conf
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/direwolf-4800.conf 

fi

FILE=cubicsdr-aprs-direwolf.xml
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/cubicsdr/cubicsdr-aprs-direwolf.xml 

fi

FILE=cubicsdr-iss-direwolf.xml
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/cubicsdr/cubicsdr-iss-direwolf.xml 

fi

FILE=cubicsdr-serenity-direwolf.xml
if [ ! -f "$FILE" ]; then

  wget https://raw.githubusercontent.com/alanbjohnston/CubeSatSim/FIAB-v3/groundstation/cubicsdr/cubicsdr-serenity-direwolf.xml 

fi

sudo killall -9 CubicSDR &>/dev/null
sudo killall -9 direwolf &>/dev/null

sleep 5

echo

echo "Choose the number for the type of decoding:"
echo
echo "1. APRS US 2m (144.390 MHz)"
echo "2. ISS APRS (145.825 MHz"
echo "3. APRS on some other frequency"
echo "4. Serenity CubeSat 4800 bps (437.1 MHz)"

echo

read -r choice


echo

echo "Opening CubicSDR... (this will take a few seconds)"

nohup CubicSDR </dev/null >/dev/null 2>&1 & 

echo

echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

echo

if [ "$choice" = "1" ]; then

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr/cubicsdr-aprs-direwolf.xml then Open."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf.conf -r 48000 -t 0

elif [ "$choice" = "2" ]; then

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr/cubicsdr-iss-direwolf.xml then Open."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf.conf -r 48000 -t 0

elif [ "$choice" = "3" ]; then

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr/cubicsdr-aprs-direwolf.xml then Open."
  echo "Then tune the CubicSDR frequency to the one you want to decode."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf.conf -r 48000 -t 0

elif [ "$choice" = "4" ]; then

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr/cubicsdr-serenity-direwolf.xml then Open."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf-4800.conf -r 48000 -t 0

fi

sleep 5


