#include "removeAlarms.h"

// Function to remove an alarm
bool removeAlarm(String alarmToRemove) {
  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i] && alarmTimes[i] == alarmToRemove) {
      alarmTimes[i] = "";
      alarmSet[i] = false;
      return true;
    }
  }
  return false; // Alarm not found
}