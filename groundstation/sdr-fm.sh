#!/bin/bash
# script to run OpenWebRX SDR

echo -e "\nScript to run SDR for ARISS Ground Station\n"

echo -e "IP Address to use in web browsers is: "

hostname -I|cut -f1 -d ' '


./kill_all.sh

cd ~/openwebrx

sudo python openwebrx.py config_webrx_107
