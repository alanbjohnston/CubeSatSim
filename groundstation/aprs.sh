#!/bin/bash
# script to auto decode APRS packets on 2m

# kill openwebrx if it is running
ps -ef | grep rtl | grep -v grep | awk '{print $2}' | sudo xargs kill

echo -e "Script to auto decode APRS packets on 144.390 MHz\n"

sudo rtl_fm -f 144.39M -s 22050 -g 48 - | multimon-ng -a AFSK1200 -A -t raw -
