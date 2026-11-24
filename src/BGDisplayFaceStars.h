#ifndef BGDISPLAYFACESTARS_H
#define BGDISPLAYFACESTARS_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A night sky with a moon and twinkling stars beside the value and its trend arrow.
class BGDisplayFaceStars : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    unsigned long getAnimationStepMillis() const override;
    void showAnimationFrame(const GlucoseReadings& readings, unsigned long frame) const override;

private:
    void drawScene(unsigned long frame) const;
};

#endif  // BGDISPLAYFACESTARS_H
