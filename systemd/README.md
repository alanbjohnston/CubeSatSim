sudo mv custom.target /etc/systemd/system/custom.target

sudo mv new.service /etc/systemd/system/custom.target.wants/new.service

sudo systemctl list-units --type target --all

sudo systemctl isolate custom.target

sudo ln -sf /etc/systemd/system/custom.target /etc/systemd/system/default.target

sudo reboot now

