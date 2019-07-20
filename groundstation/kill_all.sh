#!/bin/bash
# script to kill all SDR or RTL processes
#

echo -e "\nKilling all SDR or RTL processes \n"

# kill rtl if running
ps -ef | grep rtl_ | grep -v grep | awk '{print $2}' | sudo xargs kill > /dev/null 2>&1 

# kill openwebrx process if running
ps -ef | grep openwebrx | grep -v grep | awk '{print $2}' | sudo xargs kill > /dev/null 2>&1

# kill csdr process if running
ps -ef | grep csdr | grep -v grep | awk '{print $2}' | sudo xargs kill > /dev/null 2>&1

# kill gqrx process if running
ps -ef | grep gqrx-sdr-2.11.5-linux-rpi3/gqrx | grep -v grep | awk '{print $2}' | sudo xargs kill > /dev/null 2>&1


