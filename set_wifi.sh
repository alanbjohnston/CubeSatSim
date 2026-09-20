#!/bin/bash

if [ -z "$1" ] ; then
	echo "include the WiFi ssid and password"
	exit
elif [ -z "$2" ] ; then
	echo "include password"
	exit
fi

echo "If this works, this web page won't work again until you connect to the new WiFi network!" 

echo 

sudo nmcli device wifi rescan 

sleep 1 

sudo nmcli device wifi connect "$1" password "$2" 

sudo reboot now 

