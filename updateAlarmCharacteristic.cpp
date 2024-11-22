#include "updateAlarmCharacteristic.h"

void updateAlarmCharacteristic() {
  String alarmList = "Alarms: ";
  bool hasAlarms = false;

  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i]) {
      if (hasAlarms) {
        alarmList += ", "; // Add separator for multiple alarms
      }
      alarmList += alarmTimes[i];
      hasAlarms = true;
    }
  }

  if (!hasAlarms) {
    alarmList += "None";
  }

  // Update the BLE characteristic value
  alarmTimeCharacteristic.writeValue(alarmList);
}