#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>

class Network {
private:
    const char* ssid;
    const char* password;
    bool isConnected;

public:
    Network(const char* ssid, const char* password);
    void connect();
    bool checkConnection();
    void reconnect();
    bool getStatus();
};

#endif
