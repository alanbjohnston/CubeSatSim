#!/bin/bash

# from https://stackoverflow.com/questions/28387230/use-gpsd-or-cgps-to-return-latitude-and-longitude-then-quit

x=$(gpspipe -w -n 10 |grep lon|tail -n1|cut -d":" -f9|cut -d"," -f1)
y=$(gpspipe -w -n 10 |grep lon|tail -n1|cut -d":" -f10|cut -d"," -f1)
a=$(gpspipe -w -n 10 |grep lon|tail -n1|cut -d":" -f11|cut -d"," -f1)

echo "$x $y $a"

