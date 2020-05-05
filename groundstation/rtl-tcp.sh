#!/bin/bash
# script to run RTL-TCP Server
#
# On SDR client, use 10.3.141.1:1234 to connect 

echo -e "\nScript to run RTL-TCP Server for ARISS Ground Station\n"

echo -e "IP Address to use in web browsers is: "

hostname -I|cut -f1 -d ' '

/home/pi/kill_all.sh

/usr/local/bin/rtl_tcp -a $(hostname -I|cut -f1 -d " ")
