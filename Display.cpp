#include "Display.h"

extern UnixTime globalTimer; // Declare the global timer

Display::Display(int width, int height, int pin, int matrixType, int pixelType)
    : matrix(width, height, pin, matrixType, pixelType), colors{
      matrix.Color(255, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0) }, x(width), pass(0) {}

void Display::begin() {
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(1);
    
    matrix.setTextColor(colors[0]);
    matrix.setTextSize(1);
    x = matrix.width();
}

void Display::updateDisplay(const char* text) {
 
   matrix.fillScreen(0); // Clear the screen

    // Calculate text width only once when text changes
    static int16_t x1, y1;
    static uint16_t w, h;
    matrix.getTextBounds(text, x, 0, &x1, &y1, &w, &h); // Get text width

    matrix.setCursor(x, 0); // Set cursor position
    matrix.print(text); // Print the text
    
    // Scroll text to the left
    x--; // Decrement x by 1
    if (x < -w) { // Check if x is less than -w
        x = matrix.width(); // Reset to start from the right again
        // at the end of each pass, check the time so we can adjust the brightness
        checkTime();
    } 

    matrix.show(); // Update the display
    delay(200); // Small delay to control scroll speed
}

void Display::log(const char* message) {
    Serial.println(message);
}

void Display::networkStatus(bool status) {
    log(status ? "Connected to WiFi." : "Failed to connect to WiFi.");
    updateDisplay(status ? "K" : "X");
}

// not part of the class definition
float mapToRange(float value, float fromLow, float fromHigh) {
    float result = (value - fromLow) / (fromHigh - fromLow);
    result = max(result, 0.0f);
    result = min(result, 1.0f);
    return result;
}
// not part of the class definition
float easeInOutQuad(float t) {
    return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

void Display::checkTime() {
    float decimalHour  = globalTimer.getHour()+globalTimer.getMinute()/60.0;
    float n;
    float brightness;
    float transitionGap;
    if (decimalHour >= 0 && decimalHour < 12) {
        n = mapToRange(decimalHour, 6.0, 8.0);
        
    } else if (decimalHour >= 12 && decimalHour < 23) {
        n = 1-mapToRange(decimalHour, 18.0, 20.0);
    }
    transitionGap = easeInOutQuad(n);
    brightness = 1+ 40 * transitionGap;
    matrix.setBrightness(brightness);
    Serial.println("brightness: " + String(brightness) + " transitionGap: " + String(transitionGap) + " n: " + String(n) + " decimalHour: " + String(decimalHour));
}

void Display::minutes(int minutes) {
    char buffer[14];
    sprintf(buffer, "Next: %d min", minutes);
    log(buffer);
    char buffer2[4];
    sprintf(buffer2, "%d", minutes);

    if (minutes <= 5) {
        matrix.setTextColor(colors[0]); // Change color
    } else if (minutes <= 10) {
        matrix.setTextColor(colors[1]); // Change color
    } else {
        matrix.setTextColor(colors[2]); // Change color
    }
    updateDisplay(buffer2);
}
