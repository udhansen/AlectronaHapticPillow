#include "timeavailable.h"

extern Adafruit_SSD1306 display;  // Display object

void timeavailable(struct timeval *t) {
    Serial.println("Got time adjustment from NTP!");
    printLocalTime();
}