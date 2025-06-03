//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_TIMEWIDGETFRAME_H
#define GROUNDSTATION_2025_TIMEWIDGETFRAME_H

#include <QFrame>
#include <QPaintEvent>
#include <QFont>

class TimeWidgetFrame: public QFrame
{
public:
    explicit TimeWidgetFrame(QWidget *parent = nullptr);

    QFont *font;
    int64_t currentTime = -1;
    void paintEvent(QPaintEvent *event);
};

#endif //GROUNDSTATION_2025_TIMEWIDGETFRAME_H
