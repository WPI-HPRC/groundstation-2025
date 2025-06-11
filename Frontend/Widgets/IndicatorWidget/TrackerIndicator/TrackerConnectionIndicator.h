//
// Created by William Scheirey on 6/10/25.
//

#ifndef GROUNDSTATION_2025_TRACKERCONNECTIONINDICATOR_H
#define GROUNDSTATION_2025_TRACKERCONNECTIONINDICATOR_H

#include "Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h"

class TrackerConnectionIndicator: public LedWidget
{
Q_OBJECT
public:
    explicit TrackerConnectionIndicator(QWidget *parent = nullptr);
};


#endif //GROUNDSTATION_2025_TRACKERCONNECTIONINDICATOR_H
