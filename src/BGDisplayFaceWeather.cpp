#include "BGDisplayFaceWeather.h"

#include "BGDisplayManager.h"
#include "WeatherData.h"
#include "globals.h"

WeatherNow weatherNow;

namespace {

const int ICON_SIZE = 8;
// Weather older than this is not shown.
const unsigned long WEATHER_MAX_AGE_MILLIS = 3UL * 60 * 60 * 1000;

// One letter per pixel: S sun, M moon, C cloud, D dark cloud, R rain, W snow, B bolt, F fog.
const char* const SUN[ICON_SIZE] = {
    "...S....",  //
    ".S...S..",  //
    "...SS...",  //
    "S.SSSS.S",  //
    "..SSSS..",  //
    "...SS...",  //
    ".S...S..",  //
    "...S....",  //
};
const char* const MOON[ICON_SIZE] = {
    "..MMM...",  //
    ".MM.....",  //
    "MM......",  //
    "MM......",  //
    "MM......",  //
    ".MM...M.",  //
    "..MMMM..",  //
    "........",  //
};
const char* const SUN_AND_CLOUD[ICON_SIZE] = {
    "..S.....",  //
    "S.SS.S..",  //
    ".SSSS...",  //
    "SSSCCC..",  //
    ".SCCCCC.",  //
    ".CCCCCCC",  //
    ".CCCCCCC",  //
    "........",  //
};
const char* const CLOUD[ICON_SIZE] = {
    "........",  //
    "...CC...",  //
    "..CCCC..",  //
    ".CCCCCC.",  //
    "CCCCCCCC",  //
    "CCCCCCCC",  //
    ".CCCCCC.",  //
    "........",  //
};
const char* const FOG[ICON_SIZE] = {
    "........",  //
    "FFFFFF..",  //
    "........",  //
    "..FFFFFF",  //
    "........",  //
    "FFFFFF..",  //
    "........",  //
    "..FFFFFF",  //
};
const char* const RAIN[ICON_SIZE] = {
    "...DD...",  //
    "..DDDD..",  //
    ".DDDDDD.",  //
    "DDDDDDDD",  //
    ".DDDDDD.",  //
    "........",  //
    ".R..R..R",  //
    "R..R..R.",  //
};
const char* const SNOW[ICON_SIZE] = {
    "...CC...",  //
    "..CCCC..",  //
    ".CCCCCC.",  //
    "CCCCCCCC",  //
    ".CCCCCC.",  //
    "........",  //
    ".W..W..W",  //
    "W..W..W.",  //
};
const char* const STORM[ICON_SIZE] = {
    "...DD...",  //
    "..DDDD..",  //
    ".DDDDDD.",  //
    "DDDDDDDD",  //
    ".DDBDDD.",  //
    "...BB...",  //
    "....B...",  //
    "...B....",  //
};

uint16_t colorOf(char c) {
    switch (c) {
        case 'S':
            return 0xFE60;  // warm yellow
        case 'M':
            return 0xCE79;  // pale gray
        case 'C':
            return 0xEF7D;  // white-gray
        case 'D':
            return 0xB5B6;  // gray, for rain and storm clouds
        case 'R':
            return 0x3B3F;  // blue
        case 'W':
            return 0xFFFF;  // white
        case 'B':
            return 0xFFE0;  // yellow
        case 'F':
            return 0xAD55;  // gray
        default:
            return 0;
    }
}

// Picks the icon for a WMO weather code.
const char* const* iconFor(int code, bool isDay) {
    if (code <= 1) {
        return isDay ? SUN : MOON;
    }
    if (code == 2) {
        return isDay ? SUN_AND_CLOUD : CLOUD;
    }
    if (code == 3) {
        return CLOUD;
    }
    if (code == 45 || code == 48) {
        return FOG;
    }
    if ((code >= 71 && code <= 77) || code == 85 || code == 86) {
        return SNOW;
    }
    if (code >= 95) {
        return STORM;
    }
    return RAIN;  // drizzle, rain and showers
}

bool weatherIsFresh() {
    return weatherNow.valid && millis() - weatherNow.fetchedAt < WEATHER_MAX_AGE_MILLIS;
}

}  // namespace

void BGDisplayFaceWeather::drawWeather() const {
    const bool fresh = weatherIsFresh();
    const char* const* icon = fresh ? iconFor(weatherNow.code, weatherNow.isDay) : CLOUD;
    for (int row = 0; row < ICON_SIZE; row++) {
        for (int col = 0; col < ICON_SIZE; col++) {
            const char c = icon[row][col];
            if (c != '.') {
                DisplayManager.drawPixel(col, row, fresh ? colorOf(c) : getDataOldColor());
            }
        }
    }

    // The temperature, right-aligned before a two-pixel degree sign.
    const int degreeX = MATRIX_WIDTH - 2;
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(fresh ? static_cast<uint16_t>(DISPLAY_COLOR::WHITE) : getDataOldColor());
    const String text = fresh ? String(static_cast<int>(lroundf(weatherNow.temperatureC))) : String("--");
    DisplayManager.printText(degreeX - 2, 5, text.c_str(), TEXT_ALIGNMENT::RIGHT, 0);
    const uint16_t degreeColor =
        fresh ? static_cast<uint16_t>(DISPLAY_COLOR::WHITE) : getDataOldColor();
    DisplayManager.drawPixel(degreeX, 0, degreeColor);
    DisplayManager.drawPixel(degreeX + 1, 0, degreeColor);
    DisplayManager.drawPixel(degreeX, 1, degreeColor);
    DisplayManager.drawPixel(degreeX + 1, 1, degreeColor);
}

void BGDisplayFaceWeather::drawGlucoseLine(uint16_t color) const {
    for (int x = ICON_SIZE + 2; x < MATRIX_WIDTH; x++) {
        DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, color);
    }
}

void BGDisplayFaceWeather::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    DisplayManager.clearMatrix();
    drawWeather();
    drawGlucoseLine(dataIsOld ? getDataOldColor() : getColorByBGValue(readings.back()));
}

void BGDisplayFaceWeather::showNoData() const {
    DisplayManager.clearMatrix();
    drawWeather();
    drawGlucoseLine(getDataOldColor());
}
