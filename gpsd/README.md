Following steps from https://gpsd.gitlab.io/gpsd/installation.html

gpsd steps:

1. plug it in and type:

ls /dev/ttyUSB*

the last digit is your device number - could be 0 or 1 or other number.  The rest of the commands assume it is 0, so device is /dev/ttyUSB0

2. See if you can get NMEA data out of it (you may need to type it twice to get it to work):

sudo stty -F /dev/ttyUSB0 ispeed 4800 && cat </dev/ttyUSB0

3. install gpsd software:

sudo apt-get -y install gpsd gpsd-clients python-gps

4. disable tcp daemon

sudo systemctl disable gpsd

sudo systemctl disable gpsd.socket

sudo killall gpsd

5. Now run it manually

sudo gpsd /dev/ttyUSB0 -F /var/run/gpsd.sock

6. You can look at the result:

gpsmon

Try running ./gps.sh script to see if you get lat lon alt output
