#!/bin/bash
# script to run FUNcube Pi Telemetry App

echo "Script to run FUNcube CubeSatSim Telemetry"

echo 

echo "The Chromium browser will load in a few seconds with fctelem."

echo "You can also use another web browser if you are on the same network as your Pi."


echo 

ip=$(hostname -I|cut -f1 -d ' ')

echo "IP Address to use in web browser is: $ip:8002"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

sudo killall -9 fctelem &>/dev/null

sudo killall -9 python3 &>/dev/null

#sudo killall -9 java &>/dev/null

#sudo killall -9 rtl_fm &>/dev/null

#pkill -o chromium &>/dev/null

#sudo killall -9 rtl_tcp &>/dev/null

#sudo killall -9 CubicSDR &>/dev/null

#sudo killall -9 qsstv &>/dev/null

#sudo killall -9 aplay &>/dev/null

#sudo killall -9 direwolf &>/dev/null

#sudo killall -9 zenity &>/dev/null

#sudo systemctl stop rtl_tcp

#sudo systemctl stop openwebrx


if [[ $(/home/pi/fcdctl/fcdctl -l | grep "No FCD found") ]]; then
    echo "No FUNcube Dongle Found!"
    echo "Plug in FCD and try running again"
    sleep 30
    exit
else
    echo "FCD Found!  Setting Gain"

    FILE=/home/pi/fctelem/fctelem.cfg
	if [ -f "$FILE" ]; then
    
#        config_string=$(</home/pi/fctelem/fctelem.cfg)
        config_string=$(<$FILE)

    else
        echo "No fctelem.cfg file"
        echo "Creating it"
        config_string="-g 1 -m 1 -i 0"
        echo $config_string >> FILE
    fi    
    
    /home/pi/fcdctl/fcdctl $config_string
    
 #   /home/pi/fcdctl/fcdctl -g 1 -m 1 -i 0
    echo
    echo "You can adjust the FCD settings by running the command fcdctl/fcdctl"
    echo
fi


frequency=$(zenity --timeout=10 --list 2>/dev/null --width=410 --height=180 --title="FUNcube Telem Decoding" --text="Choose the frequency for FUNcube decoding:" --column="kHz" --column="Use" 434900 "CubeSatSim" Other "Choose another frequency")

echo $frequency

if [ -z "$frequency" ]; then 

	echo "No choice made."
	frequency=434900

fi

if [ "$frequency" = "434900" ]; then

	frequency=434900000

elif [ "$frequency" = "Other" ]; then

	echo
	
	echo "Enter the frequency in kiloHertz"
	
	echo
	
	read -r frequency
	
	frequency=$frequency"000"

fi

echo "Frequency is" $frequency
echo
echo "If your CubeSatSim is transmitting in FUNcube mode (mode 7) you should get some frames after 30 seconds"
echo

sleep 3

echo "connectport=64516 connectaddress='localhost' autotuneoffset=100000 outdir='/home/pi/go/app/fctelem/data' frequency=$frequency" > /home/pi/fctelem/fcdecode.conf

# FILE=/home/pi/CubeSatSim/groundstation/public_html
FILE=/home/pi/CubeSatSim/fctelem/public_html
if [ ! -d "$FILE" ]; then
    echo "Making public_html directory"
    mkdir /home/pi/fctelem/public_html
    mkdir /home/pi/fctelem/public_html/images
fi

cd /home/pi/fctelem/public_html

cp /home/pi/CubeSatSim/groundstation/index.html .

cp /home/pi/CubeSatSim/sstv/sstv_image_1_320_x_256.jpg ./image_file.jpeg

python3 -m http.server 8002 &

setsid chromium-browser --check-for-update-interval=1 --simulate-critical-update  --noerrdialogs --disable-infobars http://127.0.0.1:8002 &>/dev/null & 

cd /home/pi/fctelem

sudo ./fctelem | python3 /home/pi/CubeSatSim/groundstation/fc_block_decode.py

# sleep 10

#read val

#sudo killall -9 fctelem &>/dev/null

#sudo killall -9 python3 &>/dev/null

sleep 10

#$SHELL
