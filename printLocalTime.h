#ifndef PRINTLOCALTIME_H
#define PRINTLOCALTIME_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>        // OLED display library

extern Adafruit_SSD1306 display;   // Declare the display object

// Function declarations
void printLocalTime();

#endif
