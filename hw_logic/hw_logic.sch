EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:components
LIBS:hw_logic-cache
EELAYER 25 0
EELAYER END
$Descr User 10000 6000
encoding utf-8
Sheet 1 1
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
L +5V #PWR01
U 1 1 5850BCCE
P 1650 750
F 0 "#PWR01" H 1650 600 50  0001 C CNN
F 1 "+5V" H 1650 890 50  0000 C CNN
F 2 "" H 1650 750 50  0000 C CNN
F 3 "" H 1650 750 50  0000 C CNN
	1    1650 750 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 5850BD2C
P 5200 750
F 0 "#PWR02" H 5200 600 50  0001 C CNN
F 1 "+3.3V" H 5200 890 50  0000 C CNN
F 2 "" H 5200 750 50  0000 C CNN
F 3 "" H 5200 750 50  0000 C CNN
	1    5200 750 
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 5850BD32
P 5500 750
F 0 "#PWR03" H 5500 600 50  0001 C CNN
F 1 "+5V" H 5500 890 50  0000 C CNN
F 2 "" H 5500 750 50  0000 C CNN
F 3 "" H 5500 750 50  0000 C CNN
	1    5500 750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5850BDA8
P 2000 3350
F 0 "#PWR04" H 2000 3100 50  0001 C CNN
F 1 "GND" H 2000 3200 50  0000 C CNN
F 2 "" H 2000 3350 50  0000 C CNN
F 3 "" H 2000 3350 50  0000 C CNN
	1    2000 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 5850BDDA
P 5500 3600
F 0 "#PWR05" H 5500 3350 50  0001 C CNN
F 1 "GND" H 5500 3450 50  0000 C CNN
F 2 "" H 5500 3600 50  0000 C CNN
F 3 "" H 5500 3600 50  0000 C CNN
	1    5500 3600
	1    0    0    -1  
$EndComp
NoConn ~ 4600 1950
NoConn ~ 4600 1850
NoConn ~ 4600 1750
NoConn ~ 4600 1650
NoConn ~ 6150 1950
NoConn ~ 6150 1850
NoConn ~ 6150 1750
NoConn ~ 6150 1650
Text Label 6250 2150 0    60   ~ 0
USB_SS
Text Label 6250 2250 0    60   ~ 0
MOSI
Text Label 6250 2350 0    60   ~ 0
MISO
Text Label 2600 2350 0    60   ~ 0
SCK
Text Label 2600 2450 0    60   ~ 0
MISO
Text Label 2600 2550 0    60   ~ 0
MOSI
NoConn ~ 1100 1550
NoConn ~ 1100 1650
Text Label 2600 2250 0    60   ~ 0
FLASH_SS
Text Label 2600 2650 0    60   ~ 0
USB_SS
NoConn ~ 4600 2650
NoConn ~ 4600 2750
NoConn ~ 4600 2850
Text Label 6250 1250 0    60   ~ 0
GO0
Text Label 6250 1350 0    60   ~ 0
GO1
Text Label 6250 1450 0    60   ~ 0
GO2
Text Label 6250 1550 0    60   ~ 0
GO3
Text Label 4500 1250 2    60   ~ 0
GI0
Text Label 4500 1350 2    60   ~ 0
GI1
Text Label 4500 1450 2    60   ~ 0
GI2
$Comp
L IS25LQ U2
U 1 1 5853342B
P 8200 1600
F 0 "U2" H 8650 1100 60  0000 C CNN
F 1 "IS25LQ" V 8200 1600 60  0000 C CNN
F 2 "SMD_Packages:SOIC-8-N" H 8200 1600 60  0001 C CNN
F 3 "" H 8200 1600 60  0001 C CNN
	1    8200 1600
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 585334C8
P 7450 1300
F 0 "R1" V 7530 1300 50  0000 C CNN
F 1 "4k7" V 7450 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7380 1300 50  0001 C CNN
F 3 "" H 7450 1300 50  0000 C CNN
	1    7450 1300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 5853355D
P 7450 1050
F 0 "#PWR06" H 7450 900 50  0001 C CNN
F 1 "+3.3V" H 7450 1190 50  0000 C CNN
F 2 "" H 7450 1050 50  0000 C CNN
F 3 "" H 7450 1050 50  0000 C CNN
	1    7450 1050
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5853371F
P 7300 1400
F 0 "R2" V 7380 1400 50  0000 C CNN
F 1 "4k7" V 7300 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7230 1400 50  0001 C CNN
F 3 "" H 7300 1400 50  0000 C CNN
	1    7300 1400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 58533726
P 7300 1150
F 0 "#PWR07" H 7300 1000 50  0001 C CNN
F 1 "+3.3V" H 7300 1290 50  0000 C CNN
F 2 "" H 7300 1150 50  0000 C CNN
F 3 "" H 7300 1150 50  0000 C CNN
	1    7300 1150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 585337C3
P 8200 2350
F 0 "#PWR08" H 8200 2100 50  0001 C CNN
F 1 "GND" H 8200 2200 50  0000 C CNN
F 2 "" H 8200 2350 50  0000 C CNN
F 3 "" H 8200 2350 50  0000 C CNN
	1    8200 2350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 585337DD
P 8200 850
F 0 "#PWR09" H 8200 700 50  0001 C CNN
F 1 "+3.3V" H 8200 990 50  0000 C CNN
F 2 "" H 8200 850 50  0000 C CNN
F 3 "" H 8200 850 50  0000 C CNN
	1    8200 850 
	1    0    0    -1  
$EndComp
Text Label 9000 1450 0    60   ~ 0
MOSI
Text Label 9000 1550 0    60   ~ 0
MISO
Text Label 9000 1650 0    60   ~ 0
SCK
Text Label 9000 1750 0    60   ~ 0
FLASH_SS
Wire Wire Line
	1700 3250 1700 3150
Wire Wire Line
	1700 3250 2000 3250
Wire Wire Line
	1900 3250 1900 3150
Wire Wire Line
	1800 3150 1800 3250
Connection ~ 1800 3250
Wire Wire Line
	2000 3250 2000 3350
Connection ~ 1900 3250
Wire Wire Line
	5300 3500 5500 3500
Wire Wire Line
	5500 3500 5500 3600
Wire Wire Line
	6150 2150 6250 2150
Wire Wire Line
	6150 2250 6250 2250
Wire Wire Line
	6150 2350 6250 2350
Wire Wire Line
	6150 2450 6250 2450
Wire Wire Line
	2500 2350 2600 2350
Wire Wire Line
	2500 2450 2600 2450
Wire Wire Line
	2500 2550 2600 2550
Wire Wire Line
	2500 2250 2600 2250
Wire Wire Line
	2500 2650 2600 2650
Wire Wire Line
	6150 1250 6250 1250
Wire Wire Line
	6150 1350 6250 1350
Wire Wire Line
	6150 1450 6250 1450
Wire Wire Line
	6150 1550 6250 1550
Wire Wire Line
	4600 1250 4500 1250
Wire Wire Line
	4600 1350 4500 1350
Wire Wire Line
	4600 1450 4500 1450
Wire Wire Line
	7550 1550 7450 1550
Wire Wire Line
	7450 1550 7450 1450
Wire Wire Line
	7450 1150 7450 1050
Wire Wire Line
	7300 1650 7550 1650
Wire Wire Line
	7300 1650 7300 1550
Wire Wire Line
	7300 1250 7300 1150
Wire Wire Line
	8200 950  8200 850 
Wire Wire Line
	8200 2250 8200 2350
Wire Wire Line
	8900 1450 9000 1450
Wire Wire Line
	8900 1550 9000 1550
Wire Wire Line
	8900 1650 9000 1650
Wire Wire Line
	8900 1750 9000 1750
Wire Wire Line
	1100 2350 1000 2350
Wire Wire Line
	1100 4150 1100 4250
$Comp
L GND #PWR011
U 1 1 585373B0
P 1100 4250
F 0 "#PWR011" H 1100 4000 50  0001 C CNN
F 1 "GND" H 1100 4100 50  0000 C CNN
F 2 "" H 1100 4250 50  0000 C CNN
F 3 "" H 1100 4250 50  0000 C CNN
	1    1100 4250
	1    0    0    -1  
$EndComp
Text Label 3200 4200 2    60   ~ 0
GO0
Text Label 3200 4300 2    60   ~ 0
GO1
Text Label 3200 4400 2    60   ~ 0
GO2
Text Label 3200 4500 2    60   ~ 0
GO3
Text Label 3200 4600 2    60   ~ 0
GI0
Text Label 3200 4700 2    60   ~ 0
GI1
Text Label 3200 4800 2    60   ~ 0
GI2
NoConn ~ 4600 2450
Wire Wire Line
	3200 4200 3300 4200
Wire Wire Line
	3200 4300 3300 4300
Wire Wire Line
	3200 4400 3300 4400
Wire Wire Line
	3200 4500 3300 4500
Wire Wire Line
	3200 4600 3300 4600
Wire Wire Line
	3200 4700 3300 4700
Wire Wire Line
	3200 4800 3300 4800
Wire Wire Line
	1100 4150 1650 4150
NoConn ~ 2500 2050
NoConn ~ 2500 2150
NoConn ~ 1100 2450
Wire Wire Line
	4600 2250 4500 2250
Text Label 4500 2250 2    60   ~ 0
RST
Wire Wire Line
	2500 1750 2950 1750
Text Label 2650 1750 0    60   ~ 0
RST
$Comp
L SW_PUSH SW13
U 1 1 58537C37
P 3250 1750
F 0 "SW13" H 3400 1860 50  0000 C CNN
F 1 "SW_PUSH" H 3250 1670 50  0000 C CNN
F 2 "components:SW_TACTILE_THT" H 3250 1750 50  0001 C CNN
F 3 "" H 3250 1750 50  0000 C CNN
	1    3250 1750
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 585381F6
P 2600 1500
F 0 "R3" V 2680 1500 50  0000 C CNN
F 1 "4k7" V 2600 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2530 1500 50  0001 C CNN
F 3 "" H 2600 1500 50  0000 C CNN
	1    2600 1500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR012
U 1 1 58538440
P 2600 1250
F 0 "#PWR012" H 2600 1100 50  0001 C CNN
F 1 "+3.3V" H 2600 1390 50  0000 C CNN
F 2 "" H 2600 1250 50  0000 C CNN
F 3 "" H 2600 1250 50  0000 C CNN
	1    2600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1350 2600 1250
$Comp
L GND #PWR013
U 1 1 5853855A
P 3550 1850
F 0 "#PWR013" H 3550 1600 50  0001 C CNN
F 1 "GND" H 3550 1700 50  0000 C CNN
F 2 "" H 3550 1850 50  0000 C CNN
F 3 "" H 3550 1850 50  0000 C CNN
	1    3550 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1650 2600 1750
Connection ~ 2600 1750
Wire Wire Line
	3550 1750 3550 1850
$Comp
L PWR_FLAG #FLG014
U 1 1 5853B3CD
P 1650 850
F 0 "#FLG014" H 1650 945 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 1030 50  0001 C CNN
F 2 "" H 1650 850 50  0000 C CNN
F 3 "" H 1650 850 50  0000 C CNN
	1    1650 850 
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG015
U 1 1 5853BD6B
P 2100 3300
F 0 "#FLG015" H 2100 3395 50  0001 C CNN
F 1 "PWR_FLAG" H 2100 3480 50  0001 C CNN
F 2 "" H 2100 3300 50  0000 C CNN
F 3 "" H 2100 3300 50  0000 C CNN
	1    2100 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 3300 2000 3300
Connection ~ 2000 3300
NoConn ~ 4600 1550
Text Label 6250 2450 0    60   ~ 0
SCK
$Comp
L +3.3V #PWR016
U 1 1 58538B12
P 1950 750
F 0 "#PWR016" H 1950 600 50  0001 C CNN
F 1 "+3.3V" H 1950 890 50  0000 C CNN
F 2 "" H 1950 750 50  0000 C CNN
F 3 "" H 1950 750 50  0000 C CNN
	1    1950 750 
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG017
U 1 1 58539BA1
P 1950 850
F 0 "#FLG017" H 1950 945 50  0001 C CNN
F 1 "PWR_FLAG" H 1950 1030 50  0001 C CNN
F 2 "" H 1950 850 50  0000 C CNN
F 3 "" H 1950 850 50  0000 C CNN
	1    1950 850 
	0    1    1    0   
$EndComp
$Comp
L LEO_PRO_MICRO U1
U 1 1 586533E8
P 1800 2100
F 0 "U1" H 2200 1200 60  0000 C CNN
F 1 "LEO_PRO_MICRO" V 1800 2100 60  0000 C CNN
F 2 "components:LEO_PRO_MICRO" H 1800 2100 60  0001 C CNN
F 3 "" H 1800 2100 60  0001 C CNN
	1    1800 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 1050 1750 950 
Wire Wire Line
	1750 950  1650 950 
Wire Wire Line
	1650 950  1650 750 
Connection ~ 1650 850 
Wire Wire Line
	1850 1050 1850 950 
Wire Wire Line
	1850 950  1950 950 
Wire Wire Line
	1950 950  1950 750 
Connection ~ 1950 850 
Wire Wire Line
	5300 950  5300 850 
Wire Wire Line
	5300 850  5200 850 
Wire Wire Line
	5200 850  5200 750 
Wire Wire Line
	5400 950  5400 850 
Wire Wire Line
	5400 850  5500 850 
Wire Wire Line
	5500 850  5500 750 
Wire Wire Line
	5400 3500 5400 3400
Wire Wire Line
	5300 3500 5300 3400
Connection ~ 5400 3500
$Comp
L USB_HOST_SHIELD_MINI U3
U 1 1 586A6D83
P 5350 2150
F 0 "U3" H 5900 1050 60  0000 C CNN
F 1 "USB_HOST_SHIELD_MINI" V 5350 2150 60  0000 C CNN
F 2 "components:UHS_MINI" H 5450 2150 60  0001 C CNN
F 3 "" H 5450 2150 60  0001 C CNN
	1    5350 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2350 4500 2350
Text Label 4500 2350 2    60   ~ 0
USB_INT
Wire Wire Line
	1100 2650 1000 2650
Text Label 1000 2650 2    60   ~ 0
USB_INT
NoConn ~ 1100 1950
NoConn ~ 1100 2550
Text Label 1000 2050 2    60   ~ 0
LED_B
Wire Wire Line
	1000 2050 1100 2050
Wire Wire Line
	1000 2250 1100 2250
Text Label 1000 2350 2    60   ~ 0
LED_R
Text Label 1000 2250 2    60   ~ 0
LED_G
NoConn ~ 1100 2150
$Comp
L CONN_01X05 P1
U 1 1 586C5FA9
P 1850 4350
F 0 "P1" H 1850 4650 50  0000 C CNN
F 1 "CONN_01X05" V 1950 4350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05" H 1850 4350 50  0001 C CNN
F 3 "" H 1850 4350 50  0000 C CNN
	1    1850 4350
	1    0    0    1   
$EndComp
Wire Wire Line
	1650 4250 1550 4250
Text Label 1550 4450 2    60   ~ 0
LED_B
Wire Wire Line
	1550 4450 1650 4450
Wire Wire Line
	1550 4350 1650 4350
Text Label 1550 4250 2    60   ~ 0
LED_R
Text Label 1550 4350 2    60   ~ 0
LED_G
NoConn ~ 2500 1950
$Comp
L CONN_01X07 P2
U 1 1 586C62D1
P 3500 4500
F 0 "P2" H 3500 4900 50  0000 C CNN
F 1 "CONN_01X07" V 3600 4500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x07" H 3500 4500 50  0001 C CNN
F 3 "" H 3500 4500 50  0000 C CNN
	1    3500 4500
	1    0    0    1   
$EndComp
NoConn ~ 1650 4550
Text Notes 650  4950 0    60   ~ 0
Awkward layout of this connector is\nfor compatibility with already laid-out\nmating "key" board.
$EndSCHEMATC
