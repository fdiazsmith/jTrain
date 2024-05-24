#include <Arduino.h>
#include "Network.h"
#include "Display.h"
#include "NextTrain.h"

// Define your network credentials
const char* ssid = "BPE";
const char* password = "bigpantieenergy";
int RED = 0;
int GREEN = 0;
int WHITE = 0;
// Create an instance of the Network class
Network network(ssid, password);
Display display(32, 8, RED, GREEN, WHITE);
NextTrain nextTrain("https://webservices.umoiq.com", "/api/pub/v1/agencies/sfmta-cis/stopcodes/16215/predictions?key=0be8ebd0284ce712a63f29dcaf7798c4");

enum State {
    STATE_INIT,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_DISCONNECTED,
    STATE_FETCHING,
    STATE_DISPLAYING,
    STATE_ERROR
};

State currentState = STATE_INIT;

void setup() {
    Serial.begin(115200);
    network.connect();
    display.log("Starting jTrain...");
    display.networkStatus(network.getStatus());
}

void loop() {
    unsigned long currentTime = millis();
     long timestamp;
    switch (currentState) {
        case STATE_INIT:
            // Initialize network connection
            currentState = STATE_CONNECTING;
            network.connect();
            // stateChangeTime = currentTime;
            break;

        case STATE_CONNECTING:
            // Attempt to connect to WiFi
            if (network.checkConnection()) {
                currentState = STATE_CONNECTED;
            } 
            // else if (currentTime - stateChangeTime >= DISPLAY_TIME) {
            //     currentState = STATE_DISCONNECTED;
            // }
            // display.networkStatus(network.getStatus());
            // stateChangeTime = currentTime;
            break;

        case STATE_CONNECTED:
            // Connected to WiFi
            // display.networkStatus(network.getStatus());
            // delay(3000); // Display status for 3 seconds
            currentState = STATE_FETCHING;
            break;

        case STATE_DISCONNECTED:
            // Failed to connect, retry
            // display.networkStatus(false);
            // delay(3000); // Display status for 3 seconds
            currentState = STATE_CONNECTING;
            network.connect();
            // stateChangeTime = currentTime;
            break;

        case STATE_FETCHING:
            // Fetch next train data
            // nextTrain.request();
            // if (nextTrain.getTimeToNextOne() >= 0) {
            //     currentState = STATE_DISPLAYING;
            // } else {
            //     currentState = STATE_ERROR;
            // }
            // stateChangeTime = currentTime;
            nextTrain.request();
            currentState = STATE_DISPLAYING;
            delay(20000);
            break;

        case STATE_DISPLAYING:
            // Display next train time
            // display.minutes(nextTrain.getTimeToNextOne());
            // if (currentTime - stateChangeTime >= FETCH_INTERVAL) {
            //     currentState = STATE_FETCHING;
            // }
            display.minutes(nextTrain.getTimeToNextOne());
                    // Additional code to display the time components
            timestamp = nextTrain.getTimestamp();

            if (timestamp != 0) {
                int year, month, day, hour, minute, second;
                nextTrain.getTimestampComponents(year, month, day, hour, minute, second);

                char buffer[64];
                sprintf(buffer, "Time: %02d-%02d-%04d %02d:%02d:%02d", day, month, year, hour, minute, second);
                display.log(buffer);
            }
      

            currentState = STATE_FETCHING;
            break;

        case STATE_ERROR:
            // Handle errors
            // display.networkStatus(false);
            // delay(3000); // Display error for 3 seconds
            currentState = STATE_FETCHING;
            break;
    }
}
