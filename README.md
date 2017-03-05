# PowerSupplyMonitor

This program displays Voltage, Current, Current Limit Setting, and Watts for a power supply.

The major hardware components are an Arduino Nano, ADS1115 ADC, and a 16x2 LCD with I2C backpack (see scemantic)

Libraries used are 

#include <LiquidCrystal_I2C.h>   // from [here](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home)
#include <Wire.h>                // Standard Arduino Library
#include <Adafruit_ADS1015.h>    // Must use the soligen2010 fork [here](https://github.com/soligen2010/Adafruit_ADS1X15)
#include <PWM.h>                 // From [here](https://code.google.com/archive/p/arduino-pwm-frequency-library/downloads)


Released under GPL V3

Copyright 2017 Dennis Cabell
