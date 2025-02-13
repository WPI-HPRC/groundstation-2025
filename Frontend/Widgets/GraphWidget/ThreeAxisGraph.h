//
// Created by William Scheirey on 2/12/25.
//

#ifndef GROUNDSTATION_2025_THREEAXISGRAPH_H
#define GROUNDSTATION_2025_THREEAXISGRAPH_H

#include "Frontend/Widgets/GraphWidget/GraphWidget.h"
#include <QGraphicsLinearLayout>
#include "Backend/Backend.h"
#include <QTimer>

class ThreeAxisGraph: public GraphWidget
{
public:
    explicit ThreeAxisGraph(const QString &name, int range, QGraphicsItem *parent = nullptr): GraphWidget(name, range, parent)
    {
        addSeriesCustom("x");
        addSeriesCustom("y");
        addSeriesCustom("z");

        this->legend()->setInteractive(false);
        this->legend()->detachFromChart();
        // Need to let things update their size own before we can do anything
        QTimer::singleShot(0, [this]()
        {
            this->legend()->update();
            this->legend()->setGeometry(QRectF(
                    this->plotArea().x(),
                    this->plotArea().top() - 12,
                    this->plotArea().width(),
                    1000
            ));
            this->legend()->setZValue(1000);
            this->legend()->update();
        });
    }
};

#endif //GROUNDSTATION_2025_THREEAXISGRAPH_H
