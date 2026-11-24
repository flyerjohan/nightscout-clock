#ifndef BGDISPLAYFACENIGHT_H
#define BGDISPLAYFACENIGHT_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// For the night: the value and its trend arrow in red, the one LED channel that lights least and disturbs
// sleep least, with nothing moving. Out of range, the number and the arrow take the range color, so a low or
// high is still readable at a glance. Meant to run at the lowest brightness from the daily schedule.
class BGDisplayFaceNight : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
};

#endif  // BGDISPLAYFACENIGHT_H
