//
// Created by William Scheirey on 6/9/25.
//

#ifndef GROUNDSTATION_2025_ELEVATIONWIDGET_H
#define GROUNDSTATION_2025_ELEVATIONWIDGET_H


#include "Frontend/Widgets/TrackerPositionWidget/CirclePositionWidget.h"

class ElevationWidget: public CirclePositionWidget
{
public:
    explicit ElevationWidget(QWidget *parent = nullptr);
};

#endif //GROUNDSTATION_2025_ELEVATIONWIDGET_H
