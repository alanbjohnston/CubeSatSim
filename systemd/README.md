To make the CubeSat Simulator transmit automatically on boot, follow these steps, which were derived from https://www.raspberrypi.org/documentation/linux/usage/systemd.md


On the Pi, if you are not already in this directory:

cd systemd

sudo cp ~/CubeSatSim/cubesatsim.service /etc/systemd/system/cubesatsim.service

sudo systemctl service enable cubesatsim

Now after a reboot (sudo reboot now), the demo.sh script will run automatically, and the console will log to ~/CubeSatSim/log.txt  To see it:

tail ~/CubeSatSim/log.txt
