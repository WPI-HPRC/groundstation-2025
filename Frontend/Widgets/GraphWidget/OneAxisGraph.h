//
// Created by William Scheirey on 2/12/25.
//

#ifndef GROUNDSTATION_2025_ONEAXISGRAPH_H
#define GROUNDSTATION_2025_ONEAXISGRAPH_H

#include "Frontend/Widgets/GraphWidget/GraphWidget.h"
#include "Backend/Backend.h"

class OneAxisGraph: public GraphWidget
{
public:
    explicit OneAxisGraph(const QString &name, int range, QGraphicsItem *parent = nullptr): GraphWidget(name, range, parent)
    {
        addSeriesCustom("1");
        hideLegend();
    }
};

#endif //GROUNDSTATION_2025_ONEAXISGRAPH_H
