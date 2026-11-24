#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <Arduino.h>

#include "WeatherData.h"

// Fetches the current weather for Valencia from Open-Meteo every 30 minutes, for the weather face.
// Open-Meteo needs no account or key. The request is plain HTTP: the answer is public and a TLS
// connection would take memory the glucose source needs.
class WeatherManager_ {
public:
    static WeatherManager_& getInstance();
    void tick();

private:
    WeatherManager_() = default;
    bool fetch();
    unsigned long lastAttempt = 0;
    bool attempted = false;
    bool lastOk = false;
};

extern WeatherManager_& WeatherManager;

#endif  // WEATHERMANAGER_H
