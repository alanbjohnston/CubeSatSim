#!/bin/bash
# script to run RTL-TCP in Direct Sampling Mode 

echo "Script to run RTL-TCP Direct Sampling for ARISS Radio Pi"

echo 

ip=$(hostname -I|cut -f1 -d ' ')

echo "IP Address to use in SDR is: $ip:1234"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

sudo killall -9 java &>/dev/null

sudo killall -9 gqrx &>/dev/null

sudo systemctl stop rtl_tcp

sudo systemctl stop openwebrx

sudo /bin/sh -c '/usr/local/bin/rtl_tcp -a $(hostname -I|cut -f1 -d " ") -D 2'
