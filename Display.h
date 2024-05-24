#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

class Display {
private:
    int width;
    int height;
    int errorColor;
    int successColor;
    int textColor;

public:
    Display(int width, int height, int errorColor, int successColor, int textColor);
    void networkStatus(bool status);
    void minutes(int minutes);
    void log(const char* message);
};

#endif
