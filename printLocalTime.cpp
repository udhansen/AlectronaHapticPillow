#include "printLocalTime.h"

void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        display.printf("WiFi disabled");
        return;
    }
  
    // Clear the OLED display and show the current time
    display.clearDisplay();
    display.setTextSize(2);           // Set the text size
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
  
    // Format and display the time (hh:mm:ss)
    display.printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    display.display(); // Update the display
}