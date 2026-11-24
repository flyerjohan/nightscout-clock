#include "BGDisplayFaceCat.h"

#include "BGDisplayManager.h"
#include "globals.h"

namespace {

// The cat sits in the first CAT_WIDTH columns; the exclamation mark of the alert mood is in the next one but one.
const int CAT_WIDTH = 8;
const int MARK_COLUMN = 9;
const int CAT_HEIGHT = 8;

// F fur, the rest of the body; the tail is drawn separately so it can move.
const char* const CAT_ART[CAT_HEIGHT] = {
    "F...F",  //
    "FFFFF",  //
    "FFFFF",  //
    "FFFFF",  //
    ".FFF.",  //
    "FFFFF",  //
    "FFFFF",  //
    "FFFFF",  //
};

// Tail positions as {row, column}, from resting low to raised.
const int TAIL_CELLS = 4;
const int8_t TAILS[4][TAIL_CELLS][2] = {
    {{7, 5}, {7, 6}, {6, 7}, {5, 7}},
    {{7, 5}, {7, 6}, {7, 7}, {6, 7}},
    {{7, 5}, {6, 6}, {5, 6}, {4, 7}},
    {{7, 5}, {7, 6}, {7, 7}, {6, 7}},
};
const int RAISED_TAIL = 2;

const uint16_t FUR = 0xECE4;    // warm orange
const uint16_t NOSE = 0xEC96;   // pink, also the cheeks
const uint16_t EYE = 0x0000;    // an unlit LED
const uint16_t MARK = 0xF800;   // red

const unsigned long STEP_MILLIS = 250;

void cell(int row, int col, int dy, uint16_t color) {
    const int y = row + dy;
    if (y >= 0 && y < CAT_HEIGHT && col >= 0 && col < CAT_WIDTH) {
        DisplayManager.drawPixel(col, y, color);
    }
}

}  // namespace

BGDisplayFaceCat::Mood BGDisplayFaceCat::moodFor(BG_LEVEL level, BG_TREND trend, bool dataIsOld) {
    if (dataIsOld || level == BG_LEVEL::INVALID) {
        return Mood::PUZZLED;
    }
    if (level == BG_LEVEL::URGENT_LOW || level == BG_LEVEL::URGENT_HIGH) {
        return Mood::ALERT;
    }
    const bool fast = trend == BG_TREND::DOUBLE_UP || trend == BG_TREND::SINGLE_UP ||
                      trend == BG_TREND::SINGLE_DOWN || trend == BG_TREND::DOUBLE_DOWN;
    if (fast || level != BG_LEVEL::NORMAL) {
        return Mood::WATCHFUL;
    }
    return Mood::CALM;
}

void BGDisplayFaceCat::drawCat(Mood mood, unsigned long frame) const {
    DisplayManager.clearMatrixPart(0, 0, MARK_COLUMN + 1, CAT_HEIGHT);

    // The alert mood hops a row every other step.
    const int dy = mood == Mood::ALERT && frame % 2 == 0 ? -1 : 0;

    for (int row = 0; row < CAT_HEIGHT; row++) {
        for (int col = 0; col < 5; col++) {
            if (CAT_ART[row][col] == 'F') {
                cell(row, col, dy, FUR);
            }
        }
    }

    int tail;
    switch (mood) {
        case Mood::CALM:
            tail = (frame / 3) % 4;  // a slow wag
            break;
        case Mood::WATCHFUL:
            tail = RAISED_TAIL;
            break;
        case Mood::ALERT:
            tail = frame % 2 == 0 ? RAISED_TAIL : 0;
            break;
        default:
            tail = 1;
            break;
    }
    for (int i = 0; i < TAIL_CELLS; i++) {
        cell(TAILS[tail][i][0], TAILS[tail][i][1], dy, FUR);
    }

    // A blink now and then: the eyes close into fur for one step.
    const bool blink = (mood == Mood::CALM || mood == Mood::WATCHFUL) && frame % 16 == 0;
    if (!blink) {
        cell(2, 1, dy, EYE);
        cell(2, 3, dy, EYE);
        if (mood == Mood::WATCHFUL || mood == Mood::ALERT) {
            // Wide eyes, two LEDs tall.
            cell(1, 1, dy, EYE);
            cell(1, 3, dy, EYE);
        }
    }
    cell(3, 2, dy, NOSE);
    if (mood == Mood::CALM || mood == Mood::ALERT) {
        cell(3, 0, dy, NOSE);
        cell(3, 4, dy, NOSE);
    }

    if (mood == Mood::PUZZLED) {
        // One ear bends outward.
        cell(0, 4, 0, 0);
        DisplayManager.drawPixel(5, 0, FUR);
    }

    if (mood == Mood::ALERT && frame % 2 == 0) {
        for (int row = 1; row <= 4; row++) {
            DisplayManager.drawPixel(MARK_COLUMN, row, MARK);
        }
        DisplayManager.drawPixel(MARK_COLUMN, 6, MARK);
    }
}

void BGDisplayFaceCat::showReadings(const GlucoseReadings& readings, bool dataIsOld) const {
    const GlucoseReading& reading = readings.back();
    const BG_LEVEL level = bgDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv);

    drawCat(moodFor(level, reading.trend, dataIsOld), 1);
    showReading(reading, MATRIX_WIDTH - 7, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    showTrendArrow(reading, MATRIX_WIDTH - 5, 1, dataIsOld, getColorByBGValue(reading));
}

void BGDisplayFaceCat::showNoData() const {
    DisplayManager.clearMatrix();
    drawCat(Mood::PUZZLED, 1);
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(getDataOldColor());
    DisplayManager.printText(MATRIX_WIDTH - 1, 6, "---", TEXT_ALIGNMENT::RIGHT, 0);
}

unsigned long BGDisplayFaceCat::getAnimationStepMillis() const { return STEP_MILLIS; }

void BGDisplayFaceCat::showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const {
    const GlucoseReading& reading = readings.back();
    const BG_LEVEL level = bgDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv);
    drawCat(moodFor(level, reading.trend, false), frame);
}
