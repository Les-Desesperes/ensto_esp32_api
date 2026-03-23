#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class DisplayManager {
private:
    Adafruit_SSD1306 display;

public:
    // Constructor
    DisplayManager();

    // Initialize the screen with your custom left-side I2C pins
    void begin(uint8_t sdaPin, uint8_t sclPin);

    // Reusable function to draw menus/status updates
    void showMessage(String header, String line1, String line2 = "");

    // Clear the screen completely
    void clear();
};