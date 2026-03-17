#include <Arduino.h>
#include "NetworkManager.h"
#include "RFIDReader.h"

// Injected automatically by PlatformIO script from .env
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* serverUrl = SERVER_URL;
const char* dnsServer = DNS_SERVER;

// Object Instantiation
NetworkManager network(ssid, password, serverUrl, dnsServer);
RFIDReader reader(10, 5); // SS_PIN = 10, RST_PIN = 5

// Timers
unsigned long lastDebugTime = 0;
const unsigned long debugInterval = 30000;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    delay(1000);

    Serial.println("\n\n--- Starting ESP32-S3 RFID System ---");

    // SCK=12, MISO=13, MOSI=11
    reader.begin(12, 13, 11);
    network.connect();

    Serial.println("System Ready. Waiting for badges...");
}

void loop() {
    network.maintainConnection();

    if (millis() - lastDebugTime > debugInterval) {
        network.printDebug();
        lastDebugTime = millis();
    }

    String cardUID = reader.getUID();

    if (cardUID != "") {
        Serial.println("\n>>> Card Scanned: " + cardUID);
        network.sendApiRequest(cardUID);
    }

    delay(50);
}