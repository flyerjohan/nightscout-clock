#ifndef BGDISPLAYFACEDATE_H
#define BGDISPLAYFACEDATE_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// The weekday and the day of the month in Spanish, with a line in the glucose range color under them.
class BGDisplayFaceDate : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    RenderDecision getRenderDecision(const RenderContext& ctx) const override;

private:
    void drawText() const;
};

#endif  // BGDISPLAYFACEDATE_H
