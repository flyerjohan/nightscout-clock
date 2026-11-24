#ifndef BGDISPLAYFACEHEART_H
#define BGDISPLAYFACEHEART_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A beating heart in the glucose range color, beside the value and its trend arrow.
class BGDisplayFaceHeart : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    unsigned long getAnimationStepMillis() const override;
    void showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const override;

private:
    void drawHeart(uint16_t color, unsigned long frame) const;
};

#endif  // BGDISPLAYFACEHEART_H
