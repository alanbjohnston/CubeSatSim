#!/bin/bash
# script to run Sim in Fox FSK emulation mode 

# kill rpitx or sendiq if running
ps -ef | grep rpitx | grep -v grep | awk '{print $2}' | sudo xargs kill
ps -ef | grep sendiq | grep -v grep | awk '{print $2}' | sudo xargs kill

echo -e "Script to run CubeSat Simulator in Fox FSK Mode\n"

sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/CubeSatSim/rpitx/rpitx -i- -m RF -f 434.9e3& 

/home/pi/CubeSatSim/radioafsk f $0
