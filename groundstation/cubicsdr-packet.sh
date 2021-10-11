#!/bin/bash

sudo modprobe snd-aloop

sudo systemctl stop openwebrx

sudo systemctl stop rtl_tcp

pkill -o chromium &>/dev/null

sudo killall -9 rtl_fm &>/dev/null

sudo killall -9 direwolf &>/dev/null

sudo killall -9 aplay &>/dev/null

sudo killall -9 qsstv &>/dev/null

sudo killall -9 rtl_tcp &>/dev/null

sudo killall -9 java &>/dev/null

sudo killall -9 CubicSDR &>/dev/null

#sudo kill `ps -aux | grep "/packet" | grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

#sudo kill `ps -aux | grep sstv_decode_prompt| grep -v grep | awk '{ print $2 }'` &>/dev/null && killall inotifywait &>/dev/null

sudo killall -9 zenity &>/dev/null

#sleep 5

frequency=$(zenity --list --width=510 --height=320 --title="Packet Decoding with CubicSDR and Direwolf" --text="Choose the frequency for packet decoding" --column="kHz" --column="Application" 144390 "APRS US 2m" 434900 "CubeSatSim" 144800 "APRS European 2m" 145175 "APRS Australian 2m" Other "Choose another frequency" 145825 "APRS on ISS" 437100 "Serenity CubeSat 4800 bps" Serenity "Test Serenity CubeSat decoding with WAV file")

#echo $frequency

if [ -z "$frequency" ]; then 

echo "No choice made.  Exiting."

sleep 3

exit

#echo "Choose the number for the frequency for packet decoding:"
#echo
#echo "1. APRS US 2m (144390 kHz)"
#echo "2. CubeSatSim (434900 kHz)"
#echo "3. APRS European 2m (144800 kHz)"
#echo "4. APRS Australian 2m (145175 kHz)"
#echo "5. APRS on another frequency"
#echo "6. APRS on ISS (145825 kHz)"
#echo "7. Serenity CubeSat 4800 bps (437100 kHz)"
#echo "8. Test Serenity CubeSat decoding with WAV file"

#echo

#read -r choice

fi

if [ "$choice" = "1" ] || [ "$frequency" = "144390" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml then Open."
  echo "Note that you won't hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting."

  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won\'t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

#  echo $result

#  [[ ! -z "$result" ]] && read input

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "2" ] || [ "$frequency" = "434900" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."

  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-cubesatsim-direwolf.xml then Open."
  echo "Note that you won't get packets unless your CubeSat Simulator is transmitting in APRS telemetry mode."
  echo "Note that you might need to adjust the frequency in CubicSDR slightly so the signal is centered."
  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-cubesatsim-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won&#39;t get packets unless your CubeSat Simulator is transmitting in APRS telemetry mode.\n\nNote that you might need to adjust the frequency in CubicSDR slightly so the signal is centered.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "3" ] || [ "$frequency" = "144800" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-eu-direwolf.xml then Open."
 
 echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-eu-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0


elif [ "$choice" = "4" ] || [ "$frequency" = "145175" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-au-direwolf.xml then Open."
  
  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-au-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "5" ] || [ "$frequency" = "Other" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml then Open."
  echo "Then tune the CubicSDR frequency to the one you want to decode."

  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-aprs-direwolf.xml</tt> then <b>Open</b>.\n\nThen tune the CubicSDR frequency to the one you want to decode.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "6" ] || [ "$frequency" = "145825" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-iss-direwolf.xml then Open."
  echo "Note that you won't get packets unless the ISS is passing over (see Gpredict) and the ISS radio is in APRS digipeater mode (check social media)."
  echo "Note that you may need to adjust the frequency due to Doppler shift."
  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-iss-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won&#39;t get packets unless the ISS is passing over (see Gpredict) and the ISS radio is in APRS digipeater mode (check social media).\n\nNote that you may need to adjust the frequency due to Doppler shift.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf.conf -r 48000 -t 0

elif [ "$choice" = "7" ] || [ "$frequency" = "437100" ] ; then
  echo

  echo "CubicSDR will load in about 10 seconds."
  
  nohup CubicSDR </dev/null >/dev/null 2>&1 & 

  echo

  echo "When CubicSDR opens, select Generic RTL2832 device then click Start to begin."

  echo

  echo "Then select File/Session/Open Session then load /home/pi/CubeSatSim/groundstation/cubicsdr-session/cubicsdr-serenity-direwolf.xml then Open."
  echo "Note that you will not get packets unless the Serenity CubeSat is passing over (see Gpredict)and is beaconing."

  echo 

#  echo "Hit the Return key when you have done this in CubicSDR and Direwolf will start"

  result=$(zenity --info --width=650 --height=140 --title="Instructions" --text="When CubicSDR opens, select <i>Generic RTL2832U</i> device then click <b>Start</b> to begin.\n\nThen select <b>File/Session/Open Session</b> then load <tt>CubeSatSim/groundstation/cubicsdr-session/cubicsdr-serenity-direwolf.xml</tt> then <b>Open</b>.\n\nNote that you won&#39;t get packets unless the Serenity CubeSat is passing over (see Gpredict)and is beaconing.\n\nNote that you won&#39;t hear anything as the audio is routed to Direwolf for decoding.  To hear the signal, change the Audio Out setting.\n\nThen click <b>OK</b> to start Direwolf!")

  direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-4800.conf -r 48000 -t 0

elif [ "$choice" = "8" ] || [ "$frequency" = "Serenity" ] ; then

  echo

  echo "A recorded WAV file will play and you should see some packets decoded."

  echo 
  

  value=`aplay -l | grep "Loopback"`
  echo "$value" > /dev/null
  set -- $value

#  aplay -D hw:0,0 /home/pi/CubeSatSim/groundstation/WAV/SDRSharp_20210830_200034Z_437097377Hz_AF.wav &
#  aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/groundstation/WAV/SDRSharp_20210830_200034Z_437097377Hz_AF.wav &
  aplay -D hw:0,0 /home/pi/CubeSatSim/groundstation/WAV/beacon_test_2.wav &
  aplay -D hw:${2:0:1},0,0 /home/pi/CubeSatSim/groundstation/WAV/beacon_test_2.wav &

  timeout 30 direwolf -c /home/pi/CubeSatSim/groundstation/direwolf/direwolf-4800.conf -r 48000 -t 0
  
  echo
  
  echo "Test complete.  This window will close in 10 seconds."
  
  sleep 5
  
fi

sleep 5

