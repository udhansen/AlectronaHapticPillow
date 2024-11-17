#include "listAlarms.h"

// Function to list all active alarms
void listAlarms() {
  Serial.println("Active Alarms:");
  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i]) {
      Serial.println("- " + alarmTimes[i]);
    }
  }
}