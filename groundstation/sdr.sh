#!/bin/bash
# script to run OpenWebRX SDR

echo "Script to run Web SDR for ARISS Radio Pi"

echo 

ip=$(hostname -I|cut -f1 -d ' ')

echo "IP Address to use in web browsers is: $ip:8073"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

sudo killall -9 java &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 rtl_tcp &>/dev/null

sudo systemctl stop openwebrx

#sudo cp /etc/openwebrx/config_webrx_vhf.py /etc/openwebrx/config_webrx.py

sudo systemctl restart openwebrx

/usr/bin/chromium-browser --noerrdialogs --disable-infobars http://localhost:8073 &>/dev/null & 

$SHELL
