#include "BGDisplayFaceBigClock.h"

#include "BGDisplayFacePixelArt.h"
#include "ServerManager.h"

void BGDisplayFaceBigClock::drawText() const {
    tm now = ServerManager.getTimezonedTime();
    int hour = now.tm_hour;
    if (SettingsManager.settings.time_format == TIME_FORMAT::HOURS_12) {
        hour = hour % 12 == 0 ? 12 : hour % 12;
    }
    char text[6];
    snprintf(text, sizeof(text), "%d:%02d", hour, now.tm_min);
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(COLOR_WHITE);
    DisplayManager.printText(0, 6, text, TEXT_ALIGNMENT::CENTER, 0);
}

void BGDisplayFaceBigClock::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    drawText();
    PixelArt::glucoseLine(0, dataIsOld ? getDataOldColor() : getColorByBGValue(readings.back()));
}

void BGDisplayFaceBigClock::showNoData() const {
    DisplayManager.clearMatrix();
    drawText();
    PixelArt::glucoseLine(0, getDataOldColor());
}

// The minutes change, so every time tick redraws.
RenderDecision BGDisplayFaceBigClock::getRenderDecision(const RenderContext& ctx) const {
    return RenderDecision::FULL;
}
