//
// Created by William Scheirey on 2/19/25.
//

#include "GaugeWidget.h"

GaugeWidget::GaugeWidget(float maxValue, QWidget *parent): maxValue(maxValue)
{
    setMinimumSize(250, 250);
    repaint();
}

void GaugeWidget::updateValue(float newValue)
{
    value = newValue;
    repaint();
}

void GaugeWidget::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen bgPen(QColor(160, 160, 160), 15);
    bgPen.setCapStyle(Qt::RoundCap);
    painter.setPen(bgPen);
    painter.drawArc(QRectF(200, 200, 200, 200), -180*16, (int)(-180*16));

    QPen fgPen(QColor(172, 43, 55), 15);
    fgPen.setCapStyle(Qt::RoundCap);
    painter.setPen(fgPen);
    painter.drawArc(QRectF(200, 200, 200, 200), 180*16, qMax(-180*16, (int)(-180*16 * (value/maxValue)) - 16));
}