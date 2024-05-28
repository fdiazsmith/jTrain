#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "UnixTime.h"


extern UnixTime globalTimer; // Declare the global timer


class Display {
private:
    Adafruit_NeoMatrix matrix;
    const uint16_t colors[3];
    int x;
    int pass;
    void checkTime();
public:
    Display(int width, int height, int pin, int matrixType, int pixelType);
    void begin();
    void updateDisplay(const char* text);
    void log(const char* message);
    void networkStatus(bool status);
    void minutes(int minutes);
};

#endif
