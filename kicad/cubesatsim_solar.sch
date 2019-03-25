EESchema Schematic File Version 4
LIBS:cubesatsim_solar-cache
EELAYER 26 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "AMSAT CubeSat Simulator Solar Panel Board"
Date "February 27, 2019"
Rev "B3.0"
Comp "Author: KU2Y"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	1050 2300 1200 2300
Wire Wire Line
	1050 1600 1050 1700
Wire Wire Line
	1200 1600 1050 1600
Wire Wire Line
	1200 2200 1050 2200
Connection ~ 1050 2200
Wire Wire Line
	1200 2100 1050 2100
Connection ~ 1050 2100
Wire Wire Line
	1200 2000 1050 2000
Connection ~ 1050 2000
Wire Wire Line
	1200 1900 1050 1900
Connection ~ 1050 1900
Wire Wire Line
	1200 1800 1050 1800
Connection ~ 1050 1800
Wire Wire Line
	1200 1700 1050 1700
Connection ~ 1050 1700
Wire Wire Line
	1200 1000 1100 1000
Wire Wire Line
	1050 2200 1050 2300
Wire Wire Line
	1050 2100 1050 2200
Wire Wire Line
	1050 2000 1050 2100
Wire Wire Line
	1050 1900 1050 2000
Wire Wire Line
	1050 1800 1050 1900
Wire Wire Line
	1050 1700 1050 1800
Wire Wire Line
	1100 900  1200 900 
Text GLabel 7450 4900 0    50   Input ~ 0
+X_In
Text GLabel 7450 5000 0    50   Input ~ 0
+X_Out
Text GLabel 7450 5100 0    50   Input ~ 0
SDA_1
Text GLabel 7450 5200 0    50   Input ~ 0
SCL_1
Text GLabel 8975 4875 0    50   Input ~ 0
+Y_In
Text GLabel 4800 2425 0    50   Input ~ 0
+X_Out
$Comp
L pihat_template-rescue:GND-power #PWR03
U 1 1 5BF227F6
P 6900 5400
F 0 "#PWR03" H 6900 5150 50  0001 C CNN
F 1 "GND" H 6905 5227 50  0000 C CNN
F 2 "" H 6900 5400 50  0001 C CNN
F 3 "" H 6900 5400 50  0001 C CNN
	1    6900 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5400 6900 5300
Wire Wire Line
	6900 5300 7450 5300
$Comp
L pihat_template-rescue:+5V-power #PWR02
U 1 1 5BF23AF7
P 1100 900
F 0 "#PWR02" H 1100 750 50  0001 C CNN
F 1 "+5V" H 1115 1073 50  0000 C CNN
F 2 "" H 1100 900 50  0001 C CNN
F 3 "" H 1100 900 50  0001 C CNN
	1    1100 900 
	1    0    0    -1  
$EndComp
Connection ~ 1100 900 
Text GLabel 3750 1000 2    50   Input ~ 0
SCL_1
Wire Wire Line
	1100 1000 1100 900 
Text GLabel 3750 900  2    50   Input ~ 0
SDA_1
$Comp
L pihat_template-rescue:GND-power #PWR01
U 1 1 5BF2D5D9
P 850 2300
F 0 "#PWR01" H 850 2050 50  0001 C CNN
F 1 "GND" H 855 2127 50  0000 C CNN
F 2 "" H 850 2300 50  0001 C CNN
F 3 "" H 850 2300 50  0001 C CNN
	1    850  2300
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:D-device D1
U 1 1 5BF2DDAE
P 5350 2425
F 0 "D1" H 5350 2641 50  0000 C CNN
F 1 "1N5817" H 5350 2550 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 2425 50  0001 C CNN
F 3 "~" H 5350 2425 50  0001 C CNN
	1    5350 2425
	-1   0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J7
U 1 1 5BF2E1F2
P 7650 5100
F 0 "J7" H 7678 5076 50  0000 L CNN
F 1 "INA219 Sensor Board" H 7678 4985 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7650 5100 50  0001 C CNN
F 3 "~" H 7650 5100 50  0001 C CNN
	1    7650 5100
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:GND-power #PWR0102
U 1 1 5BF305B5
P 9025 4950
F 0 "#PWR0102" H 9025 4700 50  0001 C CNN
F 1 "GND" H 8900 4925 50  0000 C CNN
F 2 "" H 9025 4950 50  0001 C CNN
F 3 "" H 9025 4950 50  0001 C CNN
	1    9025 4950
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:GND-power #PWR0103
U 1 1 5BF5EFF0
P 8125 2350
F 0 "#PWR0103" H 8125 2100 50  0001 C CNN
F 1 "GND-power" H 8130 2177 50  0000 C CNN
F 2 "" H 8125 2350 50  0001 C CNN
F 3 "" H 8125 2350 50  0001 C CNN
	1    8125 2350
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:GND-power #PWR0104
U 1 1 5BF5F034
P 6650 2350
F 0 "#PWR0104" H 6650 2100 50  0001 C CNN
F 1 "GND-power" H 6655 2177 50  0000 C CNN
F 2 "" H 6650 2350 50  0001 C CNN
F 3 "" H 6650 2350 50  0001 C CNN
	1    6650 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2425 5625 2425
Wire Wire Line
	4800 2425 5200 2425
Wire Wire Line
	8975 4875 9125 4875
Text GLabel 8975 5075 0    50   Input ~ 0
+Z_In
Wire Wire Line
	8975 5075 9125 5075
Wire Wire Line
	9125 4975 9075 4975
Wire Wire Line
	9075 4975 9075 4950
Wire Wire Line
	9075 4950 9025 4950
Text GLabel 8950 5475 0    50   Input ~ 0
-X_In
$Comp
L pihat_template-rescue:GND-power #PWR0107
U 1 1 5BF6C7AE
P 9000 5550
F 0 "#PWR0107" H 9000 5300 50  0001 C CNN
F 1 "GND" H 8875 5525 50  0000 C CNN
F 2 "" H 9000 5550 50  0001 C CNN
F 3 "" H 9000 5550 50  0001 C CNN
	1    9000 5550
	1    0    0    -1  
$EndComp
Text GLabel 8975 4675 0    50   Input ~ 0
+X_In
$Comp
L pihat_template-rescue:GND-power #PWR0108
U 1 1 5BF6D13B
P 9025 4750
F 0 "#PWR0108" H 9025 4500 50  0001 C CNN
F 1 "GND" H 8900 4725 50  0000 C CNN
F 2 "" H 9025 4750 50  0001 C CNN
F 3 "" H 9025 4750 50  0001 C CNN
	1    9025 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8975 4675 9125 4675
Wire Wire Line
	9125 4775 9075 4775
Wire Wire Line
	9075 4775 9075 4750
Wire Wire Line
	9075 4750 9025 4750
Text GLabel 8950 5675 0    50   Input ~ 0
-Y_In
$Comp
L pihat_template-rescue:GND-power #PWR0109
U 1 1 5BF6DC40
P 9000 5750
F 0 "#PWR0109" H 9000 5500 50  0001 C CNN
F 1 "GND" H 8875 5725 50  0000 C CNN
F 2 "" H 9000 5750 50  0001 C CNN
F 3 "" H 9000 5750 50  0001 C CNN
	1    9000 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9125 5575 9075 5575
Wire Wire Line
	9075 5575 9075 5550
$Comp
L pihat_template-rescue:GND-power #PWR0110
U 1 1 5BF6E8ED
P 9025 5150
F 0 "#PWR0110" H 9025 4900 50  0001 C CNN
F 1 "GND" H 8900 5125 50  0000 C CNN
F 2 "" H 9025 5150 50  0001 C CNN
F 3 "" H 9025 5150 50  0001 C CNN
	1    9025 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9125 5175 9075 5175
Wire Wire Line
	9075 5175 9075 5150
Wire Wire Line
	9075 5150 9025 5150
Text GLabel 8950 5275 0    50   Input ~ 0
VbattIn
Text GLabel 9000 5375 0    50   Input ~ 0
VbattOut
Wire Wire Line
	7450 5400 7375 5400
Wire Wire Line
	7375 5400 7375 5475
Wire Wire Line
	7375 5475 7325 5475
Wire Wire Line
	7325 5475 7325 5450
Text GLabel 975  3100 0    50   Input ~ 0
SDA_3
Text GLabel 975  3000 0    50   Input ~ 0
SCL_3
$Comp
L pihat_template-rescue:Raspberry_Pi_+_Conn-pihat_template-rescue P3
U 1 1 54E92361
P 2300 2700
F 0 "P3" H 1200 4650 60  0000 C CNN
F 1 "GPIO Male Extra Long (19mm) 20x2 Header Pins" H 2050 1175 60  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical" H 2300 2700 60  0001 C CNN
F 3 "" H 2300 2700 60  0000 C CNN
F 4 "517-30340-6002" H 2300 2700 60  0001 C CNN "Mouser Part No."
	1    2300 2700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	975  3100 1175 3100
Wire Wire Line
	975  3000 1025 3000
$Comp
L pihat_template-rescue:RESISTOR-passive R2
U 1 1 5BF91C4F
P 1175 2850
F 0 "R2" V 1137 2903 60  0000 L CNB
F 1 "4.7k" V 1228 2903 40  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" H 1175 2710 40  0001 C CNN
F 3 "" H 1175 2950 60  0000 C CNN
F 4 "-" H 1175 3010 45  0001 C CNN "Part"
F 5 "Passive" H 1405 3140 50  0001 C CNN "Family"
	1    1175 2850
	0    1    1    0   
$EndComp
$Comp
L pihat_template-rescue:RESISTOR-passive R1
U 1 1 5BF91CF1
P 1025 2850
F 0 "R1" V 975 2675 60  0000 L CNB
F 1 "4.7k" V 1050 2500 40  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" H 1025 2710 40  0001 C CNN
F 3 "" H 1025 2950 60  0000 C CNN
F 4 "-" H 1025 3010 45  0001 C CNN "Part"
F 5 "Passive" H 1255 3140 50  0001 C CNN "Family"
	1    1025 2850
	0    1    1    0   
$EndComp
$Comp
L pihat_template-rescue:+3.3V-PiHatAx5043-cache #PWR04
U 1 1 5BF91DA2
P 1025 2725
F 0 "#PWR04" H 1025 2575 50  0001 C CNN
F 1 "+3.3V" H 1040 2898 50  0000 C CNN
F 2 "" H 1025 2725 50  0001 C CNN
F 3 "" H 1025 2725 50  0001 C CNN
	1    1025 2725
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2300 1050 2300
Connection ~ 1050 2300
Wire Wire Line
	1025 2725 1025 2750
Wire Wire Line
	1025 2750 1175 2750
Connection ~ 1025 2750
Wire Wire Line
	1175 2950 1175 3100
Connection ~ 1175 3100
Wire Wire Line
	1175 3100 1200 3100
Wire Wire Line
	1025 2950 1025 3000
Connection ~ 1025 3000
Wire Wire Line
	1025 3000 1200 3000
Text GLabel 2475 4950 0    50   Input ~ 0
VbattIn
Text GLabel 2475 5050 0    50   Input ~ 0
VbattOut
Text GLabel 2475 5150 0    50   Input ~ 0
SDA_1
Text GLabel 2475 5250 0    50   Input ~ 0
SCL_1
$Comp
L pihat_template-rescue:GND-power #PWR05
U 1 1 5BF9E33A
P 1925 5450
F 0 "#PWR05" H 1925 5200 50  0001 C CNN
F 1 "GND" H 1930 5277 50  0000 C CNN
F 2 "" H 1925 5450 50  0001 C CNN
F 3 "" H 1925 5450 50  0001 C CNN
	1    1925 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1925 5450 1925 5350
Wire Wire Line
	1925 5350 2475 5350
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J10
U 1 1 5BF9E342
P 2675 5150
F 0 "J10" H 2703 5126 50  0000 L CNN
F 1 "INA219 Sensor Board" H 2703 5035 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 2675 5150 50  0001 C CNN
F 3 "~" H 2675 5150 50  0001 C CNN
	1    2675 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2475 5450 2400 5450
Wire Wire Line
	2400 5450 2400 5525
Wire Wire Line
	2400 5525 2350 5525
$Comp
L pihat_template-rescue:+3.3V-PiHatAx5043-cache #PWR0101
U 1 1 5BFA1168
P 1075 1225
F 0 "#PWR0101" H 1075 1075 50  0001 C CNN
F 1 "+3.3V" H 1090 1398 50  0000 C CNN
F 2 "" H 1075 1225 50  0001 C CNN
F 3 "" H 1075 1225 50  0001 C CNN
	1    1075 1225
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1250 1075 1250
Wire Wire Line
	1075 1250 1075 1225
$Comp
L pihat_template-rescue:+5V-power #PWR0111
U 1 1 5BF939C5
P 2200 6575
F 0 "#PWR0111" H 2200 6425 50  0001 C CNN
F 1 "+5V" H 2215 6748 50  0000 C CNN
F 2 "" H 2200 6575 50  0001 C CNN
F 3 "" H 2200 6575 50  0001 C CNN
	1    2200 6575
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:+3.3V-PiHatAx5043-cache #PWR0112
U 1 1 5BF94230
P 2050 6650
F 0 "#PWR0112" H 2050 6500 50  0001 C CNN
F 1 "+3.3V" H 2065 6823 50  0000 C CNN
F 2 "" H 2050 6650 50  0001 C CNN
F 3 "" H 2050 6650 50  0001 C CNN
	1    2050 6650
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:GND-power #PWR0113
U 1 1 5BF9429B
P 1700 6775
F 0 "#PWR0113" H 1700 6525 50  0001 C CNN
F 1 "GND-power" H 1705 6602 50  0000 C CNN
F 2 "" H 1700 6775 50  0001 C CNN
F 3 "" H 1700 6775 50  0001 C CNN
	1    1700 6775
	1    0    0    -1  
$EndComp
Text GLabel 2200 6875 0    50   Input ~ 0
SDA_3
Text GLabel 2200 6975 0    50   Input ~ 0
SCL_3
Wire Wire Line
	2275 6875 2200 6875
Wire Wire Line
	2275 6975 2200 6975
Wire Wire Line
	2275 6775 1700 6775
Wire Wire Line
	2275 6675 2050 6675
Wire Wire Line
	2050 6675 2050 6650
Wire Wire Line
	2275 6575 2200 6575
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0114
U 1 1 5BFB9249
P 7325 5450
F 0 "#PWR0114" H 7325 5300 50  0001 C CNN
F 1 "power_+3.3V" H 7275 5375 50  0000 C CNN
F 2 "" H 7325 5450 50  0001 C CNN
F 3 "" H 7325 5450 50  0001 C CNN
	1    7325 5450
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0115
U 1 1 5BFB928B
P 2350 5525
F 0 "#PWR0115" H 2350 5375 50  0001 C CNN
F 1 "power_+3.3V" H 2375 5475 50  0000 C CNN
F 2 "" H 2350 5525 50  0001 C CNN
F 3 "" H 2350 5525 50  0001 C CNN
	1    2350 5525
	1    0    0    -1  
$EndComp
Text Notes 7750 5350 0    50   ~ 0
I2C Address 0x40
Text Notes 2775 5400 0    50   ~ 0
I2C Address 0x45
Text GLabel 5850 4925 0    50   Input ~ 0
+Y_In
Text GLabel 5850 5025 0    50   Input ~ 0
+Y_Out
Text GLabel 5850 5125 0    50   Input ~ 0
SDA_1
Text GLabel 5850 5225 0    50   Input ~ 0
SCL_1
$Comp
L pihat_template-rescue:GND-power #PWR0116
U 1 1 5BFB984D
P 5300 5425
F 0 "#PWR0116" H 5300 5175 50  0001 C CNN
F 1 "GND" H 5305 5252 50  0000 C CNN
F 2 "" H 5300 5425 50  0001 C CNN
F 3 "" H 5300 5425 50  0001 C CNN
	1    5300 5425
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 5425 5300 5325
Wire Wire Line
	5300 5325 5850 5325
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J8
U 1 1 5BFB9855
P 6050 5125
F 0 "J8" H 6078 5101 50  0000 L CNN
F 1 "INA219 Sensor Board" H 6078 5010 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6050 5125 50  0001 C CNN
F 3 "~" H 6050 5125 50  0001 C CNN
	1    6050 5125
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 5425 5775 5425
Wire Wire Line
	5775 5425 5775 5500
Wire Wire Line
	5775 5500 5725 5500
Wire Wire Line
	5725 5500 5725 5475
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0117
U 1 1 5BFB985F
P 5725 5475
F 0 "#PWR0117" H 5725 5325 50  0001 C CNN
F 1 "power_+3.3V" H 5675 5400 50  0000 C CNN
F 2 "" H 5725 5475 50  0001 C CNN
F 3 "" H 5725 5475 50  0001 C CNN
	1    5725 5475
	1    0    0    -1  
$EndComp
Text Notes 6125 5375 0    50   ~ 0
I2C Address 0x41\n
Text GLabel 4175 4950 0    50   Input ~ 0
+Z_In
Text GLabel 4175 5050 0    50   Input ~ 0
+Z_Out
Text GLabel 4175 5150 0    50   Input ~ 0
SDA_1
Text GLabel 4175 5250 0    50   Input ~ 0
SCL_1
$Comp
L pihat_template-rescue:GND-power #PWR0118
U 1 1 5BFBC210
P 3625 5450
F 0 "#PWR0118" H 3625 5200 50  0001 C CNN
F 1 "GND" H 3630 5277 50  0000 C CNN
F 2 "" H 3625 5450 50  0001 C CNN
F 3 "" H 3625 5450 50  0001 C CNN
	1    3625 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3625 5450 3625 5350
Wire Wire Line
	3625 5350 4175 5350
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J9
U 1 1 5BFBC218
P 4375 5150
F 0 "J9" H 4403 5126 50  0000 L CNN
F 1 "INA219 Sensor Board" H 4403 5035 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4375 5150 50  0001 C CNN
F 3 "~" H 4375 5150 50  0001 C CNN
	1    4375 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4175 5450 4100 5450
Wire Wire Line
	4100 5450 4100 5525
Wire Wire Line
	4100 5525 4050 5525
Wire Wire Line
	4050 5525 4050 5500
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0119
U 1 1 5BFBC222
P 4050 5500
F 0 "#PWR0119" H 4050 5350 50  0001 C CNN
F 1 "power_+3.3V" H 4000 5425 50  0000 C CNN
F 2 "" H 4050 5500 50  0001 C CNN
F 3 "" H 4050 5500 50  0001 C CNN
	1    4050 5500
	1    0    0    -1  
$EndComp
Text Notes 4450 5400 0    50   ~ 0
I2C Address 0x44\n
Text GLabel 7475 5775 0    50   Input ~ 0
-X_In
Text GLabel 7475 5875 0    50   Input ~ 0
-X_Out
Text GLabel 7475 5975 0    50   Input ~ 0
SDA_3
Text GLabel 7475 6075 0    50   Input ~ 0
SCL_3
$Comp
L pihat_template-rescue:GND-power #PWR0120
U 1 1 5BFBED52
P 6925 6275
F 0 "#PWR0120" H 6925 6025 50  0001 C CNN
F 1 "GND" H 6930 6102 50  0000 C CNN
F 2 "" H 6925 6275 50  0001 C CNN
F 3 "" H 6925 6275 50  0001 C CNN
	1    6925 6275
	1    0    0    -1  
$EndComp
Wire Wire Line
	6925 6275 6925 6175
Wire Wire Line
	6925 6175 7475 6175
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J11
U 1 1 5BFBED5A
P 7675 5975
F 0 "J11" H 7703 5951 50  0000 L CNN
F 1 "INA219 Sensor Board" H 7703 5860 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7675 5975 50  0001 C CNN
F 3 "~" H 7675 5975 50  0001 C CNN
	1    7675 5975
	1    0    0    -1  
$EndComp
Wire Wire Line
	7475 6275 7400 6275
Wire Wire Line
	7400 6275 7400 6350
Wire Wire Line
	7400 6350 7350 6350
Wire Wire Line
	7350 6350 7350 6325
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0121
U 1 1 5BFBED64
P 7350 6325
F 0 "#PWR0121" H 7350 6175 50  0001 C CNN
F 1 "power_+3.3V" H 7300 6250 50  0000 C CNN
F 2 "" H 7350 6325 50  0001 C CNN
F 3 "" H 7350 6325 50  0001 C CNN
	1    7350 6325
	1    0    0    -1  
$EndComp
Text Notes 7750 6225 0    50   ~ 0
I2C Address 0x40
Text GLabel 5800 5775 0    50   Input ~ 0
-Y_In
Text GLabel 5800 5875 0    50   Input ~ 0
-Y_Out
Text GLabel 5800 5975 0    50   Input ~ 0
SDA_3
Text GLabel 5800 6075 0    50   Input ~ 0
SCL_3
$Comp
L pihat_template-rescue:GND-power #PWR0122
U 1 1 5BFC1EBA
P 5250 6275
F 0 "#PWR0122" H 5250 6025 50  0001 C CNN
F 1 "GND" H 5255 6102 50  0000 C CNN
F 2 "" H 5250 6275 50  0001 C CNN
F 3 "" H 5250 6275 50  0001 C CNN
	1    5250 6275
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 6275 5250 6175
Wire Wire Line
	5250 6175 5800 6175
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J12
U 1 1 5BFC1EC2
P 6000 5975
F 0 "J12" H 6028 5951 50  0000 L CNN
F 1 "INA219 Sensor Board" H 6028 5860 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6000 5975 50  0001 C CNN
F 3 "~" H 6000 5975 50  0001 C CNN
	1    6000 5975
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 6275 5725 6275
Wire Wire Line
	5725 6275 5725 6350
Wire Wire Line
	5725 6350 5675 6350
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0123
U 1 1 5BFC1ECB
P 5675 6350
F 0 "#PWR0123" H 5675 6200 50  0001 C CNN
F 1 "power_+3.3V" H 5700 6300 50  0000 C CNN
F 2 "" H 5675 6350 50  0001 C CNN
F 3 "" H 5675 6350 50  0001 C CNN
	1    5675 6350
	1    0    0    -1  
$EndComp
Text Notes 6100 6225 0    50   ~ 0
I2C Address 0x41
Text GLabel 4800 2750 0    50   Input ~ 0
+Y_Out
$Comp
L pihat_template-rescue:D-device D2
U 1 1 5BFCE14D
P 5350 2750
F 0 "D2" H 5350 2966 50  0000 C CNN
F 1 "1N5817" H 5350 2875 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 2750 50  0001 C CNN
F 3 "~" H 5350 2750 50  0001 C CNN
	1    5350 2750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 2750 5625 2750
Wire Wire Line
	4800 2750 5200 2750
Text GLabel 4800 3100 0    50   Input ~ 0
+Z_Out
$Comp
L pihat_template-rescue:D-device D3
U 1 1 5BFD1A73
P 5350 3100
F 0 "D3" H 5350 3316 50  0000 C CNN
F 1 "1N5817" H 5350 3225 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 3100 50  0001 C CNN
F 3 "~" H 5350 3100 50  0001 C CNN
	1    5350 3100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 3100 5625 3100
Wire Wire Line
	4800 3100 5200 3100
Text GLabel 4800 3425 0    50   Input ~ 0
-X_Out
$Comp
L pihat_template-rescue:D-device D4
U 1 1 5BFD565A
P 5350 3425
F 0 "D4" H 5350 3641 50  0000 C CNN
F 1 "1N5817" H 5350 3550 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 3425 50  0001 C CNN
F 3 "~" H 5350 3425 50  0001 C CNN
	1    5350 3425
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 3425 5625 3425
Wire Wire Line
	4800 3425 5200 3425
Text GLabel 4800 3775 0    50   Input ~ 0
-Y_Out
$Comp
L pihat_template-rescue:D-device D5
U 1 1 5BFD9541
P 5350 3775
F 0 "D5" H 5350 3991 50  0000 C CNN
F 1 "1N5817" H 5350 3900 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 3775 50  0001 C CNN
F 3 "~" H 5350 3775 50  0001 C CNN
	1    5350 3775
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 3775 5625 3775
Wire Wire Line
	4800 3775 5200 3775
Wire Wire Line
	5625 2425 5625 2750
Connection ~ 5625 2425
Wire Wire Line
	5625 2750 5625 3100
Connection ~ 5625 2750
Wire Wire Line
	5625 3100 5625 3425
Connection ~ 5625 3100
Wire Wire Line
	5625 3775 5625 3425
Connection ~ 5625 3425
$Comp
L pihat_template-rescue:Conn_01x05_Female-Connector J6
U 1 1 5BFF9AD8
P 2475 6775
F 0 "J6" H 2503 6801 50  0000 L CNN
F 1 "Expansion Interface" H 2503 6710 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 2475 6775 50  0001 C CNN
F 3 "~" H 2475 6775 50  0001 C CNN
	1    2475 6775
	1    0    0    -1  
$EndComp
Wire Wire Line
	5625 1675 5625 2425
Wire Wire Line
	6650 2350 6800 2350
Wire Wire Line
	8125 2350 7975 2350
Text Label 12250 10925 0    50   ~ 0
AMSAT CubeSat Simulator Solar Power Board
$Comp
L pihat_template-rescue:CONN_01X01-conn2 J17
U 1 1 5C0D6034
P 7000 2100
F 0 "J17" H 7100 2100 50  0000 L CNN
F 1 "IN+" H 6775 2050 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 7000 2100 50  0001 C CNN
F 3 "" H 7000 2100 50  0001 C CNN
	1    7000 2100
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:CONN_01X01-conn2 J14
U 1 1 5C0D608E
P 7000 2350
F 0 "J14" H 7078 2391 50  0000 L CNN
F 1 "IN-" H 6775 2300 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 7000 2350 50  0001 C CNN
F 3 "" H 7000 2350 50  0001 C CNN
	1    7000 2350
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:CONN_01X01-conn2 J15
U 1 1 5C0D60E4
P 7775 2100
F 0 "J15" H 7900 2100 50  0000 C CNN
F 1 "OUT+" H 7575 2050 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 7775 2100 50  0001 C CNN
F 3 "" H 7775 2100 50  0001 C CNN
	1    7775 2100
	-1   0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:CONN_01X01-conn2 J16
U 1 1 5C0D6176
P 7775 2350
F 0 "J16" H 7900 2350 50  0000 C CNN
F 1 "OUT-" H 7575 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 7775 2350 50  0001 C CNN
F 3 "" H 7775 2350 50  0001 C CNN
	1    7775 2350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7975 2100 8050 2100
Wire Wire Line
	6700 1775 6700 2100
Wire Wire Line
	6700 2100 6800 2100
Text Notes 9500 3050 0    50   ~ 0
5.5mm x 2.1mm
Text Notes 9500 3150 0    50   ~ 0
female barrel jack
Text Notes 5700 6600 0    50   Italic 0
INA219 boards are mounted vertically with 90 degree 1x6 header pins
Text Notes 9150 4525 0    50   ~ 0
To Solar Panels
Text Notes 6750 2900 0    50   ~ 0
Boost Converter trim pot adjusted to \ngive 15V output at maximum solar \npanel voltage input.
Wire Wire Line
	8550 2300 8925 2300
$Comp
L 35RAPC4BH3:35RAPC4BH3 J3
U 1 1 5C4F657F
P 9350 1125
F 0 "J3" H 9021 1171 50  0000 R CNN
F 1 "35RAPC4BH3" H 9021 1080 50  0000 R CNN
F 2 "35RAPC4BH3:SWITCHCRAFT_35RAPC4BH3" H 9350 1125 50  0001 L BNN
F 3 "None" H 9350 1125 50  0001 L BNN
F 4 "35RAPC4BH3" H 9350 1125 50  0001 L BNN "Field4"
F 5 "3.5mm Threaded Right Angle Stereo Jack" H 9350 1125 50  0001 L BNN "Field5"
F 6 "https://www.digikey.com/product-detail/en/switchcraft-inc/35RAPC4BH3/SC1464-ND/1288781?utm_source=snapeda&utm_medium=aggregator&utm_campaign=symbol" H 9350 1125 50  0001 L BNN "Field6"
F 7 "SC1464-ND" H 9350 1125 50  0001 L BNN "Field7"
F 8 "Switchcraft Inc." H 9350 1125 50  0001 L BNN "Field8"
	1    9350 1125
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8550 1325 8850 1325
Wire Wire Line
	8050 1225 8850 1225
Wire Wire Line
	8850 1125 8450 1125
Wire Wire Line
	8450 1125 8450 1675
Wire Wire Line
	5625 1675 8450 1675
Wire Wire Line
	8350 1025 8350 1775
Wire Wire Line
	6700 1775 8350 1775
Wire Wire Line
	8650 925  8850 925 
Wire Wire Line
	8350 1025 8850 1025
Text Notes 5725 1050 0    50   ~ 0
With RBF switch inserted (3.5mm jack plugged in),\n Vout to MoPower comes from the Vdc in barrel jack\n through 3.5mm plug that has jumper between 3 and 1 \n(center and outer). Solar panels are isolated from the input to \nthe boost converter through the switch.
Text Notes 5800 1500 0    50   ~ 0
With the RBF switch removed, the solar panels are \nconnected to the boost converter input and the \noutput of the boost converter is connected to \nthe Vout to the MoPower board.
Text Notes 8925 1625 0    50   ~ 0
RBF switch is mounted on \nPC board.\n
Wire Notes Line style solid rgb(132, 0, 0)
	6925 2025 6925 2425
Wire Notes Line style solid rgb(132, 0, 0)
	6925 2425 7850 2425
Wire Notes Line style solid rgb(132, 0, 0)
	7850 2425 7850 2025
Wire Notes Line style solid rgb(132, 0, 0)
	7850 2025 6925 2025
Text Notes 7050 1975 0    50   ~ 0
U1 Boost Converter
$Comp
L PJ-037AH:PJ-037AH J1
U 1 1 5C775C0E
P 9100 3200
F 0 "J1" H 8800 3100 50  0000 R CNN
F 1 "PJ-037AH" H 8625 3100 50  0000 R CNN
F 2 "PJ-037AH:CUI_PJ-037AH" H 9100 3200 50  0001 L BNN
F 3 "Manufacturer recommendations" H 9100 3200 50  0001 L BNN
F 4 "CUI INC" H 9100 3200 50  0001 L BNN "Field4"
	1    9100 3200
	-1   0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:GND-power #PWR06
U 1 1 5C78910F
P 8900 3350
F 0 "#PWR06" H 8900 3100 50  0001 C CNN
F 1 "GND-power" H 8905 3177 50  0000 C CNN
F 2 "" H 8900 3350 50  0001 C CNN
F 3 "" H 8900 3350 50  0001 C CNN
	1    8900 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 3300 8900 3350
Wire Wire Line
	8900 3100 8650 3100
$Comp
L pihat_template-rescue:GND-power #PWR0106
U 1 1 5BF609DA
P 8925 2400
F 0 "#PWR0106" H 8925 2150 50  0001 C CNN
F 1 "GND-power" H 8930 2227 50  0000 C CNN
F 2 "" H 8925 2400 50  0001 C CNN
F 3 "" H 8925 2400 50  0001 C CNN
	1    8925 2400
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:CONN_01X02-conn2 J4
U 1 1 5BF6089F
P 9125 2350
F 0 "J4" H 9203 2391 50  0000 L CNN
F 1 "Vout to MoPOWER" H 9203 2300 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x01_P2.54mm_Vertical" H 9125 2350 50  0001 C CNN
F 3 "" H 9125 2350 50  0001 C CNN
	1    9125 2350
	1    0    0    -1  
$EndComp
Text GLabel 4225 5750 0    50   Input ~ 0
-Z_In
Text GLabel 4225 5850 0    50   Input ~ 0
-Z_Out
Text GLabel 4225 5950 0    50   Input ~ 0
SDA_3
Text GLabel 4225 6050 0    50   Input ~ 0
SCL_3
$Comp
L pihat_template-rescue:GND-power #PWR0105
U 1 1 5C7AD97F
P 3675 6250
F 0 "#PWR0105" H 3675 6000 50  0001 C CNN
F 1 "GND" H 3680 6077 50  0000 C CNN
F 2 "" H 3675 6250 50  0001 C CNN
F 3 "" H 3675 6250 50  0001 C CNN
	1    3675 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 6250 3675 6150
Wire Wire Line
	3675 6150 4225 6150
$Comp
L pihat_template-rescue:Conn_01x06_Female-Connector J13
U 1 1 5C7AD987
P 4425 5950
F 0 "J13" H 4453 5926 50  0000 L CNN
F 1 "INA219 Sensor Board" H 4453 5835 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4425 5950 50  0001 C CNN
F 3 "~" H 4425 5950 50  0001 C CNN
	1    4425 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4225 6250 4150 6250
Wire Wire Line
	4150 6250 4150 6325
Wire Wire Line
	4150 6325 4100 6325
$Comp
L pihat_template-rescue:power_+3.3V-PiHatAx5043-cache #PWR0124
U 1 1 5C7AD991
P 4100 6325
F 0 "#PWR0124" H 4100 6175 50  0001 C CNN
F 1 "power_+3.3V" H 4125 6275 50  0000 C CNN
F 2 "" H 4100 6325 50  0001 C CNN
F 3 "" H 4100 6325 50  0001 C CNN
	1    4100 6325
	1    0    0    -1  
$EndComp
Text Notes 4525 6200 0    50   ~ 0
I2C Address 0x44
Text GLabel 4800 4150 0    50   Input ~ 0
-Z_Out
$Comp
L pihat_template-rescue:D-device D6
U 1 1 5C7B2D77
P 5350 4150
F 0 "D6" H 5350 4366 50  0000 C CNN
F 1 "1N5817" H 5350 4275 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 5350 4150 50  0001 C CNN
F 3 "~" H 5350 4150 50  0001 C CNN
	1    5350 4150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 4150 5625 4150
Wire Wire Line
	4800 4150 5200 4150
Wire Wire Line
	5625 3775 5625 4150
Connection ~ 5625 3775
$Comp
L conn2:CONN_01X14 J2
U 1 1 5C7E4666
P 9325 5325
F 0 "J2" H 9403 5366 50  0000 L CNN
F 1 "CONN_01X14" H 9403 5275 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 9325 5325 50  0001 C CNN
F 3 "" H 9325 5325 50  0001 C CNN
	1    9325 5325
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 5475 9125 5475
Wire Wire Line
	8950 5675 9125 5675
Wire Wire Line
	9075 5750 9075 5775
Wire Wire Line
	9075 5775 9125 5775
Wire Wire Line
	9000 5750 9075 5750
Wire Wire Line
	9000 5550 9075 5550
Wire Wire Line
	8950 5275 9125 5275
Wire Wire Line
	9125 5375 9000 5375
Text GLabel 8950 5875 0    50   Input ~ 0
-Z_In
$Comp
L pihat_template-rescue:GND-power #PWR0125
U 1 1 5C82013E
P 9000 5950
F 0 "#PWR0125" H 9000 5700 50  0001 C CNN
F 1 "GND" H 8875 5925 50  0000 C CNN
F 2 "" H 9000 5950 50  0001 C CNN
F 3 "" H 9000 5950 50  0001 C CNN
	1    9000 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 5875 9125 5875
Wire Wire Line
	9075 5950 9075 5975
Wire Wire Line
	9075 5975 9125 5975
Wire Wire Line
	9000 5950 9075 5950
Wire Wire Line
	8550 1325 8550 2300
Wire Wire Line
	8650 925  8650 3100
Wire Wire Line
	8050 1225 8050 2100
$Comp
L pihat_template-rescue:D-device D7
U 1 1 5C7BFAE7
P 4075 2800
F 0 "D7" H 4075 3016 50  0000 C CNN
F 1 "LED" H 4075 2925 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P3.81mm_Vertical_AnodeUp" H 4075 2800 50  0001 C CNN
F 3 "~" H 4075 2800 50  0001 C CNN
	1    4075 2800
	0    1    -1   0   
$EndComp
$Comp
L pihat_template-rescue:RESISTOR-passive R3
U 1 1 5C7C98E3
P 4075 2475
F 0 "R3" V 4025 2300 60  0000 L CNB
F 1 "220" V 4050 2550 40  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" H 4075 2335 40  0001 C CNN
F 3 "" H 4075 2575 60  0000 C CNN
F 4 "-" H 4075 2635 45  0001 C CNN "Part"
F 5 "Passive" H 4305 2765 50  0001 C CNN "Family"
	1    4075 2475
	0    1    1    0   
$EndComp
$Comp
L pihat_template-rescue:+3.3V-PiHatAx5043-cache #PWR07
U 1 1 5C7CE806
P 1025 2725
F 0 "#PWR07" H 1025 2575 50  0001 C CNN
F 1 "+3.3V" H 1040 2898 50  0000 C CNN
F 2 "" H 1025 2725 50  0001 C CNN
F 3 "" H 1025 2725 50  0001 C CNN
	1    1025 2725
	1    0    0    -1  
$EndComp
$Comp
L pihat_template-rescue:+3.3V-PiHatAx5043-cache #PWR08
U 1 1 5C7D3690
P 4075 2300
F 0 "#PWR08" H 4075 2150 50  0001 C CNN
F 1 "+3.3V" H 4090 2473 50  0000 C CNN
F 2 "" H 4075 2300 50  0001 C CNN
F 3 "" H 4075 2300 50  0001 C CNN
	1    4075 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4075 2300 4075 2375
Wire Wire Line
	4075 2575 4075 2650
Wire Wire Line
	4075 2950 4075 3000
Wire Wire Line
	4075 3000 3750 3000
$EndSCHEMATC
