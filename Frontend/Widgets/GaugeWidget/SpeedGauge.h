//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_SPEEDGAUGE_H
#define GROUNDSTATION_2025_SPEEDGAUGE_H

#include "Frontend/Widgets/GaugeWidget/gaugedisplay.h"
#include "Frontend/Widgets/GaugeWidget/GaugeWidget.h"

class SpeedGauge: public GaugeDisplay
{
public:
    explicit SpeedGauge(QWidget *parent = nullptr);
    float currentValue;

private:
    GaugeWidget *gauge;
};


#endif //GROUNDSTATION_2025_SPEEDGAUGE_H
