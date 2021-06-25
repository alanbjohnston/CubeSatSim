EESchema Schematic File Version 2
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:brooktre
LIBS:cmos4000
LIBS:cmos_ieee
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:Lattice
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:memory
LIBS:microchip
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:microcontrollers
LIBS:motor_drivers
LIBS:motorola
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:power
LIBS:powerint
LIBS:Power_Management
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:Xicor
LIBS:xilinx
LIBS:Zilog
LIBS:pihat_template-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BSS138 Q1
U 1 1 54F14605
P 6000 3100
F 0 "Q1" H 6000 2951 40  0000 R CNN
F 1 "BSS138" H 6000 3250 40  0000 R CNN
F 2 "w_smd_trans:sot23" H 5870 3202 29  0000 C CNN
F 3 "" H 6000 3100 60  0000 C CNN
F 4 "621-DMG2305UX-13" H 6000 3100 60  0001 C CNN "Mouser Part No."
	1    6000 3100
	0    -1   -1   0   
$EndComp
Text HLabel 5000 3000 0    47   Input ~ 0
5V_SUPPLY
Text HLabel 6900 3000 2    47   Input ~ 0
5V_MCU
$Comp
L DMMT5401 U5
U 1 1 54F1CFA3
P 6000 4000
F 0 "U5" H 5700 4150 47  0000 C CNN
F 1 "DMMT5401" H 6450 3800 47  0000 C CNN
F 2 "w_smd_trans:sot23-6" H 5850 3700 47  0001 C CNN
F 3 "" H 5850 3700 47  0000 C CNN
F 4 "621-DMMT5401-7-F" H 6000 4000 60  0001 C CNN "Mouser Part No."
	1    6000 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3000 6900 3000
Wire Wire Line
	5000 3000 5800 3000
Wire Wire Line
	5800 3700 5800 3500
Wire Wire Line
	5800 3500 5650 3500
Wire Wire Line
	5650 3500 5650 3000
Connection ~ 5650 3000
Wire Wire Line
	6200 3700 6200 3500
Wire Wire Line
	6200 3500 6350 3500
Wire Wire Line
	6350 3500 6350 3000
Connection ~ 6350 3000
$Comp
L R R12
U 1 1 54F1D101
P 6200 4800
F 0 "R12" V 6280 4800 50  0000 C CNN
F 1 "47k" V 6207 4801 50  0000 C CNN
F 2 "w_smd_resistors:r_0402" V 6130 4800 30  0001 C CNN
F 3 "" H 6200 4800 30  0000 C CNN
F 4 "667-ERJ-2RKF4702X" V 6200 4800 60  0001 C CNN "Mouser Part No."
	1    6200 4800
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 54F1D193
P 5800 4800
F 0 "R11" V 5880 4800 50  0000 C CNN
F 1 "10k" V 5807 4801 50  0000 C CNN
F 2 "w_smd_resistors:r_0402" V 5730 4800 30  0001 C CNN
F 3 "" H 5800 4800 30  0000 C CNN
F 4 "71-CRCW0402-10K-E3" V 5800 4800 60  0001 C CNN "Mouser Part No."
	1    5800 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 54F1D1B9
P 6200 5200
F 0 "#PWR021" H 6200 4950 60  0001 C CNN
F 1 "GND" H 6200 5050 60  0000 C CNN
F 2 "" H 6200 5200 60  0000 C CNN
F 3 "" H 6200 5200 60  0000 C CNN
	1    6200 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 54F1D1D3
P 5800 5200
F 0 "#PWR022" H 5800 4950 60  0001 C CNN
F 1 "GND" H 5800 5050 60  0000 C CNN
F 2 "" H 5800 5200 60  0000 C CNN
F 3 "" H 5800 5200 60  0000 C CNN
	1    5800 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4350 5800 4650
Wire Wire Line
	5800 4950 5800 5200
Wire Wire Line
	6200 4350 6200 4650
Wire Wire Line
	6200 4950 6200 5200
Wire Wire Line
	5900 4350 5900 4450
Wire Wire Line
	5800 4450 6100 4450
Connection ~ 5800 4450
Wire Wire Line
	6100 4450 6100 4350
Connection ~ 5900 4450
Wire Wire Line
	6050 3300 6050 3600
Wire Wire Line
	6050 3600 6650 3600
Wire Wire Line
	6650 3600 6650 4450
Wire Wire Line
	6650 4450 6200 4450
Connection ~ 6200 4450
Text Notes 4800 2650 0    47   ~ 0
*Recomended back powering protection for raspberry pi. Recomended by\nRaspberyy Pi Foundation\nhttps://github.com/raspberrypi/hats/blob/master/designguide.md
$EndSCHEMATC
