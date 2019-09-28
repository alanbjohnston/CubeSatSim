#!/bin/bash
# script to run Sim in Fox BPSK emulation mode 

# kill processes if running
ps -ef | grep rpitx | grep -v grep | awk '{print $2}' | sudo xargs kill
ps -ef | grep sendiq | grep -v grep | awk '{print $2}' | sudo xargs kill
sudo fuser -k 8080/tcp
echo -e "Script to run CubeSat Simulator in Fox BPSK Mode\n"

sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/CubeSatSim/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.894e6 -t float 2>&1& 

/home/pi/CubeSatSim/radioafsk b $1
