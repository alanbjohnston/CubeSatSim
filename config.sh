#!/bin/bash

if [ "$1" = "r" ]; then
	echo "Restarting CubeSatSim"
	sudo systemctl restart cubesatsim
	exit 
elif [ "$1" = "a" ]; then
	echo "changing CubeSatSim to AFSK mode"
	sudo echo "ARG1=a" >> .mode	
	sudo systemctl restart cubesatsim
	exit 
elif [ "$1" = "f" ]; then
	echo "changing CubeSatSim to FSK mode"
	sudo echo "ARG1=f" >> .mode	
	sudo systemctl restart cubesatsim
	exit 
elif [ "$1" = "b" ]; then
	echo "changing CubeSatSim to BPSK mode"
	sudo echo "ARG1=b" >> .mode	
	sudo systemctl restart cubesatsim
	exit 
elif [ "$1" = "s" ]; then
	echo "changing CubeSatSim to SSTV mode"
	sudo echo "ARG1=s" >> .mode	
	sudo systemctl restart cubesatsim
	exit 
fi

echo -e "\nConfiguration script for CubeSatSim\n"
 
echo -e "Return keeps current value."

echo -e "Current sim.cfg configuration file: \n"

value=`cat sim.cfg`

echo "$value"

echo

set -- $value

#echo $1 $2 $3 $4

echo -e "Input callsign (all capitals): "

read callsign

if [ -z $callsign ] ; then

	callsign="$1"
	echo "Keeping value of" $callsign
fi

echo -e "Input reset count (integer): "

read resets

if [ -z $resets ] ; then
	resets="$2"
	echo "Keeping value of" $resets
fi

if ! [[ $resets =~ ^[0-9]+$ ]] ; then
	echo "Error: not an integer!"
	resets="$2"
	echo "Keeping value of" $resets
fi 

echo -e "Input latitude  (decimal degrees, positive is north): "

read lat 

if [ -z $lat ] ; then

	lat="$3"
	echo "Keeping value of" $lat
fi

if ! [[ $lat =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] ; then

	echo "Error: not a number!"
	lat="$3"
	echo "Keeping value of" $lat
fi 

echo -e "Input longitude (decimal degrees, positive is east): "

read long 

if [ -z $long ] ; then

	long="$4"
	echo "Keeping value of" $long
fi

if ! [[ $long =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] ; then

	echo "Error: not a number!"
	long="$4"
	echo "Keeping value of" $long
fi 

echo -e "\nCubeSatSim configuraation sim.cfg file updated to: \n"

echo $callsign $resets $lat $long

echo $callsign $resets $lat $long > sim.cfg
 
echo "Restarting CubeSatSim with new configuraation file"

sudo systemctl restart cubesatsim

