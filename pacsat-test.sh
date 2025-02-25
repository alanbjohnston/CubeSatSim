#!/bin/bash

/usr/bin/x-terminal-emulator --geometry=120x40 -e "bash /home/pi/CubeSatSim/pacsatsim-d.sh"

sleep 1

/usr/bin/x-terminal-emulator --geometry=120x40 -e "bash /home/pi/CubeSatSim/pacsatsim.sh"

sleep 1

/usr/bin/x-terminal-emulator --geometry=120x40 -e "direwolf -P+ -D1 -qd -dp -r 48000 -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-pacsat-fm-loopback.conf -t 0"

sleep 1

/usr/bin/x-terminal-emulator --geometry=120x40 -e "bash /home/pi/CubeSatSim/groundstation/pacsat.sh"


