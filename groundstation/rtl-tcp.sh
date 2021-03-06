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

pkill -o chromium &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo systemctl stop openwebrx

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo systemctl start rtl_tcp

$SHELL
