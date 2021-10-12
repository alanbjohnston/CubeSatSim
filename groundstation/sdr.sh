#!/bin/bash
# script to run OpenWebRX SDR

echo "Script to run Web SDR for ARISS Radio Pi"

echo 

echo "The Chromium browser will load in a few seconds with OpenWebRX."

echo "You can also use another web browser if you are on the same network as your Pi."


echo 

ip=$(hostname -I|cut -f1 -d ' ')

echo "IP Address to use in web browser is: $ip:8073"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

pkill -o chromium &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo systemctl stop rtl_tcp

sudo systemctl restart openwebrx

sleep 10

setsid chromium-browser --check-for-update-interval=1 --simulate-critical-update  --noerrdialogs --disable-infobars http://127.0.0.1:8073 &>/dev/null & 

sleep 10

#$SHELL
