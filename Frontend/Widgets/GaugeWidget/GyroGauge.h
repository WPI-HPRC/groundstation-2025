//
// Created by William Scheirey on 2/19/25.
//

#ifndef GROUNDSTATION_2025_GYROGAUGE_H
#define GROUNDSTATION_2025_GYROGAUGE_H

#include "Frontend/Widgets/GaugeWidget/gaugedisplay.h"
#include "Frontend/Widgets/GaugeWidget/GaugeWidget.h"

class GyroGauge: public GaugeDisplay
{
public:
    explicit GyroGauge(QWidget *parent = nullptr);
    float currentValue;

private:
    GaugeWidget *gauge;
};


#endif //GROUNDSTATION_2025_GYROGAUGE_H
