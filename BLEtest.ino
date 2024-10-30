#include <ArduinoBLE.h>              // Bluetooth library
#include <Adafruit_GFX.h>             // Core graphics library
#include <Adafruit_SSD1306.h>         // OLED display library
#include <Wire.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9 // d1
#define OLED_CLK   10 // d0
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");  // LED service UUID
BLEByteCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);  // LED characteristic UUID

const int ledPin = LED_BUILTIN;  // Use the built-in LED (usually GPIO 2 on ESP32)
bool isAdvertising = true;       // Track if advertising or connected

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);  // Start with LED off

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Initialize BLE
    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (true);
    }

    BLE.setLocalName("ULHN_LED_Control");   // Device name
    BLE.setAdvertisedService(ledService);    // Advertise the LED service
    BLE.setAdvertisingInterval(100);          // Set advertising interval to 100 ms

    ledService.addCharacteristic(ledCharacteristic);
    BLE.addService(ledService);
    BLE.advertise();

    Serial.println("BLE LED control ready. Use '1' to turn ON, '0' to turn OFF, and 'r' to reset bonding.");
}

void loop() {
    // Check for input from Serial Terminal
    if (Serial.available()) {
        char input = Serial.read();
        if (input == 'r') {
            resetBLE();
        }
    }

    // Update display based on advertising status
    if (isAdvertising) {
        displayBluetoothSymbol(true);  // Flashing symbol for advertising
    }

    // Check for BLE connection
    BLEDevice central = BLE.central();

    if (central) {
        isAdvertising = false;
        displayBluetoothSymbol(false);  // Solid symbol for connected

        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            if (ledCharacteristic.written()) {
                byte ledState = ledCharacteristic.value();
                if (ledState == '1') {
                    Serial.println("LED ON command received");
                    digitalWrite(ledPin, HIGH);  // Turn on the LED
                }
                else if (ledState == '0') {
                    Serial.println("LED OFF command received");
                    digitalWrite(ledPin, LOW);   // Turn off the LED
                }
            }
        }

        isAdvertising = true;          // Return to advertising state after disconnect
        BLE.advertise();
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }
}

void resetBLE() {
    Serial.println("Resetting BLE and clearing bonded devices...");
    // You can implement additional logic here if needed
    BLE.stopAdvertise();  // Stop advertising
    BLE.disconnect();       // Disconnect all connected devices (if any)

    // If you were to implement bonding clear functionality here, you would
    // typically do that via a more advanced BLE library or platform-specific
    // APIs, but this example doesn't currently support that.
    
    BLE.begin();  // Reinitialize BLE
    BLE.advertise();  // Start advertising again
}

void displayBluetoothSymbol(bool flash) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("BLE Status");

    if (flash) {
        // Flash the Bluetooth symbol if advertising
        static bool symbolVisible = true;
        symbolVisible = !symbolVisible;
        if (symbolVisible) {
            drawBluetoothIcon();
        }
    } else {
        // Show the Bluetooth symbol solid if connected
        drawBluetoothIcon();
    }

    display.display();
    delay(500);  // Adjust flashing rate
}

void drawBluetoothIcon() {
    // Draw a basic Bluetooth symbol
    display.drawLine(60, 20, 80, 40, SSD1306_WHITE);   // Diagonal line
    display.drawLine(80, 20, 60, 40, SSD1306_WHITE);   // Diagonal line
    display.drawLine(70, 10, 70, 50, SSD1306_WHITE);   // Vertical line
    display.drawLine(65, 35, 75, 45, SSD1306_WHITE);   // Upper cross line
    display.drawLine(65, 25, 75, 15, SSD1306_WHITE);   // Lower cross line
}