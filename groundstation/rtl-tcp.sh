#!/bin/bash
# script to run RTL-TCP Server
#
# On SDR client, use 10.3.141.1:1234 to connect 

echo "Script to run RTL-TCP Server for ARISS Radio Pi"

ip=$(hostname -I|cut -f1 -d ' ')

echo

echo "IP Address to use in SDR application is: $ip"

echo

echo "Port to use in SDR application is the default: 1234"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

sudo systemctl stop openwebrx

sleep 2

pkill -o chromium &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 zenity &>/dev/null


sudo /bin/sh -c '/usr/local/bin/rtl_tcp -a $(hostname -I|cut -f1 -d " ")'

sleep 5

#$SHELL
