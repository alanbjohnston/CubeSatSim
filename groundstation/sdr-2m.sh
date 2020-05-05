#!/bin/bash
# script to run OpenWebRX SDR

echo -e "\nScript to run SDR for ARISS Ground Station\n"

echo -e "IP Address to use in web browsers is: "

hostname -I|cut -f1 -d ' '


#./kill_all.sh

sudo systemctl stop rtl_sdr

cd ~/openwebrx

chromium-browser http://localhost:8073 &

sudo python openwebrx.py config_webrx_145
