#include "UnixTime.h"

UnixTime::UnixTime(long timestamp) {
    this->timestamp = timestamp / 1000; // Convert to seconds
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

int UnixTime::getYear() {
    long seconds = timestamp;
    int year = 1970;
    while (seconds >= (isLeapYear(year) ? 366 : 365) * 24 * 3600) {
        seconds -= (isLeapYear(year) ? 366 : 365) * 24 * 3600;
        year++;
    }
    return year;
}

int UnixTime::getMonth() {
    long seconds = timestamp;
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

int UnixTime::getDay() {
    long seconds = timestamp;
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

int UnixTime::getHour() {
    return (timestamp % 86400L) / 3600;
}

int UnixTime::getMinute() {
    return (timestamp % 3600) / 60;
}

int UnixTime::getSecond() {
    return timestamp % 60;
}

void UnixTime::getDateTimeComponents(int &year, int &month, int &day, int &hour, int &minute, int &second) {
    year = getYear();
    month = getMonth();
    day = getDay();
    hour = getHour();
    minute = getMinute();
    second = getSecond();
}
