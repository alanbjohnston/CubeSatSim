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

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

#echo "s" >> .mode

echo

echo "Choose the number for the frequency for SSTV decoding:"
echo "1. ISS (145800Hz)"
echo "2. CubeSatSim (434900 kHz)"
echo "3. Enter a frequency in kiloHertz"
echo

read -r choice

if [ "$choice" = "1" ]; then

frequency=145800000
echo "Frequency is " $frequency
echo
echo "If the ISS is overhead (check Gpredict) and in SSTV mode (see social media), you can receive images."
echo

elif [ "$choice" = "2" ]; then

frequency=434900000
echo
echo "If your CubeSatSim is transmitting in SSTV mode (mode 6) you should get images."
echo "Note: if you see and hear an SSTV signal but don't get any images, the CubeSatSim signal might have a frequency offset.  Try rebooting the CubeSatSim to fix."
echo

else

echo

echo "Enter the frequency in kiloHertz"

echo

read -r frequency

frequency=$frequency"000"

fi

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

#$SHELL
