#ifndef CHECKALARMS_H
#define CHECKALARMS_H

#include <Arduino.h>
#include <time.h>   
#include "addAlarm.h"
#include "activateMotor.h"

// Function declarations
String getCurrentTimeString();
void checkAlarms();

#endif