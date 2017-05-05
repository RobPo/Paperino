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
LIBS:paperino_breakout-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Paperino breakout"
Date ""
Rev "0"
Comp ""
Comment1 "licensed under CC-BY-4.0"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L paperino_module A1
U 1 1 58E4DACE
P 7650 4000
F 0 "A1" H 7300 4500 60  0000 C CNN
F 1 "paperino_module" H 7650 3600 60  0000 C CNN
F 2 "Paperino:paperino_module" H 7400 3950 60  0001 C CNN
F 3 "" H 7400 3950 60  0001 C CNN
	1    7650 4000
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M1
U 1 1 58E4DB75
P 5800 7450
F 0 "M1" H 5800 7300 60  0000 C CNN
F 1 "Mounting_hole" H 5800 7600 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 5800 7450 60  0001 C CNN
F 3 "" H 5800 7450 60  0001 C CNN
	1    5800 7450
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M2
U 1 1 58E4DC10
P 6050 7450
F 0 "M2" H 6050 7300 60  0000 C CNN
F 1 "Mounting_hole" H 6050 7600 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6050 7450 60  0001 C CNN
F 3 "" H 6050 7450 60  0001 C CNN
	1    6050 7450
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M3
U 1 1 58E4DC2F
P 6300 7450
F 0 "M3" H 6300 7300 60  0000 C CNN
F 1 "Mounting_hole" H 6300 7600 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6300 7450 60  0001 C CNN
F 3 "" H 6300 7450 60  0001 C CNN
	1    6300 7450
	1    0    0    -1  
$EndComp
$Comp
L Mounting_hole M4
U 1 1 58E4DC51
P 6550 7450
F 0 "M4" H 6550 7300 60  0000 C CNN
F 1 "Mounting_hole" H 6550 7600 60  0001 C CNN
F 2 "Paperino:mounting_hole_1.5" H 6550 7450 60  0001 C CNN
F 3 "" H 6550 7450 60  0001 C CNN
	1    6550 7450
	1    0    0    -1  
$EndComp
Text Notes 5700 7250 0    60   ~ 0
mounting holes
$Comp
L CONN_01X12 X1
U 1 1 58E4E002
P 3400 4050
F 0 "X1" H 3400 4700 50  0000 C CNN
F 1 "CONN_01X12" V 3500 4050 50  0001 C CNN
F 2 "Paperino:header_1x12_SMT" H 3400 4050 50  0001 C CNN
F 3 "" H 3400 4050 50  0001 C CNN
	1    3400 4050
	-1   0    0    -1  
$EndComp
NoConn ~ 7050 3950
$Comp
L GND #PWR01
U 1 1 58E5F444
P 6650 4050
F 0 "#PWR01" H 6650 3800 50  0001 C CNN
F 1 "GND" H 6650 3900 50  0000 C CNN
F 2 "" H 6650 4050 50  0001 C CNN
F 3 "" H 6650 4050 50  0001 C CNN
	1    6650 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3850 6650 3850
Wire Wire Line
	6650 3850 6650 4050
Text Label 6800 4050 0    60   ~ 0
CS2
Text Label 6800 4150 0    60   ~ 0
INT2
Text Label 6800 4250 0    60   ~ 0
INT1
Text Label 8450 4250 0    60   ~ 0
MISO
Text Label 8450 4150 0    60   ~ 0
MOSI
Text Label 8450 4050 0    60   ~ 0
CLK
Text Label 8450 3950 0    60   ~ 0
CS1
Text Label 8450 3850 0    60   ~ 0
~RST
Text Label 8450 3750 0    60   ~ 0
~BSY
Wire Wire Line
	8300 3750 8450 3750
Wire Wire Line
	8300 3850 8450 3850
Wire Wire Line
	8450 3950 8300 3950
Wire Wire Line
	8300 4050 8450 4050
Wire Wire Line
	8450 4150 8300 4150
Wire Wire Line
	8300 4250 8450 4250
Wire Wire Line
	6800 4050 7050 4050
Wire Wire Line
	7050 4150 6800 4150
Wire Wire Line
	6800 4250 7050 4250
$Comp
L +3V3 #PWR02
U 1 1 58E5F999
P 6650 3700
F 0 "#PWR02" H 6650 3550 50  0001 C CNN
F 1 "+3V3" H 6650 3840 50  0000 C CNN
F 2 "" H 6650 3700 50  0001 C CNN
F 3 "" H 6650 3700 50  0001 C CNN
	1    6650 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3700 6650 3750
Wire Wire Line
	6650 3750 7050 3750
$Comp
L +3V3 #PWR03
U 1 1 58E5FC08
P 3700 4000
F 0 "#PWR03" H 3700 3850 50  0001 C CNN
F 1 "+3V3" H 3700 4140 50  0000 C CNN
F 2 "" H 3700 4000 50  0001 C CNN
F 3 "" H 3700 4000 50  0001 C CNN
	1    3700 4000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR04
U 1 1 58E5FC25
P 3850 3900
F 0 "#PWR04" H 3850 3650 50  0001 C CNN
F 1 "GND" H 3850 3750 50  0000 C CNN
F 2 "" H 3850 3900 50  0001 C CNN
F 3 "" H 3850 3900 50  0001 C CNN
	1    3850 3900
	0    -1   -1   0   
$EndComp
Text Label 3900 3700 0    60   ~ 0
CS2
Text Label 3900 3600 0    60   ~ 0
INT2
Text Label 3900 3500 0    60   ~ 0
INT1
Text Label 3900 4600 0    60   ~ 0
MISO
Text Label 3900 4500 0    60   ~ 0
MOSI
Text Label 3900 4400 0    60   ~ 0
CLK
Text Label 3900 4300 0    60   ~ 0
CS1
Text Label 3900 4200 0    60   ~ 0
~RST
Text Label 3900 4100 0    60   ~ 0
~BSY
NoConn ~ 3600 3800
Wire Wire Line
	3600 3500 3900 3500
Wire Wire Line
	3600 3600 3900 3600
Wire Wire Line
	3600 3700 3900 3700
Wire Wire Line
	3600 4100 3900 4100
Wire Wire Line
	3600 4200 3900 4200
Wire Wire Line
	3600 4300 3900 4300
Wire Wire Line
	3600 4400 3900 4400
Wire Wire Line
	3600 4500 3900 4500
Wire Wire Line
	3600 4600 3900 4600
Wire Wire Line
	3600 4000 3700 4000
Wire Wire Line
	3600 3900 3850 3900
$EndSCHEMATC
