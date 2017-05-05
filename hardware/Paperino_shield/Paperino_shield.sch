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
LIBS:Paperino_shield-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Paperino shield"
Date ""
Rev "0"
Comp ""
Comment1 "licensed under CC-BY-4.0"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Photon A1
U 1 1 58D3965C
P 4450 4250
F 0 "A1" H 4200 5050 60  0000 C CNN
F 1 "Photon" H 4450 3500 60  0000 C CNN
F 2 "Paperino:photon_headers" H 4150 3800 60  0001 C CNN
F 3 "" H 4150 3800 60  0001 C CNN
	1    4450 4250
	1    0    0    -1  
$EndComp
$Comp
L paperino_module A2
U 1 1 58D39C7F
P 7750 4300
F 0 "A2" H 7400 4850 60  0000 C CNN
F 1 "paperino_module" H 7750 3900 60  0000 C CNN
F 2 "Paperino:paperino_module" H 7500 4250 60  0001 C CNN
F 3 "" H 7500 4250 60  0001 C CNN
	1    7750 4300
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR01
U 1 1 58D39ECD
P 5150 3600
F 0 "#PWR01" H 5150 3450 50  0001 C CNN
F 1 "+3V3" H 5150 3740 50  0000 C CNN
F 2 "" H 5150 3600 50  0001 C CNN
F 3 "" H 5150 3600 50  0001 C CNN
	1    5150 3600
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR02
U 1 1 58D39EF5
P 6650 4000
F 0 "#PWR02" H 6650 3850 50  0001 C CNN
F 1 "+3V3" H 6650 4140 50  0000 C CNN
F 2 "" H 6650 4000 50  0001 C CNN
F 3 "" H 6650 4000 50  0001 C CNN
	1    6650 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4050 6650 4050
Wire Wire Line
	6650 4050 6650 4000
Wire Wire Line
	4950 3700 5150 3700
Wire Wire Line
	5150 3700 5150 3600
$Comp
L GND #PWR03
U 1 1 58D39FDA
P 6650 4250
F 0 "#PWR03" H 6650 4000 50  0001 C CNN
F 1 "GND" H 6650 4100 50  0000 C CNN
F 2 "" H 6650 4250 50  0001 C CNN
F 3 "" H 6650 4250 50  0001 C CNN
	1    6650 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 58D3A06A
P 5550 4100
F 0 "#PWR04" H 5550 3850 50  0001 C CNN
F 1 "GND" H 5550 3950 50  0000 C CNN
F 2 "" H 5550 4100 50  0001 C CNN
F 3 "" H 5550 4100 50  0001 C CNN
	1    5550 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 58D3A07B
P 3300 3900
F 0 "#PWR05" H 3300 3650 50  0001 C CNN
F 1 "GND" H 3300 3750 50  0000 C CNN
F 2 "" H 3300 3900 50  0001 C CNN
F 3 "" H 3300 3900 50  0001 C CNN
	1    3300 3900
	1    0    0    -1  
$EndComp
NoConn ~ 7150 4250
Wire Wire Line
	7150 4150 6650 4150
Wire Wire Line
	6650 4150 6650 4250
Wire Wire Line
	4950 4000 5550 4000
Wire Wire Line
	5550 4000 5550 4100
Wire Wire Line
	3950 3800 3300 3800
Wire Wire Line
	3300 3800 3300 3900
Text Label 8550 4050 0    60   ~ 0
~BUSY
Text Label 8550 4150 0    60   ~ 0
~RST
Text Label 8550 4250 0    60   ~ 0
SPI_CS1
Text Label 8550 4350 0    60   ~ 0
SPI_CLK
Text Label 8550 4450 0    60   ~ 0
SPI_MOSI
Text Label 8550 4550 0    60   ~ 0
SPI_MISO
Text Label 6750 4350 0    60   ~ 0
SPI_CS2
Text Label 6750 4450 0    60   ~ 0
INT2
Text Label 6750 4550 0    60   ~ 0
INT1
Wire Wire Line
	6750 4350 7150 4350
Wire Wire Line
	7150 4450 6750 4450
Wire Wire Line
	6750 4550 7150 4550
Wire Wire Line
	8400 4050 8550 4050
Wire Wire Line
	8550 4150 8400 4150
Wire Wire Line
	8400 4250 8550 4250
Wire Wire Line
	8550 4350 8400 4350
Wire Wire Line
	8400 4450 8550 4450
Wire Wire Line
	8550 4550 8400 4550
Text Label 5050 4200 0    60   ~ 0
SPI_CS2
Wire Wire Line
	4950 4200 5050 4200
Text Label 3450 4800 0    60   ~ 0
~RST
Text Label 3450 4700 0    60   ~ 0
~BUSY
Text Label 3450 4300 0    60   ~ 0
SPI_MOSI
Text Label 3450 4400 0    60   ~ 0
SPI_MISO
Text Label 3450 4600 0    60   ~ 0
SPI_CS1
Text Label 3450 4500 0    60   ~ 0
SPI_CLK
Text Label 3450 4100 0    60   ~ 0
INT1
Wire Wire Line
	3450 4100 3950 4100
Wire Wire Line
	3450 4300 3950 4300
Wire Wire Line
	3950 4400 3450 4400
Wire Wire Line
	3450 4500 3950 4500
Wire Wire Line
	3950 4600 3450 4600
Wire Wire Line
	3450 4700 3950 4700
Wire Wire Line
	3950 4800 3450 4800
$Comp
L Mounting_hole M1
U 1 1 58D4C49F
P 5950 7500
F 0 "M1" H 5950 7350 60  0000 C CNN
F 1 "Mounting_hole" H 5950 7650 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 5950 7500 60  0001 C CNN
F 3 "" H 5950 7500 60  0001 C CNN
	1    5950 7500
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M2
U 1 1 58D4C672
P 6200 7500
F 0 "M2" H 6200 7350 60  0000 C CNN
F 1 "Mounting_hole" H 6200 7650 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6200 7500 60  0001 C CNN
F 3 "" H 6200 7500 60  0001 C CNN
	1    6200 7500
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M3
U 1 1 58D4C696
P 6450 7500
F 0 "M3" H 6450 7350 60  0000 C CNN
F 1 "Mounting_hole" H 6450 7650 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6450 7500 60  0001 C CNN
F 3 "" H 6450 7500 60  0001 C CNN
	1    6450 7500
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M4
U 1 1 58D4C6BB
P 6700 7500
F 0 "M4" H 6700 7350 60  0000 C CNN
F 1 "Mounting_hole" H 6700 7650 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6700 7500 60  0001 C CNN
F 3 "" H 6700 7500 60  0001 C CNN
	1    6700 7500
	1    0    0    -1  
$EndComp
Text Notes 5850 7350 0    60   ~ 0
Mounting holes
Text Label 5050 4300 0    60   ~ 0
INT2
Wire Wire Line
	5050 4300 4950 4300
$EndSCHEMATC
