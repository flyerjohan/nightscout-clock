#ifndef BGDISPLAYFACECAT_H
#define BGDISPLAYFACECAT_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A small sitting cat beside the value and its trend arrow. The cat reacts to the reading without judging it:
// calm and wagging its tail in range, wide-eyed while the reading moves fast or is out of range, hopping next to
// an exclamation mark past an urgent limit, and puzzled, one ear bent, when the data is old or missing.
class BGDisplayFaceCat : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    unsigned long getAnimationStepMillis() const override;
    void showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const override;

    enum class Mood : uint8_t { CALM, WATCHFUL, ALERT, PUZZLED };
    static Mood moodFor(BG_LEVEL level, BG_TREND trend, bool dataIsOld);

private:
    void drawCat(Mood mood, unsigned long frame) const;
};

#endif  // BGDISPLAYFACECAT_H
