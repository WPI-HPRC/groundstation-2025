//
// Created by William Scheirey on 2/19/25.
//

#ifndef GROUNDSTATION_2025_ACCELERATIONGAUGE_H
#define GROUNDSTATION_2025_ACCELERATIONGAUGE_H

#include "Frontend/Widgets/GaugeWidget/gaugedisplay.h"
#include "Frontend/Widgets/GaugeWidget/GaugeWidget.h"


class AccelerationGauge: public GaugeDisplay
{
public:
    explicit AccelerationGauge(QWidget *parent = nullptr);
    float currentValue;

private:
    GaugeWidget *gauge;
};


#endif //GROUNDSTATION_2025_ACCELERATIONGAUGE_H
