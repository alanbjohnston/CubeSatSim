#!/bin/sh

sudo killall -9 CubicSDR &>/dev/null
sudo killall -9 direwolf &>/dev/null

sleep 5

CubicSDR &>/dev/null &

sleep 15

xdotool mousemove 290 348 click 1
sleep 1.9 
xdotool mousemove 449 572 click 1
sleep 1.9
xdotool mousemove 20 77 click 1
sleep 1.9
xdotool mousemove 59 208 
sleep 1.9
xdotool mousemove 325 208 click 1
sleep 1.9
xdotool mousemove 290 487 click 1
sleep 1.9
xdotool type "cubicsdr-aprs-direwolf.xml"
sleep 5
xdotool mousemove 938 568 click 1
sleep 5

direwolf -c direwolf.conf -r 48000 -t 0

sleep 5



