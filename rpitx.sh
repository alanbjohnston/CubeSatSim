#!/bin/bash

echo -e "\nrpitx for CubeSatSim at 434.9 MHz\n"

#python rpitx.py $1

	if [ "$1" = "a" ]; then
    		echo "Mode is continuous AFSK"
    		sleep infinity 
	elif [ "$1" = "b" ]; then
   		echo "Tx is continuous BPSK"
		sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.8945e6 -t float
	else 
    		echo "Tx is continuous FSK"
		sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/rpitx/rpitx -i- -m RF -f 434.897e3
	fi
