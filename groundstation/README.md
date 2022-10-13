The files relate to the optional Raspberry Pi Ground Station.

The Ground Station can be installed on any Windows or Linux PC or laptop, but it does require drivers to be installed (RTL-SDR)
and virtual audio cable (for sound) to be installed.  This requires administrator privileges on the computer.

A simpler approach perhaps well suited to school environments is to use a dedicated Raspberry Pi Ground Station.

These scripts are part of the ARISS Radio Pi.  The setup is described here https://github.com/alanbjohnston/CubeSatSim/wiki/3.-Ground-Station

The Raspberry Pi Ground Station can be used as a stand-alone ground station for receiving, decoding, and analyzing telemetry 
from the CubeSat Simulator if an HDMI monitor, USB keyboard, and USB mouse are plugged into the Pi.

Alternatively, with another computer or laptop that has SSH access to the Pi, the telemetry can be received and decoded on the Pi
and analyzed on the other computer or laptop.

Also, a web SDR known as OpenWebRX can be run on the Pi so that other PCs or laptops on the same WiFi can use the SDR to tune the
telemetry with only a web browser.

See https://github.com/alanbjohnston/CubeSatSim/wiki/Raspberry-Pi-Ground-Station-Setup for the details.

If you have a Fox-in-a-Box-v3 Raspberry Pi image, here's how you can upgrade the version of FoxTelem:

Steps to Upgrade version of FoxTelem for FIABv3
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

On your Pi, open the Chromium browser and download the new linux version with this URL: https://www.g0kla.com/foxtelem/downloads/test/

Close Chromium.

Open File Manager (file icon at top)

Click on Downloads

Right click on the new version you just downloaded and select Extract To

Click on the little file icon next to Extract To then click on Desktop then Open.  Or, just type this in the Extract To box: /home/pi/Desktop

Click Extract

Close File Manager

You should now see a new Folder on the Desktop with the new version of FoxTelem.  The name of the folder is what you will type in a command below.  For example FoxTelem_1.12z1_linux

Open a Terminal Window (click on black box in top) then type:

rm FoxTelem

ln -s Desktop/FoxTelem_1.12z1_linux/ FoxTelem

Close the Terminal Window.

Now, when you run FoxTelem the first time, you may get an error such as this: Could not read properties file.  If this is a new release then the format has probably been extended.  Should I create a new properties file after reading as much as possible from the existing one?  

Click Yes 

You may then get messages saying there is a newer spacecraft file - click Yes each time.

You will need to do this for each profile of FoxTelem such as FIAB, CubeSatSim Ground Station, or Download.

You can now move the old FoxTelem version folder to the trash and empty it.

