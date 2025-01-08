#!/bin/bash
# script to run sdrpp

echo "Script to run SDRpp for ARISS Radio Pi"

echo

sudo systemctl stop openwebrx

sudo killall -9 java &>/dev/null

sudo killall -9 sdrpp &>/dev/null

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

#sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

#sudo kill `ps -aux | grep cubicsdr-packet | grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

#sudo kill `ps -aux | grep packet | grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

#sudo kill `ps -aux | grep sstv_decode_prompt| grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo /etc/init.d/alsa-utils stop
sudo /etc/init.d/alsa-utils start

sleep 5

setsid sdrpp

sleep 10

#$SHELL
