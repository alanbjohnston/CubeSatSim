#!/bin/bash
# script to auto decode CubeSat Simulator telemetry

echo -e "Script to auto decode CubeSat Simulator telemetry\n"

sudo rtl_fm -f 440.386M -s 22050 -g 48 - | multimon-ng -a AFSK1200 -A -t raw -
