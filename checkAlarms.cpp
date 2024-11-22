#include "checkAlarms.h"

// Function to get the current time as a string in HH:MM format
String getCurrentTimeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to get local time");
        return "";
    }
    
    // Format the current time as HH:MM
    char timeString[6]; // Buffer to hold the formatted time
    snprintf(timeString, sizeof(timeString), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    return String(timeString);
}

// Function to check all alarms against the current time
void checkAlarms() {
    String currentTime = getCurrentTimeString();
    if (currentTime == "") return; // Exit if time is unavailable

    for (int i = 0; i < maxAlarms; i++) {
        if (alarmSet[i] && alarmTimes[i] == currentTime) {
            Serial.println("Alarm triggered: " + alarmTimes[i]);
            alarmSet[i] = false; // Reset the alarm after activation
            activateMotor();
        }
    }
}


