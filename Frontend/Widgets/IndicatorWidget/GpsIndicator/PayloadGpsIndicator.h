//
// Created by William Scheirey on 1/6/25.
//

#ifndef GS_BACKEND_2024_2025_PAYLOADGPSINDICATOR_H
#define GS_BACKEND_2024_2025_PAYLOADGPSINDICATOR_H


#include "Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h"

class PayloadGpsIndicator: public LedWidget
{
Q_OBJECT
public:
    explicit PayloadGpsIndicator(QWidget *parent = nullptr);
};


#endif //GS_BACKEND_2024_2025_PAYLOADGPSINDICATOR_H
