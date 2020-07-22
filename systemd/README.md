To make the CubeSat Simulator transmit automatically on boot, follow these steps, which were derived from https://www.raspberrypi.org/documentation/linux/usage/systemd.md


Log into the Pi and type:

sudo cp ~/CubeSatSim/systemd/cubesatsim.service /etc/systemd/system/cubesatsim.service

sudo systemctl enable cubesatsim

sudo cp ~/CubeSatSim/systemd/rpitx.service /etc/systemd/system/rpitx.service

sudo systemctl enable rpitx

Reboot to start the autoboot service:

sudo reboot now

Now after the Pi starts up, the demo.sh script will run automatically, to see the console:

sudo journalctl -u cubesatsim

