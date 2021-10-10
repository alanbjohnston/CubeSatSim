#!/bin/bash
# script to run OpenWebRX SDR

echo "Script to run CubicSDR for ARISS Radio Pi"

echo 

echo "Note: Select Generic RTL2832 device then click Start to begin"

echo

sudo systemctl stop openwebrx

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo systemctl stop rtl_tcp

sudo killall -9 rtl_tcp &>/dev/null

#sudo kill `ps -aux | grep cubicsdr-packet | grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

#sudo kill `ps -aux | grep packet | grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

#sudo kill `ps -aux | grep sstv_decode_prompt| grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

sudo killall -9 zenity &>/dev/null

sleep 5

setsid CubicSDR

sleep 10

#$SHELL
