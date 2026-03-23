#include <Arduino.h>
#include "NetworkManager.h"
#include "RFIDReader.h"
#include "DisplayManager.h"

// Injected automatically by PlatformIO script from .env
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* serverUrl = SERVER_URL;
const char* dnsServer = DNS_SERVER;

// Object Instantiation
NetworkManager network(ssid, password, serverUrl, dnsServer);
RFIDReader reader(5, 4);
DisplayManager screen;

// Timers
unsigned long lastDebugTime = 0;
const unsigned long debugInterval = 30000;
unsigned long messageClearTime = 0;
bool isShowingMessage = false;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    delay(1000);

    Serial.println("\n\n--- Starting ESP32-S3 RFID System ---");

    // Initialize OLED Screen on left-side pins (SDA=20, SCL=21)
    screen.begin(20, 21);

    // Initialize RFID Hardware
    reader.begin(6, 7, 15);

    // Connect to WiFi
    screen.showMessage("WIFI", "Connecting to:", ssid);
    network.connect();

    // Ready state
    screen.showMessage("READY", "Waiting for badge...");
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

        // Update the OLED screen
        screen.showMessage("SCAN SUCCESS", "UID Read:", cardUID);

        // Send to API
        network.sendApiRequest(cardUID);

        // Set a timer to clear the success message after 3 seconds
        messageClearTime = millis();
        isShowingMessage = true;
    }

    // Return the screen to the "Ready" state 3 seconds after a scan
    if (isShowingMessage && (millis() - messageClearTime > 3000)) {
        screen.showMessage("READY", "Waiting for badge...");
        isShowingMessage = false;
    }

    delay(50);
}