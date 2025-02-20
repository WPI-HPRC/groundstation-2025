//
// Created by William Scheirey on 2/19/25.
//

#ifndef GROUNDSTATION_2025_GAUGEWIDGET_H
#define GROUNDSTATION_2025_GAUGEWIDGET_H

#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPaintEvent>

class GaugeWidget: public QWidget
{
public:
    explicit GaugeWidget(float maxValue, QWidget *parent = nullptr);
    void updateValue(float newValue);

    void paintEvent(QPaintEvent *paintEvent);

    float value;
    float maxValue;
};


#endif //GROUNDSTATION_2025_GAUGEWIDGET_H
