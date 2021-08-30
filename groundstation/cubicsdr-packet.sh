#!/bin/bash

sudo modprobe snd-aloop

sudo systemctl stop openwebrx

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

#sleep 5

echo

echo "Choose the number for the type of decoding:"
echo
echo "1. APRS US 2m (144390 kHz)"
echo "2. CubeSatSim (434900 kHz)"
echo "3. APRS European 2m (144800 kHz)"
echo "4. APRS Australian 2m (145175 kHz)"
echo "5. APRS on another frequency"
echo "6. APRS on ISS (145825 kHz)"
echo "7. Serenity CubeSat 4800 bps (437100 kHz)"
echo "8. Test Serenity CubeSat decoding with WAV file"

echo

read -r choice


if [ "$choice" = "1" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml then Open."
  echo "Note that you won't hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "2" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-cubesatsim-direwolf.xml then Open."
  echo "Note that you won't get packets unless your CubeSat Simulator is transmitting in APRS digipeater mode."
  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "3" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-eu-direwolf.xml then Open."
 
 echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0


elif [ "$choice" = "4" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-au-direwolf.xml then Open."
  
  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "5" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml then Open."
  echo "Then tune the CubicSDR frequency to the one you want to decode."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "6" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-iss-direwolf.xml then Open."
  echo "Note that you won't get packets unless the ISS is passing over and the radio is in APRS digipeater mode."
  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "7" ]; then
  echo

  echo "CubicSDR will load in about 10 sceonds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-serenity-direwolf.xml then Open."
  echo "Note that you won't get packets unless the Serenity CubeSat is passing over and is beaconing."

  echo 

  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  read input

  direwolf -c direwolf/direwolf-4800.conf -r 48000 -t 0

elif [ "$choice" = "8" ]; then

  echo

  echo "A recorded WAV file will play and you should see some packets decoded"

  echo 
  

  value=`aplay -l | grep "Loopback"`
  echo "$value" > /dev/null
  set -- $value

#  aplay -D hw:0,0 WAV/SDRSharp_20210830_200034Z_437097377Hz_AF.wav &
#  aplay -D hw:${2:0:1},0,0 WAV/SDRSharp_20210830_200034Z_437097377Hz_AF.wav &
  aplay -D hw:0,0 WAV/beacon_test_2.wav &
  aplay -D hw:${2:0:1},0,0 WAV/beacon_test_2.wav &

  timeout 30 direwolf -c direwolf/direwolf-4800.conf -r 48000 -t 0
  
  echo
  
  echo "Test complete.  This window will close in 10 seconds."
  
  sleep 5
  
fi

sleep 5

