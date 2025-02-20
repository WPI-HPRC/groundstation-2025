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

    float value;
    float maxValue;

    int lastHeight;

public slots:
    void paintEvent(QPaintEvent *paintEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
};


#endif //GROUNDSTATION_2025_GAUGEWIDGET_H
