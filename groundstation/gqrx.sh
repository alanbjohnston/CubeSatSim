#!/bin/bash
# script to run gqrx 

echo "Script to run Gqrx for ARISS Radio Pi"

echo

sudo killall -9 java &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 rtl_tcp &>/dev/null

sudo systemctl stop openwebrx

/usr/bin/gqrx
