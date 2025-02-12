//
// Created by William Scheirey on 2/12/25.
//

#ifndef GROUNDSTATION_2025_THREEAXISGRAPH_H
#define GROUNDSTATION_2025_THREEAXISGRAPH_H

#include "Frontend/Widgets/GraphWidget/GraphWidget.h"
#include "Backend/Backend.h"

class ThreeAxisGraph: public GraphWidget
{
public:
    explicit ThreeAxisGraph(const QString &name, int range, QGraphicsItem *parent = nullptr): GraphWidget(name, range, parent)
    {
        addSeriesCustom("x");
        addSeriesCustom("y");
        addSeriesCustom("z");
    }
};

#endif //GROUNDSTATION_2025_THREEAXISGRAPH_H
