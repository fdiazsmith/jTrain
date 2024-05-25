#include "UnixTime.h"
#include <ArduinoJson.h>

UnixTime::UnixTime() : timestamp(0) {}

UnixTime::UnixTime(int64_t timestamp) {
    this->timestamp = timestamp/1000;
}

UnixTime::UnixTime(const String& continent, const String& city) {
    if (!fetchTimeFromAPI(continent, city)) {
        this->timestamp = 0;
    }
}

bool UnixTime::fetchTimeFromAPI(const String& continent, const String& city) {
    String url = "https://worldtimeapi.org/api/timezone/" + continent + "/" + city;
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                this->timestamp = doc["unixtime"].as<int64_t>();
                this->dstoffset = doc["dst_offset"].as<long>();
                this->rawOffset = doc["raw_offset"].as<long>() + this->dstoffset;
                this->timestamp += this->rawOffset;
                return true;
            } else {
                Serial.print("Deserialization error: ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.print("HTTP error: ");
            Serial.println(http.errorToString(httpCode).c_str());
        }
    } else {
        Serial.print("Connection error: ");
        Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
    return false;
}

void UnixTime::setTimestamp(int64_t timestamp) {
    this->timestamp = timestamp;
}

int64_t UnixTime::getTimestamp() const {
    return timestamp;
}

// Helper function to check for leap year
bool isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// Get the total number of days in the given month
int getDaysInMonth(int year, int month) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    }
    const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return daysInMonth[month - 1];
}

int UnixTime::getYear() const {
    int64_t seconds = timestamp;
    int year = 1970;
    while (seconds >= (isLeapYear(year) ? 366 : 365) * 24 * 3600) {
        seconds -= (isLeapYear(year) ? 366 : 365) * 24 * 3600;
        year++;
    }
    return year;
}

int UnixTime::getMonth() const {
    int64_t seconds = timestamp;
    int year = getYear();
    int month = 1;
    for (int i = 1970; i < year; i++) {
        seconds -= (isLeapYear(i) ? 366 : 365) * 24 * 3600;
    }
    while (seconds >= getDaysInMonth(year, month) * 24 * 3600) {
        seconds -= getDaysInMonth(year, month) * 24 * 3600;
        month++;
    }
    return month;
}

int UnixTime::getDay() const {
    int64_t seconds = timestamp;
    int year = getYear();
    int month = getMonth();
    for (int i = 1970; i < year; i++) {
        seconds -= (isLeapYear(i) ? 366 : 365) * 24 * 3600;
    }
    for (int i = 1; i < month; i++) {
        seconds -= getDaysInMonth(year, i) * 24 * 3600;
    }
    return seconds / (24 * 3600) + 1;
}

int UnixTime::getHour() const {
    return (timestamp % 86400L) / 3600;
}

int UnixTime::getMinute() const {
    return (timestamp % 3600) / 60;
}

int UnixTime::getSecond() const {
    return timestamp % 60;
}

void UnixTime::getDateTimeComponents(int &year, int &month, int &day, int &hour, int &minute, int &second) const {
    year = getYear();
    month = getMonth();
    day = getDay();
    hour = getHour();
    minute = getMinute();
    second = getSecond();
}

UnixTime UnixTime::operator-(const UnixTime& other) const {
    return UnixTime(this->timestamp - other.timestamp);
}

void UnixTime::update() {
    this->timestamp += millis() / 1000;
}
