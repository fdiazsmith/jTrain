#ifndef UNIXTIME_H
#define UNIXTIME_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class UnixTime {
private:
    int64_t timestamp; // Unix timestamp in seconds
    long dstoffset;
    long rawOffset;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    bool fetchTimeFromAPI(const String& continent, const String& city);

public:
    UnixTime();
    UnixTime(int64_t timestamp);
    UnixTime(const String& continent, const String& city);

    void setTimestamp(int64_t timestamp);
    int64_t getTimestamp() const;

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    void setRawOffset(long offset) { rawOffset = offset; timestamp += offset;}
    long getRawOffset() const { return rawOffset; }
    void getDateTimeComponents(int &year, int &month, int &day, int &hour, int &minute, int &second) const;

    UnixTime operator-(const UnixTime& other) const;

    void update();
};

#endif
