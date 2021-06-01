#!/bin/bash

echo -e "\ninstallation script for CubeSatSim\n"

sudo apt-get update && sudo apt-get dist-upgrade -y

sudo apt-get install -y wiringpi git libasound2-dev i2c-tools cpulimit

cd /tmp

wget https://project-downloads.drogon.net/wiringpi-latest.deb

sudo dpkg -i wiringpi-latest.deb


cd

sudo apt install -y python3-pip python-smbus

sudo pip3 install --upgrade setuptools

sudo pip3 install adafruit-blinka RPI.GPIO adafruit-extended-bus adafruit-circuitpython-ina219


cd ~/CubeSatSim

git pull

make debug

FILE=/home/pi/CubeSatSim/.mode
if [ -f "$FILE" ]; then
    echo "$FILE exists."
else 
    echo "creating $FILE"
    echo "ARG1=f" >> .mode
fi

FILE=/home/pi/CubeSatSim/sim.cfg
if [ -f "$FILE" ]; then
    echo "$FILE exists."
else 
    echo "creating $FILE"
    echo $1 >> sim.cfg
fi

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

cd

sudo cp ~/CubeSatSim/systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service

sudo systemctl enable cubesatsim

sudo cp ~/CubeSatSim/systemd/rpitx.service /etc/systemd/system/rpitx.service

sudo systemctl enable rpitx


sudo cp /boot/config.txt /boot/config.txt.0

sudo cp /boot/cmdline.txt /boot/cmdline.txt.0

sudo raspi-config nonint do_i2c 0

#if [ "$1" = "u" ]; then
#fi

  sudo sed -i 's/console=serial0,115200 //g' /boot/cmdline.txt
  
  sudo sed -i 's/#dtparam=i2c_arm=on/dtparam=i2c_arm=on/g' /boot/config.txt
  
  if [[ $(grep 'dtoverlay=i2c-gpio,bus=3,i2c_gpio_delay_us=1,i2c_gpio_sda=23,i2c_gpio_scl=24' /boot/config.txt) ]]; then
    echo "dtoverlay=i2c-gpio already in /boot/config.txt"
  else
    echo "adding dtoverlay=i2c-gpio to /boot/config.txt"
    sudo sh -c 'echo "\ndtoverlay=i2c-gpio,bus=3,i2c_gpio_delay_us=1,i2c_gpio_sda=23,i2c_gpio_scl=24" >> /boot/config.txt'
  fi

  if [[ $(grep 'dtoverlay=pi3-miniuart-bt' /boot/config.txt) ]]; then
    echo "dtoverlay=pi3-miniuart-bt already in /boot/config.txt"
  else
    echo "adding dtoverlay=pi3-miniuart-bt to /boot/config.txt"
    sudo sh -c 'echo "\ndtoverlay=pi3-miniuart-bt" >> /boot/config.txt'
  fi
  
  if [[ $(grep 'dtoverlay=dwc2' /boot/config.txt) ]]; then
    echo "dtoverlay=dwc2 aalready in /boot/config.txt"
  else
    echo "adding dtoverlay=dwc2 to /boot/config.txt"
    sudo sh -c 'echo "\ndtoverlay=dwc2" >> /boot/config.txt'
  fi

  if [[ $(grep 'modules-load=dwc2,g_ether' /boot/cmdline.txt) ]]; then
    echo "modules-load=dwc2,g_ether already in /boot/cmdline.txt"
  else
    echo "adding modules-load=dwc2,g_ether to /boot/cmdline.txt"
    sudo sed -i 's/ rootwait/ rootwait modules-load=dwc2,g_ether/g' /boot/cmdline.txt
  fi
  
echo "You need to reboot to complete the installation\n"

