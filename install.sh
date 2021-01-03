#!/bin/bash

echo -e "\ninstallation script for CubeSatSim\n"

#if [ "$1" = "c" ]; then
#fi

sudo apt-get update && sudo apt-get dist-upgrade -y

sudo apt-get install -y wiringpi git libasound2-dev i2c-tools cpulimit

cd /tmp

wget https://project-downloads.drogon.net/wiringpi-latest.deb

sudo dpkg -i wiringpi-latest.deb

cd

sudo cp /boot/config.txt /boot/config.txt.0

sudo cp /boot/cmdline.txt /boot/cmdline.txt.0

sudo apt install -y python3-pip python-smbus

sudo pip3 install --upgrade setuptools

sudo pip3 install adafruit-blinka RPI.GPIO adafruit-extended-bus adafruit-circuitpython-ina219

cd ~/CubeSatSim

make 

cd

git clone https://github.com/alanbjohnston/direwolf.git

cd direwolf

make -j

sudo make install

echo "You may get an error in this step - don't worry\n"

make install-rpi

cd

git clone https://github.com/F5OEO/rpitx.git

cd rpitx

./install.sh

cd

git clone https://github.com/alanbjohnston/pi-power-button.git

cd pi-power-button

./script/install

sudo cp ~/CubeSatSim/systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service

sudo systemctl enable cubesatsim

sudo cp ~/CubeSatSim/systemd/rpitx.service /etc/systemd/system/rpitx.service

sudo systemctl enable rpitx


