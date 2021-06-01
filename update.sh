#!/bin/bash

echo -e "\nupdate script for CubeSatSim\n"

if [ "$1" = "u" ]; then

  sudo apt-get update && sudo apt-get dist-upgrade -y

  sudo apt-get install -y wiringpi git libasound2-dev i2c-tools cpulimit

fi

cd /home/pi/CubeSatSim

git pull > .updated

make debug

FLAG=0

if [[ $(grep 'cubesatsim.service' /home/pi/CubeSatSim/.updated) ]]; then
  echo "copying cubesatsim.service"
  sudo cp systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service
  FLAG=1
else
  echo "no changes to cubesatsim.service"
fi

if [[ $(grep 'rpitx.service' /home/pi/CubeSatSim/.updated) ]]; then
  echo "copying rpitx.service"
  sudo cp systemd/rpitx.service /etc/systemd/system/rpitx.service
  FLAG=1
else
  echo "no changes to rpitx.service"
fi

if [ $FLAG -eq 1 ]; then
  echo "systemctl daemon-reload"
  sudo systemctl daemon-reload 
else
  echo "systemctl restart cubesatsim"
  sudo systemctl restart cubesatsim
fi
