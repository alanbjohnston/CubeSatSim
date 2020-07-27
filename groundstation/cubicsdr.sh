#!/bin/bash
# script to run OpenWebRX SDR

echo "Script to run CubicSDR for ARISS Radio Pi"

echo 

echo "Note: Select Generic RTL2832 device then click Start to begin"

echo

sudo killall -9 java &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 rtl_tcp &>/dev/null

sudo systemctl stop openwebrx

CubicSDR

$SHELL
