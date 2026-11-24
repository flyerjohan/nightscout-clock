#ifndef BGDISPLAYFACEBIGCLOCK_H
#define BGDISPLAYFACEBIGCLOCK_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// The time across the panel, with a line in the glucose range color under it.
class BGDisplayFaceBigClock : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    RenderDecision getRenderDecision(const RenderContext& ctx) const override;

private:
    void drawText() const;
};

#endif  // BGDISPLAYFACEBIGCLOCK_H
