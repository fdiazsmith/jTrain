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
Display display(4, 8, 13, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

NextTrain nextTrain("https://webservices.umoiq.com", "/api/pub/v1/agencies/sfmta-cis/stopcodes/16215/predictions?key=0be8ebd0284ce712a63f29dcaf7798c4");
UnixTime timeUntil;

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
    display.begin();
    display.log("Starting jTrain...v4");
    display.networkStatus(network.getStatus());

}
unsigned long stateChangeTime = 0;

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
                display.networkStatus(network.getStatus());
                delay(3000); // Display status for 3 seconds
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
            
            break;

        case STATE_DISPLAYING:
            // Display next train time
            // display.minutes(nextTrain.getTimeToNextOne());
            // if (currentTime - stateChangeTime >= FETCH_INTERVAL) {
            //     currentState = STATE_FETCHING;
             
            // }
            if (millis() - stateChangeTime >= 20000) {
                currentState = STATE_FETCHING;
                stateChangeTime = millis();
            }
            else{
                currentState = STATE_DISPLAYING;

                // nextTrain.update();
                display.minutes(nextTrain.getTimeToNextOne());
                // delay(3000);
            }
       
            break;

        case STATE_ERROR:
            // Handle errors
            // display.networkStatus(false);
            // delay(3000); // Display error for 3 seconds
            currentState = STATE_FETCHING;
            break;
    }
}
