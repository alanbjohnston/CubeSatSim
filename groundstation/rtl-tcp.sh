#!/bin/bash
# script to run RTL-TCP Server
#
# On SDR client, use 10.3.141.1:1234 to connect 

echo -e "\nScript to run RTL-TCP Server for ARISS Radio Pi\n"

echo -e "IP Address to use in your SDR application is: "

hostname -I|cut -f1 -d ' '

echo -e "The port is the default 1234. "

./kill_all.sh

sudo /bin/sh -c '/usr/local/bin/rtl_tcp -a $(hostname -I|cut -f1 -d " ")'
