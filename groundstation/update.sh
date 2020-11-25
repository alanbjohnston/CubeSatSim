#!/bin/bash
#

echo -e "\nUpdating configurations. \n"

cd ~/CubeSatSim && git pull

if [ "$1" = "make" ]; then
  make debug
fi

sudo cp ~/CubeSatSim/groundstation/config_webrx.py /etc/openwebrx/config_webrx.py

sudo cp ~/CubeSatSim/groundstation/bookmarks.json /etc/openwebrx/bookmarks.json

sudo cp ~/CubeSatSim/groundstation/readme.txt ~/readme.txt

sudo cp ~/CubeSatSim/groundstation/*.desktop ~/Desktop

