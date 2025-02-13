//
// Created by William Scheirey on 2/12/25.
//

#ifndef GROUNDSTATION_2025_FOURAXISGRAPH_H
#define GROUNDSTATION_2025_FOURAXISGRAPH_H

#include "Frontend/Widgets/GraphWidget/GraphWidget.h"
#include "Backend/Backend.h"

class FourAxisGraph: public GraphWidget
{
public:
    explicit FourAxisGraph(const QString &name, int range, QGraphicsItem *parent = nullptr): GraphWidget(name, range, parent)
    {
        addSeriesCustom("i");
        addSeriesCustom("j");
        addSeriesCustom("k");
        addSeriesCustom("w");
    }
};

#endif //GROUNDSTATION_2025_FOURAXISGRAPH_H
