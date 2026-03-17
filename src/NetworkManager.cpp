#include "NetworkManager.h"

NetworkManager::NetworkManager(const char* ssid, const char* pass, const char* url, const char* dns)
    : ssid(ssid), password(pass), serverUrl(url), dnsServer(dns) {}

void NetworkManager::connect() {
    if (WiFi.status() == WL_CONNECTED) return;

    Serial.println("\n=================================");
    Serial.printf("[WIFI] Connecting to SSID: %s\n", ssid);
    Serial.printf("[WIFI] Configuring DNS with: %s\n", dnsServer ? dnsServer : "(null)");

    // Force custom DNS and avoid router-provided DNS fallback.
    const char* fallbackDns = "192.168.66.6";
    IPAddress selectedDNS;
    bool hasEnvDns = (dnsServer != nullptr) && (strlen(dnsServer) > 0);
    if (hasEnvDns && selectedDNS.fromString(dnsServer)) {
        Serial.printf("[WIFI] Using DNS from .env: %s\n", dnsServer);
    } else {
        selectedDNS.fromString(fallbackDns);
        Serial.printf("[WIFI] Invalid/empty DNS in .env. Forcing fallback DNS: %s\n", fallbackDns);
    }

    IPAddress no_ip(0, 0, 0, 0); // Keep DHCP for IP/GW/subnet, but force DNS.
    WiFi.config(no_ip, no_ip, no_ip, selectedDNS, selectedDNS);
    Serial.printf("[WIFI] Custom DNS Applied (primary/secondary): %s\n", selectedDNS.toString().c_str());

    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WIFI] Connection Successful!");
        printDebug();
    } else {
        Serial.println("\n[WIFI] Connection Failed. Will retry later.");
    }
    Serial.println("=================================\n");
}

void NetworkManager::maintainConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        connect();
    }
}

bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::printDebug() {
    if (isConnected()) {
        Serial.println("--- WiFi Diagnostics ---");
        Serial.printf("IP Address:    %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Gateway:       %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("Primary DNS:   %s\n", WiFi.dnsIP(0).toString().c_str());
        Serial.printf("MAC Address:   %s\n", WiFi.macAddress().c_str());
        Serial.printf("Signal (RSSI): %d dBm\n", WiFi.RSSI());
        Serial.println("------------------------");
    } else {
        Serial.println("[WIFI DEBUG] Status: DISCONNECTED.");
    }
}

void NetworkManager::sendApiRequest(String uid) {
    if (!isConnected()) {
        Serial.println("[API ERROR] WiFi is disconnected. Cannot send request.");
        return;
    }

    HTTPClient http;
    String fullUrl = String(serverUrl) + uid;

    Serial.printf("[API] GET Request to: %s\n", fullUrl.c_str());

    http.begin(fullUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.printf("[API] Success! HTTP %d: %s\n", httpResponseCode, http.getString().c_str());
    } else {
        Serial.printf("[API] Failed! Error Code: %d\n", httpResponseCode);
    }
    http.end();
}