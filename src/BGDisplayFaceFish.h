#ifndef BGDISPLAYFACEFISH_H
#define BGDISPLAYFACEFISH_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A little fish swimming back and forth among rising bubbles, beside the value and its trend arrow.
class BGDisplayFaceFish : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    unsigned long getAnimationStepMillis() const override;
    void showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const override;

private:
    void drawScene(unsigned long frame) const;
};

#endif  // BGDISPLAYFACEFISH_H
