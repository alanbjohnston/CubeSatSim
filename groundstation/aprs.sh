#!/bin/bash
# script to auto decode APRS packets on 2m

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

echo

echo "Choose the number for the frequency for APRS decoding:"
echo
echo "1. APRS US 2m (144390 kHz)"
echo "2. CubeSatSim (434900 kHz)"
echo "3. APRS European 2m (144800 kHz)"
echo "4. APRS Australian 2m (145175 kHz)"
echo "5. Enter a frequency in kiloHertz"
echo

read -r choice

if [ "$choice" = "1" ]; then

frequency=144390000

elif [ "$choice" = "2" ]; then

frequency=434900000

elif [ "$choice" = "3" ]; then

frequency=144800000

elif [ "$choice" = "4" ]; then

frequency=145175000

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

echo -e "Auto decoding APRS packets on $frequency Hz"

#sudo rtl_fm -f 144.39M -s 22050 -g 48 - | multimon-ng -a AFSK1200 -A -t raw -

direwolf -r 48000 -t 0 &

sleep 5

value=`aplay -l | grep "Loopback"`
echo "$value" > /dev/null
set -- $value

#rtl_fm -M fm -f 144.39M -s 48k | aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1
rtl_fm -M fm -f $frequency -s 48k | tee >(aplay -D hw:${2:0:1},0,0 -r 48000 -t raw -f S16_LE -c 1) | aplay -D hw:0,0 -r 48000 -t raw -f S16_LE -c 1

sleep 5
