#include "Display.h"

Display::Display(int width, int height, int pin, int matrixType, int pixelType)
    : matrix(width, height, pin, matrixType, pixelType), colors{
      matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) }, x(width), pass(0) {}

void Display::begin() {
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(10);
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
    Serial.println("x is:" + (String)x +"  w is "+ (String)w + "  x+w is "+ (String)(x+w) + "  matrix.width is "+ (String)matrix.width() +  "  pass is "+ (String)pass + "   text "+ text);
    // Scroll text to the left
    x--; // Decrement x by 1
    if (x < -w) { // Check if x is less than -w
        x = matrix.width(); // Reset to start from the right again
        if (++pass >= 3) pass = 0;
        matrix.setTextColor(colors[pass]); // Change color
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

void Display::minutes(int minutes) {
    char buffer[14];
    sprintf(buffer, "Next: %d min", minutes);
    log(buffer);
    char buffer2[4];
    sprintf(buffer2, "-%d", minutes);
    updateDisplay(buffer2);
}
