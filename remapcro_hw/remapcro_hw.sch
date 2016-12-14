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
LIBS:remapcro_hw-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
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
L USB_HOST_SHIELD_MINI U?
U 1 1 5850BC37
P 5650 3100
F 0 "U?" H 6200 2000 60  0000 C CNN
F 1 "USB_HOST_SHIELD_MINI" V 5650 3100 60  0000 C CNN
F 2 "" H 5750 3100 60  0001 C CNN
F 3 "" H 5750 3100 60  0001 C CNN
	1    5650 3100
	1    0    0    -1  
$EndComp
$Comp
L LEO_PRO_MICRO U?
U 1 1 5850BC67
P 2150 3000
F 0 "U?" H 2550 2100 60  0000 C CNN
F 1 "LEO_PRO_MICRO" V 2150 3000 60  0000 C CNN
F 2 "components:LEO_PRO_MICRO" H 2150 3000 60  0001 C CNN
F 3 "" H 2150 3000 60  0001 C CNN
	1    2150 3000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5850BCB6
P 2200 1850
F 0 "#PWR?" H 2200 1700 50  0001 C CNN
F 1 "+3.3V" H 2200 1990 50  0000 C CNN
F 2 "" H 2200 1850 50  0000 C CNN
F 3 "" H 2200 1850 50  0000 C CNN
	1    2200 1850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5850BCCE
P 2100 1600
F 0 "#PWR?" H 2100 1450 50  0001 C CNN
F 1 "+5V" H 2100 1740 50  0000 C CNN
F 2 "" H 2100 1600 50  0000 C CNN
F 3 "" H 2100 1600 50  0000 C CNN
	1    2100 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 1950 2200 1850
Wire Wire Line
	2100 1950 2100 1600
$Comp
L +3.3V #PWR?
U 1 1 5850BD2C
P 5600 1800
F 0 "#PWR?" H 5600 1650 50  0001 C CNN
F 1 "+3.3V" H 5600 1940 50  0000 C CNN
F 2 "" H 5600 1800 50  0000 C CNN
F 3 "" H 5600 1800 50  0000 C CNN
	1    5600 1800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5850BD32
P 5700 1550
F 0 "#PWR?" H 5700 1400 50  0001 C CNN
F 1 "+5V" H 5700 1690 50  0000 C CNN
F 2 "" H 5700 1550 50  0000 C CNN
F 3 "" H 5700 1550 50  0000 C CNN
	1    5700 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1900 5600 1800
Wire Wire Line
	5700 1900 5700 1550
Wire Wire Line
	2050 4150 2050 4050
Wire Wire Line
	2050 4150 2350 4150
Wire Wire Line
	2250 4150 2250 4050
Wire Wire Line
	2150 4050 2150 4150
Connection ~ 2150 4150
Wire Wire Line
	2350 4150 2350 4250
Connection ~ 2250 4150
$Comp
L GND #PWR?
U 1 1 5850BDA8
P 2350 4250
F 0 "#PWR?" H 2350 4000 50  0001 C CNN
F 1 "GND" H 2350 4100 50  0000 C CNN
F 2 "" H 2350 4250 50  0000 C CNN
F 3 "" H 2350 4250 50  0000 C CNN
	1    2350 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 4450 5550 4350
Wire Wire Line
	5550 4450 5850 4450
Wire Wire Line
	5750 4450 5750 4350
Wire Wire Line
	5650 4350 5650 4450
Connection ~ 5650 4450
Wire Wire Line
	5850 4450 5850 4550
Connection ~ 5750 4450
$Comp
L GND #PWR?
U 1 1 5850BDDA
P 5850 4550
F 0 "#PWR?" H 5850 4300 50  0001 C CNN
F 1 "GND" H 5850 4400 50  0000 C CNN
F 2 "" H 5850 4550 50  0000 C CNN
F 3 "" H 5850 4550 50  0000 C CNN
	1    5850 4550
	1    0    0    -1  
$EndComp
NoConn ~ 4900 2900
NoConn ~ 4900 2800
NoConn ~ 4900 2700
NoConn ~ 4900 2600
NoConn ~ 6450 2900
NoConn ~ 6450 2800
NoConn ~ 6450 2700
NoConn ~ 6450 2600
Wire Wire Line
	6450 3100 6550 3100
Wire Wire Line
	6450 3200 6550 3200
Wire Wire Line
	6450 3300 6550 3300
Wire Wire Line
	6450 3400 6550 3400
Text Label 6550 3100 0    60   ~ 0
USB_SS
Text Label 6550 3200 0    60   ~ 0
MOSI
Text Label 6550 3300 0    60   ~ 0
MISO
Text Label 6550 3400 0    60   ~ 0
SCK
Wire Wire Line
	2850 3250 2950 3250
Wire Wire Line
	2850 3350 2950 3350
Wire Wire Line
	2850 3450 2950 3450
Text Label 2950 3250 0    60   ~ 0
SCK
Text Label 2950 3350 0    60   ~ 0
MISO
Text Label 2950 3450 0    60   ~ 0
MOSI
NoConn ~ 1450 2450
NoConn ~ 1450 2550
Wire Wire Line
	1450 3150 1350 3150
Text Label 1350 3150 2    60   ~ 0
FLASH_SS
Wire Wire Line
	2850 3550 2950 3550
Text Label 2950 3550 0    60   ~ 0
USB_SS
$EndSCHEMATC
