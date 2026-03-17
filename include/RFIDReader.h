#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class RFIDReader {
private:
    uint8_t ssPin;
    uint8_t rstPin;
    MFRC522 mfrc522;

public:
    // Constructor
    RFIDReader(uint8_t ss, uint8_t rst);

    // Initialize SPI and the module
    void begin(int sck, int miso, int mosi);

    // Checks for a card and returns UID as a String (or empty string if no card)
    String getUID();
};