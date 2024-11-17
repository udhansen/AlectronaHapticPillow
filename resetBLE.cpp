#include "resetBLE.h"

void resetBLE() {
    Serial.println("Resetting BLE and clearing bonded devices...");
    BLE.stopAdvertise();  // Stop advertising
    BLE.disconnect();       // Disconnect all connected devices (if any)
    
    BLE.begin();  // Reinitialize BLE
    BLE.advertise();  // Start advertising again
}