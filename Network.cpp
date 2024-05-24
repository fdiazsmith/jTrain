#include "Network.h"

Network::Network(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
    isConnected = false;
}

void Network::connect() {
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(1000);
        attempts++;
    }
    isConnected = (WiFi.status() == WL_CONNECTED);
}

bool Network::checkConnection() {
    isConnected = (WiFi.status() == WL_CONNECTED);
    return isConnected;
}

void Network::reconnect() {
    if (!checkConnection()) {
        connect();
    }
}

bool Network::getStatus() {
    return isConnected;
}
