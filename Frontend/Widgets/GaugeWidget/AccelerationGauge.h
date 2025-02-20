//
// Created by William Scheirey on 2/19/25.
//

#ifndef GROUNDSTATION_2025_ACCELERATIONGAUGE_H
#define GROUNDSTATION_2025_ACCELERATIONGAUGE_H

#include "Frontend/Widgets/GaugeWidget/GaugeWidget.h"

class AccelerationGauge: public GaugeWidget
{
public:
    explicit AccelerationGauge(QWidget *parent = nullptr);
};


#endif //GROUNDSTATION_2025_ACCELERATIONGAUGE_H
