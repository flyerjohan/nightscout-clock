#include "BGDisplayFaceHeart.h"

#include "BGDisplayFacePixelArt.h"

namespace {

const char* const BIG_HEART[6] = {
    ".XX.XX.",  //
    "XXXXXXX",  //
    "XXXXXXX",  //
    ".XXXXX.",  //
    "..XXX..",  //
    "...X...",  //
};
const char* const SMALL_HEART[4] = {
    ".X.X.",  //
    "XXXXX",  //
    ".XXX.",  //
    "..X..",  //
};

// A beat is two quick pulses and a rest: big, small, big, then small for three steps.
const bool BEAT[6] = {true, false, true, false, false, false};

}  // namespace

void BGDisplayFaceHeart::drawHeart(uint16_t color, unsigned long frame) const {
    DisplayManager.clearMatrixPart(0, 0, 8, MATRIX_HEIGHT);
    if (BEAT[frame % 6]) {
        PixelArt::draw(BIG_HEART, 6, 0, 1, color);
    } else {
        PixelArt::draw(SMALL_HEART, 4, 1, 2, color);
    }
}

void BGDisplayFaceHeart::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    const GlucoseReading& reading = readings.back();
    const uint16_t color = dataIsOld ? getDataOldColor() : getColorByBGValue(reading);
    drawHeart(color, 0);
    showReading(reading, MATRIX_WIDTH - 7, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    showTrendArrow(reading, MATRIX_WIDTH - 5, 1, dataIsOld, color);
}

unsigned long BGDisplayFaceHeart::getAnimationStepMillis() const { return 180; }

void BGDisplayFaceHeart::showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const {
    drawHeart(getColorByBGValue(readings.back()), frame);
}
