#include "WeatherManager.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

#include "globals.h"

namespace {

// Valencia, Spain.
const char* const WEATHER_URL =
    "http://api.open-meteo.com/v1/forecast?latitude=39.47&longitude=-0.38"
    "&current=temperature_2m,weather_code,is_day";

const unsigned long REFRESH_MILLIS = 30UL * 60 * 1000;
// After a failure, try again sooner.
const unsigned long RETRY_MILLIS = 5UL * 60 * 1000;
// Wait a little after start so the glucose source connects first.
const unsigned long FIRST_FETCH_MILLIS = 30UL * 1000;
const uint16_t TIMEOUT_MILLIS = 4000;

}  // namespace

WeatherManager_& WeatherManager_::getInstance() {
    static WeatherManager_ instance;
    return instance;
}

WeatherManager_& WeatherManager = WeatherManager_::getInstance();

void WeatherManager_::tick() {
    const unsigned long now = millis();
    if (!attempted) {
        if (now < FIRST_FETCH_MILLIS) {
            return;
        }
    } else {
        if (now - lastAttempt < (lastOk ? REFRESH_MILLIS : RETRY_MILLIS)) {
            return;
        }
    }

    attempted = true;
    lastAttempt = now;
    lastOk = fetch();
    if (!lastOk) {
        DEBUG_PRINTLN("Weather: fetch failed");
    }
}

bool WeatherManager_::fetch() {
    WiFiClient wifiClient;
    HTTPClient http;
    http.setTimeout(TIMEOUT_MILLIS);
    http.setConnectTimeout(TIMEOUT_MILLIS);
    if (!http.begin(wifiClient, WEATHER_URL)) {
        return false;
    }
    http.addHeader("User-Agent", "Nightscout-clock");
    const int status = http.GET();
    if (status != HTTP_CODE_OK) {
        DEBUG_PRINTLN("Weather: HTTP " + String(status));
        http.end();
        return false;
    }

    JsonDocument filter;
    filter["current"]["temperature_2m"] = true;
    filter["current"]["weather_code"] = true;
    filter["current"]["is_day"] = true;
    JsonDocument doc;
    const DeserializationError error =
        deserializeJson(doc, http.getString(), DeserializationOption::Filter(filter));
    http.end();
    if (error || !doc["current"]["temperature_2m"].is<float>()) {
        DEBUG_PRINTLN("Weather: unexpected answer");
        return false;
    }

    weatherNow.temperatureC = doc["current"]["temperature_2m"].as<float>();
    weatherNow.code = doc["current"]["weather_code"] | 0;
    weatherNow.isDay = (doc["current"]["is_day"] | 1) == 1;
    weatherNow.fetchedAt = millis();
    weatherNow.valid = true;
    DEBUG_PRINTLN(
        "Weather: " + String(weatherNow.temperatureC, 1) + " C, code " + String(weatherNow.code));
    return true;
}
