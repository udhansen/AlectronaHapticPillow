#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9 // d1
#define OLED_CLK   10 // d0
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// WiFi credentials
const char* ssid       = "MY-NETWORK";
const char* password   = "123456";

// NTP and time configuration
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;  // GMT+1 for Denmark (Central European Time)
const int   daylightOffset_sec = 3600;  // Daylight savings offset (1 hour)

// Function to print the current local time to the serial monitor
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  
  // Print to Serial Monitor
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

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
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for most OLEDs
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

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
}

void loop() {
  // Print and display the local time every second
  printLocalTime();
  delay(1000);
}