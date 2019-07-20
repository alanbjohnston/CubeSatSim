#!/bin/bash
# script to auto decode CubeSat Simulator telemetry

# kill rtl if running
ps -ef | grep rtl | grep -v grep | awk '{print $2}' | sudo xargs kill

# kill openwebrx process if running
ps -ef | grep openwebrx | grep -v grep | awk '{print $2}' | sudo xargs kill

# kill csdr process if running
ps -ef | grep csdr | grep -v grep | awk '{print $2}' | sudo xargs kill

echo -e "Script to run Gqrx\n"

/home/pi/gqrx-sdr-2.11.5-linux-rpi3/gqrx

