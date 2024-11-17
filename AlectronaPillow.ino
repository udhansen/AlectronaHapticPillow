// Project: Alectrona Pillow
// Coded by: Ulrik Hansen
// Date: 17 november 2024

// Libraries
#include <ArduinoBLE.h>              // Bluetooth library
#include <Adafruit_GFX.h>            // Core graphics library
#include <Adafruit_SSD1306.h>        // OLED display library
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

// Alarm functions & definitions
#include "addAlarm.h"
#include "removeAlarms.h"
#include "checkAlarms.h"
#include "isValidTimeFormat.h"
#include "listAlarms.h"

const int maxAlarms = 5;  // Maximum number of alarms
String alarmTimes[maxAlarms]; // Array to store alarm times
bool alarmSet[maxAlarms] = {false}; // Flags to check if each alarm is set

// Clock functions & Definitions
// NTP and time configuration
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;  // GMT+1 for Denmark (Central European Time)
const int   daylightOffset_sec = 3600;  // Daylight savings offset (1 hour)

// Display functions & definitions
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_MOSI   9 // d1
#define OLED_CLK    10 // d0
#define OLED_DC     11
#define OLED_CS     12
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#include "displayBluetoothSymbol.h"

// BLE Service and Characteristic UUIDs
BLEService alarmService("180A");
BLEStringCharacteristic alarmTimeCharacteristic("2A56", BLERead | BLEWrite, 50);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

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
}

void loop() {
    // Check for input from Serial Terminal
    if (Serial.available()) {
        char input = Serial.read();
        if (input == 'r') {
            resetBLE();
        }
    }

    printLocalTime();
    delay(1000);
    
}

