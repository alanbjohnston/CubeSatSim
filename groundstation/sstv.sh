#!/bin/bash
# script to transmit SSTV images

echo

echo "Script to transmit SSTV images from Pi Camera using CubeSatSim Lite"

echo 

echo "Taking picture with Pi Camera to Desktop camera.jpg"

echo 

raspistill -o ~/Desktop/camera.jpg -w 640 -h 496 > /dev/null 2>&1

echo "Converting image to SSTV with pysstv (takes about a minute)"

echo 

python3 -m pysstv --mode PD120 ~/Desktop/camera.jpg sstv_camera.wav > /dev/null 2>&1

echo "Transmitting PD120 SSTV image on 434.9 MHz using rpitx (takes about two minutes to send)"

echo 

sudo systemctl stop rpitx

sudo killall -9 rpitx > /dev/null 2>&1

cat sstv_camera.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1

