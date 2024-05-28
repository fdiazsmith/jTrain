#ifndef NEXTTRAIN_H
#define NEXTTRAIN_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "UnixTime.h"

extern UnixTime globalTimer; // Declare the global timer

class NextTrain {
private:
    const char* server;
    String url;
    int timeToNextOne;
    int64_t timestamp;

public:
    UnixTime *unixTime;
    
    NextTrain(const char* server, String url);
    void request();
    int getTimeToNextOne();
    long getTimestamp();
    void update();
   void getTimestampComponents(int &year, int &month, int &day, int &hour, int &minute, int &second);
};

#endif
