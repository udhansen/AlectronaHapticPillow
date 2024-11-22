#include "displayConfig.h"

// Define the display object (allocate memory)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void initDisplay() {
    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);  // Halt the program if display initialization fails
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Display initialized"));
    display.display();
}

void displayMessage(const char *message) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(message);
    display.display();
}