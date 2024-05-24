#include "NextTrain.h"

NextTrain::NextTrain(const char* server, String url) {
    this->server = server;
    this->url = url;
    this->timeToNextOne = -1;
}


void NextTrain::request() {
    HTTPClient http;
    String fullUrl = server + url;
    http.begin(fullUrl);
    int httpCode = http.GET();

    if (httpCode > 0) { // Check for the returning code
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(4096);
            DeserializationError error = deserializeJson(doc, payload);
            if (!error) {
                timeToNextOne = doc[0]["values"][0]["minutes"];
                timestamp = doc[0]["values"][0]["timestamp"];
                Serial.println('timestamp');
                Serial.println(timestamp);
                if (unixTime != nullptr) {
                    delete unixTime;
                }
                unixTime = new UnixTime(timestamp);
            } else {
                timeToNextOne = -1;
                timestamp = 0;
                if (unixTime != nullptr) {
                    delete unixTime;
                    unixTime = nullptr;
                }
            }
        } else {
            timeToNextOne = -1;
            timestamp = 0;
            if (unixTime != nullptr) {
                delete unixTime;
                unixTime = nullptr;
            }
        }
    } else {
        timeToNextOne = -1;
        timestamp = 0;
        if (unixTime != nullptr) {
            delete unixTime;
            unixTime = nullptr;
        }
    }

    http.end();
}

int NextTrain::getTimeToNextOne() {
    return timeToNextOne;
}

long NextTrain::getTimestamp() {
    Serial.println(timestamp);
    return timestamp;
}

void NextTrain::getTimestampComponents(int &year, int &month, int &day, int &hour, int &minute, int &second) {
    if (unixTime != nullptr) {
        unixTime->getDateTimeComponents(year, month, day, hour, minute, second);
    } else {
        year = month = day = hour = minute = second = 0;
    }
}