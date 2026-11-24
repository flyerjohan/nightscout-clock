#include "BGDisplayFaceStars.h"

#include "BGDisplayFacePixelArt.h"

namespace {

const int SCENE_WIDTH = 12;

const char* const MOON[5] = {
    ".XXX",  //
    "XX..",  //
    "XX..",  //
    "XX..",  //
    ".XXX",  //
};

// Star positions {x, y} and the step each one twinkles on.
const int STAR_COUNT = 7;
const int8_t STARS[STAR_COUNT][3] = {
    {6, 0, 0}, {10, 1, 3}, {8, 3, 5}, {11, 4, 1}, {5, 5, 6}, {9, 6, 2}, {1, 7, 4},
};

const uint16_t MOON_COLOR = 0xFFB0;  // pale yellow
const uint16_t STAR_BRIGHT = 0xFFFF;
const uint16_t STAR_DIM = 0x7BEF;

}  // namespace

void BGDisplayFaceStars::drawScene(unsigned long frame) const {
    DisplayManager.clearMatrixPart(0, 0, SCENE_WIDTH, MATRIX_HEIGHT);
    PixelArt::draw(MOON, 5, 1, 1, MOON_COLOR);
    for (int i = 0; i < STAR_COUNT; i++) {
        const bool twinkle = frame % 8 == static_cast<unsigned long>(STARS[i][2]);
        DisplayManager.drawPixel(STARS[i][0], STARS[i][1], twinkle ? STAR_BRIGHT : STAR_DIM);
    }
}

void BGDisplayFaceStars::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    const GlucoseReading& reading = readings.back();
    drawScene(1);
    showReading(reading, MATRIX_WIDTH - 7, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    showTrendArrow(reading, MATRIX_WIDTH - 5, 1, dataIsOld, getColorByBGValue(reading));
}

unsigned long BGDisplayFaceStars::getAnimationStepMillis() const { return 300; }

void BGDisplayFaceStars::showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const {
    drawScene(frame);
}
