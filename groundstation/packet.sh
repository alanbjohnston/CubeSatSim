#!/bin/bash

# script to auto decode packet using rtl_fm and Direwolf

sudo modprobe snd-aloop

sudo killall -9 CubicSDR &>/dev/null

sudo systemctl stop openwebrx

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 sdrpp &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

#sudo killall -9 fctelem &>/dev/null

sudo /etc/init.d/alsa-utils stop
sudo /etc/init.d/alsa-utils start

echo

autotune=0

frequency=$(zenity --timeout=10 --list 2>/dev/null --width=410 --height=400 --title="Packet Decoding with Direwolf" --text="Choose the frequency for packet decoding" --column="kHz" --column="Application" 144390 "APRS US 2m" 434900 "CubeSatSim" Auto-tune "CubeSatSim Auto-tune" 144800 "APRS European 2m" 145175 "APRS Australian 2m" Other "Choose another frequency" 145825 "APRS on ISS" APRS "Test APRS decoding with CubeSatSim WAV file")

#echo $frequency

if [ -z "$frequency" ]; then 

echo "No choice made."

frequency=434900

#echo "Choose the number for the packet decoding option:"
#echo
#echo "1. APRS US 2m (144390 kHz)"
#echo "2. CubeSatSim (434900 kHz)"
#echo "3. APRS European 2m (144800 kHz)"
#echo "4. APRS Australian 2m (145175 kHz)"
#echo "5. APRS on another frequency"
#echo "6. APRS on ISS (145825 kHz)"
#echo "7. Serenity CubeSat 4800 bps (437.1 MHz)"
#echo "8. Test Serenity CubeSat decoding with WAV file"
#echo "9. Test APRS decoding with CubeSatSim WAV file"
#echo

#read -r choice

fi

if [ "$choice" = "1" ] || [ "$frequency" = "144390" ]; then

  frequency=144390000

elif [ "$choice" = "2" ] || [ "$frequency" = "434900" ] ; then

  frequency=434900000
  echo
  echo "If your CubeSatSim is transmitting in APRS mode (mode 1) then you should see packets."
  echo

elif [ "$frequency" = "Auto-tune" ] ; then

  frequency=434900000
  autotune=1

elif [ "$choice" = "3" ] || [ "$frequency" = "144800" ]; then

  frequency=144800000

elif [ "$choice" = "4" ] || [ "$frequency" = "145175" ]; then

  frequency=145175000

elif [ "$choice" = "5" ] || [ "$frequency" = "Other" ] ; then

  echo

  echo "Enter the frequency in kiloHertz"

  echo

  read -r frequency

  frequency=$frequency"000"

  #echo $frequency

elif [ "$choice" = "6" ]  || [ "$frequency" = "145825" ] ; then

  frequency=145825000
  echo
  echo "If the ISS is overhead and in APRS mode (see tracking application such as Gpredict), you will see packets."
  echo

elif [ "$choice" = "8" ] || [ "$frequency" = "APRS" ] ; then

  echo "A recorded APRS WAV file from the CubeSatSim will play and you should see a packet decoded."

  echo 
 
  value=`aplay -l | grep "Loopback"`
  echo "$value" > /dev/null
  set -- $value

  #aplay -D hw:0,0 /home/pi/CubeSatSim/telem.wav &
  (while true; do (sleep 2 && aplay -D hw:0,0 /home/pi/CubeSatSim/telem.wav &>/dev/null); done) &
  
  #aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/telem.wav &
  (while true; do (sleep 2 && aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/telem.wav &>/dev/null); done) &
  
  timeout 30 direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0
  
  echo
  
  echo "Test complete.  This window will close in 10 seconds."
  
  sleep 5
  
  exit

#fi

fi

echo

echo "Note that the 'Tuned to' frequency will be different from the chosen frequency due to the way SDRs work."

echo

#if [ "$choice" = "7" ] || [ "$choice" = "8" ]  || [ "$frequency" = "Serenity" ]  || [ "$frequency" = "437100000" ] ; then

#  echo -e "Auto decoding 4800 bps AX.25 packets on $frequency Hz"

#  direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-4800.conf -t 0 &

#else

  echo -e "Auto decoding APRS packets on $frequency Hz"

 # direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -t 0 &
  setsid direwolf -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf  &

#fi

#sleep 5

if [ "$autotune" = "1" ]; then
  threshold=1
  delay=1
  retries=30
  
  tries=0
  confidence=0
  while [ $tries -le $retries ] && [ "$confidence" -le "$threshold" ]; do

#    sleep $delay
    source /home/pi/venv/bin/activate
    python3 /home/pi/CubeSatSim/groundstation/auto-tune.py 434900000 n 2> null > /home/pi/CubeSatSim/groundstation/auto-tune.txt
    # echo "auto-tune.txt"
    # cat /home/pi/CubeSatSim/groundstation/auto-tune.txt
    confidence=$(awk '{print $2}' /home/pi/CubeSatSim/groundstation/auto-tune.txt)
    echo "Auto tune confidence: " $confidence

  done
  
  if [ "$confidence" -gt "$threshold" ]; then
    frequency=$(awk '{print $1}' /home/pi/CubeSatSim/groundstation/auto-tune.txt)
    echo "Auto tune frequency: " $frequency
  else
    echo "Auto tune failed, frequency unchanged"
  fi
  echo
  echo "If your CubeSatSim is transmitting in APRS mode (mode 1) then you should see packets."
  echo

fi

value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f 144.39M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1
rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -r 48000 -t raw -f S16_LE -c 1

rtl_fm -M fm -f $frequency -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1


sleep 5
