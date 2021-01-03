#!/bin/bash

echo -e "\ninstallation script for CubeSatSim\n"

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

git pull

make debug


cd

git clone https://github.com/alanbjohnston/direwolf.git

cd direwolf

make -j

sudo make install

make install-rpi

cd

git clone https://github.com/alanbjohnston/pi-power-button.git

cd pi-power-button

./script/install

cd

git clone https://github.com/F5OEO/rpitx.git

cd rpitx

./install.sh

sudo cp ~/CubeSatSim/systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service

sudo systemctl enable cubesatsim

sudo cp ~/CubeSatSim/systemd/rpitx.service /etc/systemd/system/rpitx.service

sudo systemctl enable rpitx

if [ "$1" = "u" ]; then

  sed -i 's/console=serial0,115200//g' /boot/cmdline.txt
  
  sudo echo "\ndtoverlay=i2c-gpio,bus=3,i2c_gpio_delay_us=1,i2c_gpio_sda=23,i2c_gpio_scl=24\ndtoverlay=pi3-miniuart-bt" >> /boot/config.txt

fi

echo "You need to reboot to complete the installation\n"

