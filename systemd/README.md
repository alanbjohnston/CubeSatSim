To make the CubeSat Simulator transmit automatically on boot, follow these steps, which were derived from https://www.mauras.ch/systemd-run-it-last.html

On the Pi, if you are not already in this directory:

cd ~/CubeSatSim/systemd

sudo cp custom.target /etc/systemd/system/custom.target

sudo mkdir /etc/systemd/system/custom.target.wants

sudo cp cubesatsim.service /etc/systemd/system/custom.target.wants/cubesatsim.service

sudo systemctl isolate custom.target

sudo ln -sf /etc/systemd/system/custom.target /etc/systemd/system/default.target

Now after a reboot (sudo reboot now), the demo.sh script will run automatically, and the console will log to ~/CubeSatSim/log.txt  To see it:

tail ~/CubeSatSim/log.txt
