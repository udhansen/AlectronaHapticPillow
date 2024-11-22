#ifndef ADDALARM_H
#define ADDALARM_H

#include <Arduino.h>

const int maxAlarms = 5;          // Maximum number of alarms
extern String alarmTimes[maxAlarms];          // Array to store alarm times
extern bool alarmSet[maxAlarms];              // Array to store whether an alarm is set

bool addAlarm(String newAlarmTime);  // Function declaration

#endif
