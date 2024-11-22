#ifndef UPDATEALARMCHARACTERISTIC_H
#define UPDATEALARMCHARACTERISTIC_H

#include "addAlarm.h"
#include <Arduino.h>
#include <ArduinoBLE.h>

extern BLEStringCharacteristic alarmTimeCharacteristic;

void updateAlarmCharacteristic();

#endif