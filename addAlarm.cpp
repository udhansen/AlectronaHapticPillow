#include "addAlarm.h"

String alarmTimes[maxAlarms];
bool alarmSet[maxAlarms]={false};

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
