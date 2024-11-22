#ifndef DISPLAYCONFIG_H
#define DISPLAYCONFIG_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declare constants for screen size and pins
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 8       // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_MOSI 9        // d1
#define OLED_CLK 10        // d0
#define OLED_DC 11
#define OLED_CS 12

// Declare the display object as an extern (to be defined in a .cpp file)
extern Adafruit_SSD1306 display;

void initDisplay();
void displayMessage(const char *message);

#endif
