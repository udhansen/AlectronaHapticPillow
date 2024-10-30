#include <ArduinoBLE.h>              // Bluetooth library
#include <Adafruit_GFX.h>            // Core graphics library
#include <Adafruit_SSD1306.h>        // OLED display library
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9 // d1
#define OLED_CLK    10 // d0
#define OLED_DC     11
#define OLED_CS     12
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// BLE setup
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");  // LED service UUID
BLEByteCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);  // LED characteristic UUID
const int ledPin = LED_BUILTIN;  // Use the built-in LED (usually GPIO 2 on ESP32)
bool isAdvertising = true;        // Track if advertising or connected

// WiFi credentials
const char* ssid       = "Hansen_wifi_network_2.4G";
const char* password   = "Rolf1998frede";

// NTP and time configuration
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;  // GMT+1 for Denmark (Central European Time)
const int   daylightOffset_sec = 3600;  // Daylight savings offset (1 hour)


// Function to print the current local time to the OLED display
void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        return;
    }
  
    // Clear the OLED display and show the current time
    display.clearDisplay();
    display.setTextSize(2);           // Set the text size
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
  
    // Format and display the time (hh:mm:ss)
    display.printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    display.display();
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
    Serial.println("Got time adjustment from NTP!");
    printLocalTime();
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);  // Start with LED off

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Connect to WiFi
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

    // Set the NTP time sync notification callback
    sntp_set_time_sync_notification_cb(timeavailable);

    // Configure time with NTP servers
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    // Give some time for the clock to sync
    delay(2000);

    // Initialize BLE
    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (true);
    }

    BLE.setLocalName("ULHN_LED_Control");   // Device name
    BLE.setAdvertisedService(ledService);    // Advertise the LED service
    BLE.setAdvertisingInterval(100);          // Set advertising interval to 100 ms

    ledService.addCharacteristic(ledCharacteristic);
    BLE.addService(ledService);
    BLE.advertise();

    Serial.println("BLE LED control ready. Use '1' to turn ON, '0' to turn OFF, and 'r' to reset bonding.");
}

void loop() {
    // Check for input from Serial Terminal
    if (Serial.available()) {
        char input = Serial.read();
        if (input == 'r') {
            resetBLE();
        }
    }

    // Check for BLE connection
    BLEDevice central = BLE.central();

    if (central) {
        isAdvertising = false;
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            if (ledCharacteristic.written()) {
                byte ledState = ledCharacteristic.value();
                if (ledState == '1') {
                    Serial.println("LED ON command received");
                    digitalWrite(ledPin, HIGH);  // Turn on the LED
                }
                else if (ledState == '0') {
                    Serial.println("LED OFF command received");
                    digitalWrite(ledPin, LOW);   // Turn off the LED
                }
            }
            printLocalTime();  // Update the display with local time while connected
            displayBluetoothSymbol(); // Show connected status
            delay(1000);       // Optional delay to prevent rapid updates
        }

        isAdvertising = true;          // Return to advertising state after disconnect
        BLE.advertise();
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    } else {
        printLocalTime();  // Update the display with local time
        displayBluetoothSymbol(); // Show advertising status
    }

    delay(1000);       // Update every second
}

void resetBLE() {
    Serial.println("Resetting BLE and clearing bonded devices...");
    BLE.stopAdvertise();  // Stop advertising
    BLE.disconnect();       // Disconnect all connected devices (if any)
    
    BLE.begin();  // Reinitialize BLE
    BLE.advertise();  // Start advertising again
}

void displayBluetoothSymbol() {
    // Set cursor position at the bottom of the display
    display.setCursor(0, 50);  
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Clear the area for the Bluetooth status text
    display.fillRect(0, 50, SCREEN_WIDTH, 14, SSD1306_BLACK); // Clear previous status

    if (isAdvertising) {
        display.print("BLE: Advertising");  // Show advertising status
    } else {
        display.print("BLE: Connected");    // Show connected status
    }
    
    display.display(); // Ensure the display is updated
}
