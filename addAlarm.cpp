#include "addAlarm.h"

// Function to add a new alarm
bool addAlarm(String newAlarmTime) {
  for (int i = 0; i < maxAlarms; i++) {
    if (!alarmSet[i]) {
      alarmTimes[i] = newAlarmTime;
      alarmSet[i] = true;
      return true;
    }
  }
  Serial.println("No space for new alarms!");
  return false;
}