#include "BGDisplayFaceFish.h"

#include "BGDisplayFacePixelArt.h"

namespace {

const int SCENE_WIDTH = 12;
const int FISH_WIDTH = 6;
const int FISH_HEIGHT = 4;

// Facing right; mirrored when swimming left. The eye is drawn over the body.
const char* const FISH[FISH_HEIGHT] = {
    "...XX.",  //
    "X.XXXX",  //
    "XXXXXX",  //
    "X.XXX.",  //
};

const uint16_t FISH_COLOR = 0xFC60;    // orange
const uint16_t EYE_COLOR = 0x0000;
const uint16_t BUBBLE_COLOR = 0x5D7F;  // light blue
const uint16_t WATER_COLOR = 0x0010;   // deep blue, the bottom row

// The fish swims from x 0 to SCENE_WIDTH - FISH_WIDTH and back, one pixel a step.
const int TRAVEL = SCENE_WIDTH - FISH_WIDTH;

}  // namespace

void BGDisplayFaceFish::drawScene(unsigned long frame) const {
    DisplayManager.clearMatrixPart(0, 0, SCENE_WIDTH, MATRIX_HEIGHT);
    for (int x = 0; x < SCENE_WIDTH; x++) {
        DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, WATER_COLOR);
    }

    const unsigned long position = frame % (2 * TRAVEL);
    const bool goingRight = position < static_cast<unsigned long>(TRAVEL);
    const int x = goingRight ? position : 2 * TRAVEL - position;
    const int y = 1 + (frame / 3) % 2;  // a gentle bob
    if (goingRight) {
        PixelArt::draw(FISH, FISH_HEIGHT, x, y, FISH_COLOR);
        DisplayManager.drawPixel(x + 4, y + 1, EYE_COLOR);
    } else {
        PixelArt::drawMirrored(FISH, FISH_HEIGHT, FISH_WIDTH, x, y, FISH_COLOR);
        DisplayManager.drawPixel(x + 1, y + 1, EYE_COLOR);
    }

    // Two bubbles rise from the bottom, a few steps apart.
    for (int b = 0; b < 2; b++) {
        const int bubbleX = b == 0 ? 2 : 9;
        const int bubbleY = MATRIX_HEIGHT - 2 - static_cast<int>((frame + b * 4) % 7);
        if (bubbleY >= 0) {
            DisplayManager.drawPixel(bubbleX, bubbleY, BUBBLE_COLOR);
        }
    }
}

void BGDisplayFaceFish::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    const GlucoseReading& reading = readings.back();
    drawScene(0);
    showReading(reading, MATRIX_WIDTH - 7, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    showTrendArrow(reading, MATRIX_WIDTH - 5, 1, dataIsOld, getColorByBGValue(reading));
}

unsigned long BGDisplayFaceFish::getAnimationStepMillis() const { return 350; }

void BGDisplayFaceFish::showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const {
    drawScene(frame);
}
