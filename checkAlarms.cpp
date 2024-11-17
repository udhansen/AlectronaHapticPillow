#include "checkAlarms.h"

// Function to check all alarms against the current time
void checkAlarms() {
  String currentTime = getCurrentTimeString();
  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i] && alarmTimes[i] == currentTime) {
      Serial.println("Alarm triggered: " + alarmTimes[i]);
      alarmSet[i] = false; // Reset the alarm after activation
    }
  }
}