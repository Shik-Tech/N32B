EESchema Schematic File Version 4
LIBS:N32B-Top-panel-cache
EELAYER 29 0
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
L Mechanical:MountingHole_Pad H1
U 1 1 5F306011
P 4350 2900
F 0 "H1" H 4450 2949 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 2858 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 2900 50  0001 C CNN
F 3 "~" H 4350 2900 50  0001 C CNN
	1    4350 2900
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5F306417
P 4350 3250
F 0 "H2" H 4450 3299 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 3208 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 3250 50  0001 C CNN
F 3 "~" H 4350 3250 50  0001 C CNN
	1    4350 3250
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5F307010
P 4350 3650
F 0 "H3" H 4450 3699 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 3608 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 3650 50  0001 C CNN
F 3 "~" H 4350 3650 50  0001 C CNN
	1    4350 3650
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5F307464
P 4350 4000
F 0 "H4" H 4450 4049 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 3958 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 4000 50  0001 C CNN
F 3 "~" H 4350 4000 50  0001 C CNN
	1    4350 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5F307A66
P 3700 3400
F 0 "#PWR01" H 3700 3150 50  0001 C CNN
F 1 "GND" H 3705 3227 50  0000 C CNN
F 2 "" H 3700 3400 50  0001 C CNN
F 3 "" H 3700 3400 50  0001 C CNN
	1    3700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3000 4200 3000
Wire Wire Line
	3700 3000 3700 3400
Wire Wire Line
	4350 3350 4200 3350
Wire Wire Line
	4200 3350 4200 3000
Connection ~ 4200 3000
Wire Wire Line
	4200 3000 3700 3000
Wire Wire Line
	4350 3750 4200 3750
Wire Wire Line
	4200 3750 4200 3350
Connection ~ 4200 3350
Wire Wire Line
	4350 4100 4200 4100
Wire Wire Line
	4200 4100 4200 3750
Connection ~ 4200 3750
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 5F5D091E
P 4350 4400
F 0 "H5" H 4450 4449 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 4358 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 4400 50  0001 C CNN
F 3 "~" H 4350 4400 50  0001 C CNN
	1    4350 4400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 5F5D0DF7
P 4350 4800
F 0 "H6" H 4450 4849 50  0000 L CNN
F 1 "MountingHole_Pad" H 4450 4758 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 4350 4800 50  0001 C CNN
F 3 "~" H 4350 4800 50  0001 C CNN
	1    4350 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4100 4200 4500
Wire Wire Line
	4200 4500 4350 4500
Connection ~ 4200 4100
Wire Wire Line
	4200 4500 4200 4900
Wire Wire Line
	4200 4900 4350 4900
Connection ~ 4200 4500
$EndSCHEMATC
