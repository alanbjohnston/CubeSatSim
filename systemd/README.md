cd systemd

sudo cp custom.target /etc/systemd/system/custom.target

sudo mkdir /etc/systemd/system/custom.target.wants

sudo cp cubesatsim.service /etc/systemd/system/custom.target.wants/cubesatsim.service

sudo systemctl list-units --type target --all

sudo systemctl isolate custom.target

sudo ln -sf /etc/systemd/system/custom.target /etc/systemd/system/default.target

sudo reboot now

