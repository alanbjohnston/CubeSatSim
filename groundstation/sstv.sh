#!/bin/bash
# script to transmit SSTV images

echo

echo "Script to transmit SSTV images from  ARISS Radio Pi using CubeSatSim Lite"

echo 

sudo systemctl stop rpitx

echo "Taking picture with Raspicam to Desktop raspicam.jpg"

echo 

raspistill -o ~/Desktop/raspicam.jpg -w 640 -h 496 

echo "Converting image to SSTV with pysstv (takes about 3 minutes)"

echo 

python3 -m pysstv --mode PD120 ~/Desktop/raspicam.jpg output.wav > /dev/null 2>&1

echo "Transmitting PD120 SSTV image on 434.9 MHz using rpitx"

echo 

sudo killall -9 rpitx > /dev/null 2>&1

cat output.wav | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo rpitx -i- -m RF -f 434.9e3 > /dev/null 2>&1

