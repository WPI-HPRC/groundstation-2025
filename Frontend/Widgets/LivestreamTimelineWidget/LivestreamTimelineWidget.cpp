//
// Created by William Scheirey on 2/20/25.
//

#include "LivestreamTimelineWidget.h"
#include "Backend/Backend.h"
#include <QPainter>

LivestreamTimelineWidget::LivestreamTimelineWidget(QWidget *parent): QWidget(parent)
{
    maxValue = 5000;
}

void LivestreamTimelineWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = event->rect();

    float penSize = 30;

    float margin = rect.height()/20;
    float x = rect.width()*0.5;
    float maxHeight = rect.height() - 2*margin;

    QPen bgPen(QColor(160, 160, 160), penSize);
    bgPen.setCapStyle(Qt::SquareCap);
    painter.setPen(bgPen);
    painter.drawLine(x, rect.height()-margin, x, margin);

    QPen fillPen(QColor(250, 250, 250), penSize);
    fillPen.setCapStyle(Qt::SquareCap);
    painter.setPen(fillPen);
    painter.drawLine(x, rect.height()-margin, x, rect.height()-margin - 1 - maxHeight * qMax(0.f,currentValue)/maxValue);

    QWidget::paintEvent(event);
}

void LivestreamTimelineWidget::setMaxValue(float newMaxValue)
{
    maxValue = newMaxValue;
    repaint();
}

void LivestreamTimelineWidget::updateValue(float newValue)
{
    currentValue = newValue;
    repaint();
}