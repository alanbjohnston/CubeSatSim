#!/bin/bash
# script to kill all SDR or RTL processes
#

echo -e "\nKilling all SDR or RTL processes \n"

sudo killall -9 csdr

sudo killall -9 openwebrx

sudo killall -9 rtl_tcp

sudo fuser -k 8073/tcp

sudo killall -9 FoxTelem

