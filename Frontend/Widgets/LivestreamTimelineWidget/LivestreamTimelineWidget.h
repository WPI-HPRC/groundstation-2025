//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMTIMELINEWIDGET_H
#define GROUNDSTATION_2025_LIVESTREAMTIMELINEWIDGET_H

#include <QWidget>
#include <QPaintEvent>

class LivestreamTimelineWidget: public QWidget
{
public:
    explicit LivestreamTimelineWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void setMaxValue(float newMaxValue);
    void updateValue(float newValue);

    float currentValue;
    float maxValue;
};


#endif //GROUNDSTATION_2025_LIVESTREAMTIMELINEWIDGET_H
