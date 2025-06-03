//
// Created by William Scheirey on 5/24/25.
//

#ifndef GROUNDSTATION_2025_CIRCLEPOSITIONWIDGET_H
#define GROUNDSTATION_2025_CIRCLEPOSITIONWIDGET_H

#include <QWidget>
#include <QPaintEvent>

class CirclePositionWidget: public QWidget
{
public:
    explicit CirclePositionWidget(QWidget *parent = nullptr);
    void updateValue(float newValue);
    void updateTargetValue(float newTargetValue);

    float value;
    float targetValue;

    int lastHeight;

public slots:
    void paintEvent(QPaintEvent *paintEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
};


#endif //GROUNDSTATION_2025_CIRCLEPOSITIONWIDGET_H
