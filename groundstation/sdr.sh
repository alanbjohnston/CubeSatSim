#!/bin/bash
# script to run OpenWebRX SDR

echo "Script to run Web SDR for ARISS Radio Pi"

echo 

echo "The browser will load in a few seconds with OpenWebRX."

echo "You can also use another web browser if you are on the same network as your Pi."


echo 

ip=$(hostname -I|cut -f1 -d ' ')

echo "IP Address to use in web browser is: $ip:8073"

echo

ssid=$(iwgetid -r)

echo "Note: you need to be on the Wifi network: $ssid"

echo

if [[ $(gpio -v | grep "Pi 4") ]] && [[ ! $(sudo raspi-config nonint get_browser | grep "chromium") ]] ; then
	echo "Since Pi 4, changing default browser to Chromium"
	sudo raspi-config nonint do_browser chromium
fi

if [[ $(gpio -v | grep "Pi 5") ]]&& [[ ! $(sudo raspi-config nonint get_browser | grep "firefox") ]] ; then
	echo "Since Pi 5, changing default browser to Firefox"
	sudo raspi-config nonint do_browser firefox
fi

sudo killall -9 sdrpp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

pkill -o chromium &>/dev/null

pkill -o firefox &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

sudo killall -9 sdrpp &>/dev/null

sudo killall -9 qsstv &>/dev/null

#sudo killall -9 aplay &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 zenity &>/dev/null

sudo systemctl stop rtl_tcp &>/dev/null

sudo /etc/init.d/alsa-utils stop
sudo /etc/init.d/alsa-utils start

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo systemctl restart openwebrx

sleep 10

#setsid chromium-browser --password-store=basic --check-for-update-interval=1 --simulate-critical-update  --noerrdialogs --disable-infobars --app=http://localhost:8073 &>/dev/null & 
setsid x-www-browser http://localhost:8073/

sleep 10

#$SHELL
