#!/bin/bash
# script to run OpenWebRX SDR

echo -e "\nScript to run SDR for CubeSat Simulator\n"

# kill openwebrx if running
ps -ef | grep rtl | grep -v grep | awk '{print $2}' | sudo xargs kill

echo -e "Connect to CubeSatSim WiFi and open browser to http://192.168.8.245:8073\n"

cd ../openwebrx

sudo python openwebrx.py
