#!/bin/bash
exit 0

echo -e "\nDemo of CubeSatSim sends AFSK telemetry at 440 MHz continuously\n\n"

sleep 60

#echo 'sleep over' >> /home/pi/CubeSatSim/log.txt

echo $(date '+%Y %b %d %H:%M') Starting Hostname $HOSTNAME  >> /home/pi/CubeSatSim/log.txt

/home/pi/CubeSatSim/radioafsk >> /home/pi/CubeSatSim/log.txt 
#/home/pi/DigitalTxRxRPi/testafsktx  >> /home/pi/CubeSatSim/log.txt

#echo $(date '+%Y %b %d %H:%M') Stopping Hostname $HOSTNAME  >> /home/pi/CubeSatSim/log.txt

#/home/pi/mopower/mpcmd LED_STAT=0
#sleep 30

#/home/pi/CubeSatSim/configax
