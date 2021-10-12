#!/bin/bash

echo "Script to decode SSTV using QSSTV with rtl_fm"

echo

sudo systemctl stop openwebrx

sudo modprobe snd-aloop

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 zenity &>/dev/null

#echo "s" >> .mode

frequency=$(zenity --list 2>/dev/null --width=410 --height=220 --title="SSTV Decoding using QSSTV" --text="Choose the frequency for SSTV decoding:" --column="kHz" --column="Use" 145800 "ISS" 434900 "CubeSatSim" Other "Choose another frequency" SSTV "Test SSTV decoding with WAV file")

echo $frequency

if [ -z "$frequency" ]; then 

echo "No choice made.  Exiting."

sleep 3

exit

#echo "Choose the number for the frequency for SSTV decoding:"
#echo "1. ISS (145800Hz)"
#echo "2. CubeSatSim (434900 kHz)"
#echo "3. Enter a frequency in kiloHertz"
#echo "4. Play an SSTV WAV file to test"
#echo

#read -r choice

fi

if [ "$choice" = "1" ] || [ "$frequency" = "145800" ]; then

frequency=145800000
echo "Frequency is" $frequency
echo
echo "If the ISS is overhead (check Gpredict) and in SSTV mode (see social media), you can receive images."

elif [ "$choice" = "2" ] || [ "$frequency" = "434900" ]; then

frequency=434900000
echo "Frequency is" $frequency
echo
echo "If your CubeSatSim is transmitting in SSTV mode (mode 5) you should get images."
echo "Note: if you see and hear an SSTV signal but don't get any images, the CubeSatSim signal might have a frequency offset.  Try rebooting the CubeSatSim to fix."

elif [ "$choice" = "3" ] || [ "$frequency" = "Other" ]; then

echo

echo "Enter the frequency in kiloHertz"

echo

read -r frequency

frequency=$frequency"000"

elif [ "$choice" = "4" ] || [ "$frequency" = "SSTV" ]; then

  echo "A recorded Scottie 2 SSTV WAV file will play and you should see an image decoded."

  echo 
 
  value=`aplay -l | grep "Loopback"`
  echo "$value" > /dev/null
  set -- $value

  #aplay -D hw:0,0 /home/pi/CubeSatSim/telem.wav &
  (while true; do (sleep 5 && aplay -D hw:0,0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav); done) &
  
  #aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/telem.wav &
  (while true; do (sleep 5 && aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/sstv_image_1_320_x_256.jpg.wav); done) &
  
  qsstv 
  
fi


if [ "$choice" != "4" ] && [ "$frequency" != "SSTV" ]; then

#echo $frequency

echo

echo "Note that the 'Tuned to' frequency will be different from the chosen frequency due to the way SDRs work."

echo

echo -e "Auto decoding SSTV on $frequency Hz"

sleep 2

qsstv &

sleep 5

#sudo systemctl restart cubesatsim


value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f 434.9M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1 
#rtl_fm -M fm -f 434.9M -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1
rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1

sleep 5

fi

#$SHELL
