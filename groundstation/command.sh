#!/bin/bash

# script to send APRS or DTMR commands to a CubeSatSim

value=`cat /home/pi/CubeSatSim/.mode`
echo "$value" > /dev/null
set -- $value

if [ "$1" != "n" ]; then

  echo "You are not in Transmit Commands mode"
  sleep 10
  exit

fi

sudo killall -9 zenity &>/dev/null

echo

autotune=0

mode=$(zenity --timeout=20 --list 2>/dev/null --width=300 --height=360 --title="Transmit Command" --text="Choose the command to transmit" --column="Mode" --column="Result" a "APRS Mode" f "FSK Mode" b "BPSK Mode" s "SSTV Mode" m "CM Mode" e "Repeater Mode" j "FUNcube Mode" p "PacSatSim Mode" o "Beacon On/Off")

# echo $mode

if [ -z "$mode" ]; then 

  echo "No choice made."
  sleep 10
  
  exit

fi

/home/pi/CubeSatSim/config -$mode
sleep 10



  
