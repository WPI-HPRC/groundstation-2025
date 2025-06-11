//
// Created by William Scheirey on 6/10/25.
//

#ifndef GROUNDSTATION_2025_POINTERCONNECTIONINDICATOR_H
#define GROUNDSTATION_2025_POINTERCONNECTIONINDICATOR_H

#include "Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h"

class PointerConnectionIndicator: public LedWidget
{
Q_OBJECT
public:
    explicit PointerConnectionIndicator(QWidget *parent = nullptr);
};


#endif //GROUNDSTATION_2025_POINTERCONNECTIONINDICATOR_H
