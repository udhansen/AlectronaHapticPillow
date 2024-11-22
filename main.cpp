// Project: Alectrona Pillow
// Coded by: Ulrik Hansen
// Date: 17 november 2024

// Libraries
#include <ArduinoBLE.h>              // Bluetooth library
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

// Alarm functions & definitions
#include "addAlarm.h"
#include "removeAlarms.h"
#include "checkAlarms.h"
#include "isValidTimeFormat.h"
#include "listAlarms.h"
#include "updateAlarmCharacteristic.h"
#include "activateMotor.h"

// Clock functions & Definitions
#include "printLocalTime.h"
#include "timeavailable.h"

// WiFi credentials
const char* ssid       = "network_2.4G";
const char* password   = "xXxXxXx";

char ledState = '0';                 // Assuming the command is received with '1'

// NTP and time configuration
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;  // GMT+1 for Denmark (Central European Time)
const int   daylightOffset_sec = 3600;  // Daylight savings offset (1 hour)

// Display functions & definitions
#include "displayConfig.h"

// BLE functions & definitions
// #include "displayBluetoothSymbol.h"
bool isAdvertising = true;
#include "resetBLE.h"

const int ledPin = LED_BUILTIN;  // Use the built-in LED (usually GPIO 2 on ESP32)

// BLE Service and Characteristic UUIDs
BLEService alarmService("180A");
BLEStringCharacteristic alarmTimeCharacteristic("2A56", BLERead | BLEWrite, 50);

void setup() {
  Serial.begin(115200);
  //while (!Serial);

  Serial.println("Starting...");
    
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Start with LED off

  // Initialize the display
  initDisplay();

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
       delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  ledState = '1';

  // Set the NTP time sync notification callback
  sntp_set_time_sync_notification_cb(timeavailable);

  // Configure time with NTP servers
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Give some time for the clock to sync
  delay(2000);

  // Initialize BLE
  if (!BLE.begin()) {
      Serial.println("Starting BLE failed!");
      while (1);
  }

  BLE.setLocalName("AlecTronaPillow");
  BLE.setAdvertisedService(alarmService);

  // Add characteristic to the service
  alarmService.addCharacteristic(alarmTimeCharacteristic);
  BLE.addService(alarmService);

  // Start advertising the BLE service
  BLE.advertise();
  Serial.println("AlectronaPillow is ready to connect. Use'r' to reset bonding.");

  displayMessage("Hello, Ulrik!");
  Serial.println("Hello, Ulrik!");
}

void loop() {
    // Always display the local time, even if no BLE connection exists
    printLocalTime();

    // Listen for BLE connection
    BLEDevice central = BLE.central();

    // If a central device is connected
    if (central) {
        Serial.println("Connected to central device");

        while (central.connected()) {
            // Continuously update the local time while connected
            printLocalTime();

            // Check if the characteristic was written
            if (alarmTimeCharacteristic.written()) {
                String input = alarmTimeCharacteristic.value();
                input.trim(); // Remove leading and trailing whitespace

                if (input.startsWith("ADD:")) {
                    String newAlarm = input.substring(4); // Extract time after "ADD:"
                    if (isValidTimeFormat(newAlarm)) {
                        if (addAlarm(newAlarm)) {
                            Serial.println("Added alarm: " + newAlarm);
                        } else {
                            Serial.println("Failed to add alarm. Alarm list may be full or alarm already exists.");
                        }
                    } else {
                        Serial.println("Invalid format. Use 'ADD:HH:MM'");
                    }
                } else if (input.startsWith("REMOVE:")) {
                    String alarmToRemove = input.substring(7); // Extract time after "REMOVE:"
                    if (isValidTimeFormat(alarmToRemove)) {
                        if (removeAlarm(alarmToRemove)) {
                            Serial.println("Removed alarm: " + alarmToRemove);
                        } else {
                            Serial.println("Alarm not found: " + alarmToRemove);
                        }
                    } else {
                        Serial.println("Invalid format. Use 'REMOVE:HH:MM'");
                    }
                } else if (input == "LIST") {
                    listAlarms(); // Call function to list all alarms
                } else {
                    Serial.println("Invalid command. Use 'ADD:HH:MM', 'REMOVE:HH:MM', or 'LIST'");
                }
            }

            updateAlarmCharacteristic();

            // Check if any alarms should trigger
            checkAlarms();

            delay(100); // Small delay to prevent CPU overload
        }

        Serial.println("Disconnected from central device");
    }
}
