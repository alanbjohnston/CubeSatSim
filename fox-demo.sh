(while true; do 

#  ps -ef | grep rpitx | grep -v grep | awk '{print $2}' | sudo xargs kill
#  ps -ef | grep sendiq | grep -v grep | awk '{print $2}' | sudo xargs kill
#  sudo nc -l 8080 | csdr convert_i16_f | csdr gain_ff 7000 | csdr convert_f_samplerf 20833 | sudo /home/pi/CubeSatSim/rpitx/rpitx -i- -m RF -f 434.9e3& 
#  /home/pi/CubeSatSim/radioafsk f 3

  /home/pi/CubeSatSim/fox-fsk.sh $1

#  ps -ef | grep rpitx | grep -v grep | awk '{print $2}' | sudo xargs kill
#  ps -ef | grep sendiq | grep -v grep | awk '{print $2}' | sudo xargs kill
#  sudo nc -l 8080 | csdr convert_i16_f | csdr fir_interpolate_cc 2 | csdr dsb_fc | csdr bandpass_fir_fft_cc 0.002 0.06 0.01 | csdr fastagc_ff | sudo /home/pi/CubeSatSim/rpitx/sendiq -i /dev/stdin -s 96000 -f 434.9e6 -t float 2>&1&
#  /home/pi/CubeSatSim/radioafsk b 3
 
  /home/pi/CubeSatSim/fox-bpsk.sh $1
  
done)

