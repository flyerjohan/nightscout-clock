#include "BGDisplayFaceNight.h"

#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceNight::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    const GlucoseReading& reading = readings.back();
    const BG_LEVEL level = bgDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv);
    const bool inRange = level == BG_LEVEL::NORMAL || level == BG_LEVEL::INVALID;
    const uint16_t color = dataIsOld ? getDataOldColor()
                           : inRange ? static_cast<uint16_t>(DISPLAY_COLOR::RED)
                                     : getBandColor(level);

    DisplayManager.setTextColor(color);
    printReading(reading, MATRIX_WIDTH - 7, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM);
    showTrendArrow(reading, MATRIX_WIDTH - 5, 1, dataIsOld, color);
}
