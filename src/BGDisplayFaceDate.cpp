#include "BGDisplayFaceDate.h"

#include "BGDisplayFacePixelArt.h"
#include "ServerManager.h"

namespace {
// Spanish weekday abbreviations, Sunday first as tm_wday counts them.
const char* const WEEKDAYS[7] = {"DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB"};
}  // namespace

void BGDisplayFaceDate::drawText() const {
    tm now = ServerManager.getTimezonedTime();
    char text[8];
    snprintf(text, sizeof(text), "%s %d", WEEKDAYS[now.tm_wday % 7], now.tm_mday);
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(COLOR_WHITE);
    DisplayManager.printText(0, 6, text, TEXT_ALIGNMENT::CENTER, 0);
}

void BGDisplayFaceDate::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    drawText();
    PixelArt::glucoseLine(0, dataIsOld ? getDataOldColor() : getColorByBGValue(readings.back()));
}

void BGDisplayFaceDate::showNoData() const {
    DisplayManager.clearMatrix();
    drawText();
    PixelArt::glucoseLine(0, getDataOldColor());
}

// The day changes at midnight; redrawing on every minute tick is cheap and simplest.
RenderDecision BGDisplayFaceDate::getRenderDecision(const RenderContext& ctx) const {
    return RenderDecision::FULL;
}
