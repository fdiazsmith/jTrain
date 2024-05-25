#include "NextTrain.h"

NextTrain::NextTrain(const char* server, String url) {
    this->server = server;
    this->url = url;
    this->timeToNextOne = -1;
    this->unixTime = nullptr;
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
                timestamp = doc[0]["values"][0]["timestamp"].as<int64_t>();
                if (unixTime != nullptr) {
                    delete unixTime;
                }
                currentTime = new UnixTime("America", "Los_Angeles");
                unixTime = new UnixTime(timestamp);
                unixTime->setRawOffset(currentTime->getRawOffset());

                
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

void NextTrain::update() {
    if (currentTime != nullptr) {
        // unixTime->update();
        currentTime->update();
    }
}

int NextTrain::getTimeToNextOne() {


        
        // int year, month, day, hour, minute, second;

        // currentTime->getDateTimeComponents(year, month, day, hour, minute, second);
        // char buffer[64];
        // sprintf(buffer, "current train: %02d-%02d-%04d %02d:%02d:%02d", day, month, year, hour, minute, second);
        // Serial.println(buffer);

        // unixTime->getDateTimeComponents(year, month, day, hour, minute, second);
        // char buffer2[64];
        // sprintf(buffer2, "-next train  : %02d-%02d-%04d %02d:%02d:%02d", day, month, year, hour, minute, second);
        // Serial.println(buffer2);

       
        // UnixTime diff = unixTime - currentTime;
        // //print diff timestamp
        // Serial.print("diff timestamp: ");
        // Serial.println(diff.getTimestamp() );

        // // print diff buffer 
        // int diffYear, diffMonth, diffDay, diffHour, diffMinute, diffSecond;
        // diff.getDateTimeComponents(diffYear, diffMonth, diffDay, diffHour, diffMinute, diffSecond);
        // char buffer3[64];
        // sprintf(buffer3, "diff       : %02d-%02d-%04d %02d:%02d:%02d", diffDay, diffMonth, diffYear, diffHour, diffMinute, diffSecond);
        // Serial.println(buffer3);
        // delay(1000);
        
    return timeToNextOne;
}

long NextTrain::getTimestamp() {
    return timestamp;
}

void NextTrain::getTimestampComponents(int &year, int &month, int &day, int &hour, int &minute, int &second) {
    if (unixTime != nullptr) {
        unixTime->getDateTimeComponents(year, month, day, hour, minute, second);
    } else {
        year = month = day = hour = minute = second = 0;
    }
}
