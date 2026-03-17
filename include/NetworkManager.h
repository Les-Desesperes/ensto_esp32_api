#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class NetworkManager {
private:
    const char* ssid;
    const char* password;
    const char* serverUrl;
    const char* dnsServer;

public:
    // Constructor updated to include DNS
    NetworkManager(const char* ssid, const char* pass, const char* url, const char* dns);

    void connect();
    void maintainConnection();
    bool isConnected();
    void printDebug();
    void sendApiRequest(String uid);
};