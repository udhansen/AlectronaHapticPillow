#include "printLocalTime.h"
#include <Wire.h>                     // Wire library for I2C communication

void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("Time sync...");
        display.display();
        return;
    }

    // Print to Serial Monitor
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
    // Clear the OLED display and show the current time
    display.clearDisplay();
    display.setTextSize(2);           // Set the text size
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
  
    // Format and display the time (hh:mm:ss)
    display.printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    display.display(); // Update the display
}