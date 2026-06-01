#!/bin/bash

# script to send APRS or DTMR commands to a CubeSatSim

value=`cat /home/pi/CubeSatSim/.mode`
echo "$value" > /dev/null
set -- $value

if [ "$1" != "n" ]; then

  echo "Do you want to change to Transmit Commands mode?"

  exit

fi

sudo killall -9 zenity &>/dev/null

echo

autotune=0

mode=$(zenity --timeout=10 --list 2>/dev/null --width=410 --height=400 --title="Transmit Command" --text="Choose the command to transmit" --column="Command" --column="Result" a "APRS Mode" f "FSK Mode" b "BPSK Mode" s "SSTV Mode" m "CM Mode" e "Repeater Mode" j "FUNcube Mode" o "Beacon On/Off")

echo $mode

if [ -z "$mode" ]; then 

echo "No choice made."

fi

if [ "$mode" = "a" ]; then

  echo "Change to APRS mode"

  /home/pi/CubeSatSim/config -a

fi

  
