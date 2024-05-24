#ifndef UNIXTIME_H
#define UNIXTIME_H

#include <Arduino.h>

class UnixTime {
private:
    long timestamp; // Unix timestamp in seconds

public:
    UnixTime(long timestamp);

    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();

    void getDateTimeComponents(int &year, int &month, int &day, int &hour, int &minute, int &second);
};

#endif
