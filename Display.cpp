#include "Display.h"

Display::Display(int width, int height, int errorColor, int successColor, int textColor) {
    this->width = width;
    this->height = height;
    this->errorColor = errorColor;
    this->successColor = successColor;
    this->textColor = textColor;
}

void Display::networkStatus(bool status) {
    if (status) {
        log("Connected to WiFi.");
        // Add code to display success status on LED matrix
    } else {
        log("Failed to connect to WiFi.");
        // Add code to display error status on LED matrix
    }
}

void Display::minutes(int minutes) {
    char buffer[32];
    sprintf(buffer, "Next train in %d minutes", minutes);
    log(buffer);
    // Add code to display minutes on LED matrix
}

void Display::log(const char* message) {
    Serial.println(message);
    // Add code to display message on LED matrix if needed
}
