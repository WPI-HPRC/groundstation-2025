//
// Created by William Scheirey on 6/9/25.
//

#ifndef GROUNDSTATION_2025_AZIMUTHWIDGET_H
#define GROUNDSTATION_2025_AZIMUTHWIDGET_H
#include "Frontend/Widgets/TrackerPositionWidget/CirclePositionWidget.h"

class AzimuthWidget: public CirclePositionWidget
{
public:
    explicit AzimuthWidget(QWidget *parent = nullptr);
};


#endif //GROUNDSTATION_2025_AZIMUTHWIDGET_H
