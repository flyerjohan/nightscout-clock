#ifndef BGDISPLAYFACEWEATHER_H
#define BGDISPLAYFACEWEATHER_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// The weather in Valencia: an icon for the sky and the temperature. A line under the temperature takes
// the glucose range color, so the glucose can still be told at a glance.
class BGDisplayFaceWeather : public BGDisplayFaceTextBase {
public:
    void showReadings(const GlucoseReadings& readings, bool dataIsOld = false) const override;
    void showNoData() const override;

private:
    void drawWeather() const;
    void drawGlucoseLine(uint16_t color) const;
};

#endif  // BGDISPLAYFACEWEATHER_H
