#!/bin/bash

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
	echo $callsign
fi

echo -e "Input reset count (integer): "

read resets

if [ -z $resets ] ; then
	resets="$2"
	echo $resets
fi

if ! [[ $resets =~ ^[0-9]+$ ]] ; then
	echo "Error: not an integer!"
	resets="$2"
	echo $resets
fi 

echo -e "Input latitude  (decimal degrees, positive is north): "

read lat 

if [ -z $lat ] ; then

	lat="$3"
	echo $lat
fi

if ! [[ $lat =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] ; then

	echo "Error: not a number!"
	lat="$3"
fi 

echo -e "Input longitude (decimal degrees, positive is east): "

read long 

if [ -z $long ] ; then

	long="$4"
	echo $long
fi

if ! [[ $long =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]] ; then

	echo "Error: not a number!"
	long="$4"
fi 

echo -e "\nCubeSatSim configuraation sim.cfg file updated to: \n"

echo $callsign $resets $lat $long

echo $callsign $resets $lat $long > sim.cfg

