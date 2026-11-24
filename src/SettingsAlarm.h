#ifndef SettingsAlarm_h
#define SettingsAlarm_h

#include <ArduinoJson.h>

#include <vector>

#include "SettingsAlertWindow.h"

// Must match the no-data alert's defaultMelody in web/src/js/schema.js.
#define NO_DATA_DEFAULT_MELODY "nodata:d=8,o=6,b=140:c,p,e,p,g,4p,g,p,e,p,c"

std::vector<AlertWindow> readAlertWindows(JsonVariantConst configured);
void writeAlertWindows(
    JsonDocument& doc, const char* windowsKey, const std::vector<AlertWindow>& windows);

#endif
