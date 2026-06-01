#!/bin/bash

# script to send APRS or DTMR commands to a CubeSatSim

sudo killall -9 zenity &>/dev/null

echo

autotune=0

mode=$(zenity --timeout=10 --list 2>/dev/null --width=410 --height=400 --title="Transmit Command" --text="Choose the command to transmit" --column="Command" --column="Result" a "APRS Mode" f "FSK Mode" b "BPSK Mode" s "SSTV Mode" m "CM Mode" e "Repeater Mode" j "FUNcube Mode" o "Beacon On/Off")

echo $mode

if [ -z "$mode" ]; then 

echo "No choice made."

fi

if [ "mode" = "a" ]; then

  echo "CHange to APRS mode"

fi
  
