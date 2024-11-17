#include "displayBluetoothSymbol.h"

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