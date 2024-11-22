#include <ArduinoBLE.h>

// const int motorPin = 5; // Motor control pin
const int maxAlarms = 5;  // Maximum number of alarms
String alarmTimes[maxAlarms]; // Array to store alarm times
bool alarmSet[maxAlarms] = {false}; // Flags to check if each alarm is set

// Variables for software clock
unsigned long previousMillis = 0;
int currentHour = 0;
int currentMinute = 0;

// BLE Service and Characteristic UUIDs
BLEService alarmService("180A");
BLEStringCharacteristic alarmTimeCharacteristic("2A56", BLERead | BLEWrite, 50); // Increased size for multiple alarms

void setup() {
  Serial.begin(115200);
  // pinMode(motorPin, OUTPUT);
  // digitalWrite(motorPin, LOW);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("ESP32_AlarmClock");
  BLE.setAdvertisedService(alarmService);

  // Add characteristic to the service
  alarmService.addCharacteristic(alarmTimeCharacteristic);
  BLE.addService(alarmService);

  // Start advertising the BLE service
  BLE.advertise();
  Serial.println("BLE Alarm Clock is ready to connect");
}

void loop() {
  // Listen for BLE connection
  BLEDevice central = BLE.central();

  // If a central device is connected
  if (central) {
    Serial.println("Connected to central device");

    while (central.connected()) {
      // Check if the characteristic was written
      if (alarmTimeCharacteristic.written()) {
        String input = alarmTimeCharacteristic.value();
        input.trim();

        if (input.startsWith("ADD:")) {
          String newAlarm = input.substring(4);
          if (isValidTimeFormat(newAlarm)) {
            if (addAlarm(newAlarm)) {
              Serial.println("Added alarm: " + newAlarm);
            }
          } else {
            Serial.println("Invalid format. Use 'ADD:HH:MM'");
          }
        } else if (input.startsWith("REMOVE:")) {
          String alarmToRemove = input.substring(7);
          if (removeAlarm(alarmToRemove)) {
            Serial.println("Removed alarm: " + alarmToRemove);
          } else {
            Serial.println("Alarm not found: " + alarmToRemove);
          }
        } else if (input == "LIST") {
          listAlarms();
        } else {
          Serial.println("Invalid command. Use 'ADD:HH:MM', 'REMOVE:HH:MM', or 'LIST'");
        }
      }

      updateAlarmCharacteristic();

      // Update software clock every 60 seconds
      if (millis() - previousMillis >= 60000) {
        previousMillis = millis();
        incrementTime();
        Serial.println("Current Time: " + getCurrentTimeString());
      }

      // Check if any alarms should trigger
      checkAlarms();

      delay(100); // Small delay to prevent CPU overload
    }

    Serial.println("Disconnected from central device");
  }
}

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

// Function to list all active alarms
void listAlarms() {
  Serial.println("Active Alarms:");
  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i]) {
      Serial.println("- " + alarmTimes[i]);
    }
  }
}

// Function to check all alarms against the current time
void checkAlarms() {
  String currentTime = getCurrentTimeString();
  for (int i = 0; i < maxAlarms; i++) {
    if (alarmSet[i] && alarmTimes[i] == currentTime) {
      Serial.println("Alarm triggered: " + alarmTimes[i]);
      // activateMotor();
      alarmSet[i] = false; // Reset the alarm after activation
    }
  }
}

// Function to increment the software clock time
void incrementTime() {
  currentMinute++;
  if (currentMinute >= 60) {
    currentMinute = 0;
    currentHour++;
  }
  if (currentHour >= 24) {
    currentHour = 0;
  }
}

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


// Function to activate the motor
// void activateMotor() {
  // Serial.println("Alarm Activated! Wake up!");

//  digitalWrite(motorPin, HIGH); // Turn on the motor
//  delay(5000);                  // Keep the motor on for 5 seconds
//  digitalWrite(motorPin, LOW);  // Turn off the motor
// }

// Function to get the current time as a string in HH:MM format
String getCurrentTimeString() {
  String hourStr = (currentHour < 10) ? "0" + String(currentHour) : String(currentHour);
  String minuteStr = (currentMinute < 10) ? "0" + String(currentMinute) : String(currentMinute);
  return hourStr + ":" + minuteStr;
}

// Function to validate HH:MM time format
bool isValidTimeFormat(String timeStr) {
  if (timeStr.length() != 5 || timeStr.charAt(2) != ':') return false;

  String hours = timeStr.substring(0, 2);
  String minutes = timeStr.substring(3, 5);

  int hour = hours.toInt();
  int minute = minutes.toInt();

  return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59);
}
