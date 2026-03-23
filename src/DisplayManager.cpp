#include "DisplayManager.h"

// Initialize the display object in the constructor
DisplayManager::DisplayManager() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

void DisplayManager::begin(uint8_t sdaPin, uint8_t sclPin) {
    // Route I2C to your custom pins (SDA=20, SCL=21)
    Wire.begin(sdaPin, sclPin);

    // 0x3C is the standard I2C address for 0.96" OLEDs
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("[OLED] SSD1306 allocation failed. Check wiring!"));
        return;
    }

    Serial.println(F("[OLED] Screen Initialized."));
    display.setTextColor(SSD1306_WHITE);

    // Show a quick boot screen
    showMessage("SYSTEM BOOT", "Starting up...");
}

void DisplayManager::showMessage(String header, String line1, String line2) {
    display.clearDisplay();

    // Draw the Header
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(header);

    // Draw a separator line
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    // Draw the content
    display.setCursor(0, 16);
    display.println(line1);

    if (line2 != "") {
        display.println(line2);
    }

    // Push the buffer to the actual screen
    display.display();
}

void DisplayManager::clear() {
    display.clearDisplay();
    display.display();
}