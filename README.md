# CubeSatSim

The CubeSat Simulator https://github.com/alanbjohnston/CubeSatSim/wiki is a low cost satellite emulator that runs on solar panels and batteries, transmits UHF radio telemetry, has a 3D printed frame, and can be extended by additional sensors and modules.  This project is sponsored by the not-for-profit [Radio Amateur Satellite Corporation, AMSAT®](https://amsat.org) https://CubeSatSim.org

<img width="500" alt="CubeSatSim v2" src="https://CubeSatSim.org/v2/cubesatsim%20v2%20complete.png">

The three custom PCBs are the Main, Battery and Solar boards.  Information including schematics and gerbers is in https://CubeSatSim.org/hardware

<img width="200" alt="cubesatsim-main-v2 0 1_top" src="https://github.com/alanbjohnston/CubeSatSim/blob/628fd65edbb01fb772a9f223605d969a0aad14bd/hardware/v2.1/cubesatsim-main-v2.1.0_top.png"> <img width="200" alt="cubesatsim-battery-v2 0_top" src="https://github.com/alanbjohnston/CubeSatSim/blob/4d079b486116180071c4dbf64ae2464518a63bdd/hardware/v2.1/cubesatsim-battery-v2.1.0_top.png"> <img width="200" alt="cubesatsim-solar-v2 0_top" src="https://github.com/alanbjohnston/CubeSatSim/blob/c904c66d7c8fb44a3d088d0cbebf24903cf3572c/hardware/v2.1/cubesatsim-solar-v2.1.0_top.png">

For information about the AMSAT STEM education program based on the CubeSatSim, see https://CubeSatSim.org/EducatorsGuide

For STEM activities to do with the CubeSatSim, see the https://CubeSatSim.org/ActivityGuides

There are several hardware versions and software branches to go with them - see below for information.

See the Wiki Software Install page for more details: https://github.com/alanbjohnston/CubeSatSim/wiki/2.-Software-Install. The software runs on any Raspberry Pi including the Raspberry Pi 4B, 3B, 3B+, Pi Zero, Pi Zero W, or Pi Zero 2 W.

There are two ways to get the CubeSatSim(TM) software for your Pi.

One option is to download the disk image file and write it to a 16GB micro SD card or larger.  The image is based on Raspberry Pi OS (Bulleye) Lite. All software is installed, you just need to login to change your password and set your amateur radio callsign if you have one.  You can run the ./update.sh script to update all packages and update and compile the latest CubeSatSim software.

The other option is to start with a Raspberry Pi OS (Rasbian) image (only Bullseye works, Bookworm or Trixie are not compatible) and run the installation script ./install.sh which will install and compile all the related software.  This includes the following packages:
- wiringpi
- git
- libasound2-dev
- pi-power-button
- Direwolf
- rpitx
- python3-pip
- python3-smbus

## Disk Image Option Steps

See the Wiki Software Install page for details: [https://github.com/alanbjohnston/CubeSatSim/wiki/2.-Software-Install](https://github.com/alanbjohnston/CubeSatSim/wiki/2.-Software-Install#disk-image-option-steps).

## Installation Script Option Steps

CubeSatSim runs on the Bullseye version of Raspberry Pi OS (Desktop or Lite), although a Pi Zero or Pi Zero W should only run Lite.  Your Pi will need to have internet access to update settings and install packages.

For the status of our efforts to get the code running on Bookworm, see https://github.com/alanbjohnston/CubeSatSim/discussions/389

Here is a Bullseye Pi OS image that works: https://downloads.raspberrypi.org/raspios_full_armhf/images/raspios_full_armhf-2023-05-03/ Here is the file download link: https://downloads.raspberrypi.org/raspios_full_armhf/images/raspios_full_armhf-2023-05-03/2023-05-03-raspios-bullseye-armhf-full.img.xz

You can use this with the Raspberry Pi Imager, but under `OS` you need to choose the `Use Custom` option at the very bottom of the list, then select the downloaded image `2023-05-03-raspios-bullseye-armhf-full.img.xz`

Unfortunately, Customization is not supported with a Custom image, so you can't preset your username, password, localization, and WiFi settings.

So after you write this image to your microSD card, you will need to connect a monitor and keyboard to your Pi Zero 2 to set these things (make sure you set `pi` as the username). After that, you can connect it to your WiFi and turn on ssh and vnc for remote access.

You can then follow the software installation steps and it work work.  The only hiccup is that if you run the installer script while running the Desktop, the Direwolf installation will freeze.  But you can just cycle the power to the Pi Zero 2, then run the installer script again and it will complete successfully.  Alternatively, if you use `sudo raspi-config` to change to Boot to Console (System/ Boot Auto Login/ Console or Console Auto Login) then the installer script will complete without any issues.

*Note:* CubeSatSim expects to be installed as the `pi` user and references many paths in `/home/pi/*`. Installing as a different user will result in configuration files not saving along with many other issues.

To install the software follow these steps:

`sudo apt-get update`

`sudo apt-get install -y git`

`git clone http://github.com/alanbjohnston/CubeSatSim.git`

`cd CubeSatSim`

`git checkout master`

`# You are now ready to install the software using this script in the CubeSatSim directory:`

`./install`

The installation script will run for quite a while.  You will get prompted for your amateur radio callsign in all capitals, if you have one.  If you don't you can put some other word which will be transmitted as a CW ID.  You will also be prompted for your latitude and longitude for APRS packets. The script will prompt you if you want to modify /boot/config.txt file.  Type a `y` and the script will complete.  You will need to reboot.

See the Wiki for more details about the CubeSatSim [https://github.com/alanbjohnston/CubeSatSim/wiki](https://github.com/alanbjohnston/CubeSatSim/wiki/v1.3.2-0.-Home)

Here are some Quick Start Guides for operating the CubeSatSim: https://CubeSatSim.org/qsg

Older Versions

There are several versions of the hardware and software to go with them:

- The original design used a Digital Transceiver Board for the Raspberry Pi (DigitalTxRxRPi) and APRS AFSK 1200 bps telemetry only.  Use the aprs-digitaltxrx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-digitaltxrx for this and the vB2 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB2-Home (e.g. run `git checkout aprs-digitaltxrx` before running the ./install.sh script)
- An updated design that used the Transmitter/Filter Board (TFB) instead of the the Transceiver Board.  For the same APRS AFSK telemetry, use the aprs-rpitx branch https://github.com/alanbjohnston/CubeSatSim/tree/aprs-rpitx and the vB3 wiki page https://github.com/alanbjohnston/CubeSatSim/wiki/vB3-Home. (e.g. run `git checkout aprs-rpitx` before running the ./install.sh script) Note that this hardware can still use the master code branch.
