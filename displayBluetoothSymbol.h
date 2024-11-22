#ifndef DISPLAYBLUETOOTHSYMBOL_H
#define DISPLAYBLUETOOTHSYMBOL_H

#include "displayConfig.h"  // Include displayConfig.h to access SCREEN_WIDTH and display object

extern Adafruit_SSD1306 display;   // Declare the external display object
extern bool isAdvertising;  // Declare the external isAdvertising flag

void displayBluetoothSymbol();

#endif
