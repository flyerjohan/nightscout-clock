#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <Arduino.h>

// The current weather, written by WeatherManager and read by the weather face. It lives apart from
// WeatherManager so that the face, which the web UI's preview also compiles, needs no network code.
struct WeatherNow {
    bool valid = false;
    float temperatureC = 0;
    // WMO weather interpretation code, as Open-Meteo reports it.
    int code = 0;
    bool isDay = true;
    // millis() when it was fetched.
    unsigned long fetchedAt = 0;
};

// Defined in BGDisplayFaceWeather.cpp.
extern WeatherNow weatherNow;

#endif  // WEATHERDATA_H
