#!/bin/bash
#

if [ ! $(systemctl get-default | grep graphical) ]; then

	echo "Graphical User Interface not running!"
	echo 
	echo "Do you want to reboot and switch to GUI so you can run Gpredict?"
	read answer

	if [ "$answer" = "y" ] || [ "$answer" = "yes" ]; then
		echo "Rebooting!"
		sudo raspi-config nonint do_boot_behaviour B4
		sudo reboot now
	else
		echo "You can run Gpredict next time you run the GUI."
		exit
	fi
fi

nohup /usr/bin/gpredict </dev/null >/dev/null 2>&1 &
