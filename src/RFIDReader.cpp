#include "RFIDReader.h"

// Initialize the MFRC522 object in the constructor's initializer list
RFIDReader::RFIDReader(uint8_t ss, uint8_t rst) : ssPin(ss), rstPin(rst), mfrc522(ss, rst) {}

void RFIDReader::begin(int sck, int miso, int mosi) {
    SPI.begin(sck, miso, mosi, ssPin);
    mfrc522.PCD_Init();
    Serial.println("[RFID] Hardware Initialized.");
}

String RFIDReader::getUID() {
    // Check if a new card is present and can be read
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return "";
    }

    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uidString += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        uidString += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidString.toUpperCase();

    // Halt card so it doesn't read the same scan 100 times a second
    mfrc522.PICC_HaltA();

    return uidString;
}