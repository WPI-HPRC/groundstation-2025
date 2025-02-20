//
// Created by William Scheirey on 2/19/25.
//

#include "GaugeWidget.h"

GaugeWidget::GaugeWidget(float maxValue, QWidget *parent): QWidget(parent), maxValue(maxValue)
{
//    setMinimumSize(250, 250);
    repaint();
}

void GaugeWidget::updateValue(float newValue)
{
    if(abs(value - newValue) > 0.1)
    {
        value = newValue;
        repaint();
    }
}

void GaugeWidget::paintEvent(QPaintEvent *paintEvent)
{
    /*
    if(paintEvent->rect().height() < lastHeight)
    {
        lastHeight = paintEvent->rect().height();
        return;
    }
    lastHeight = paintEvent->rect().height();
     */

//    if(this->size() != paintEvent->rect().size())
//        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int size = qMin(paintEvent->rect().width(), paintEvent->rect().height());

    int left = paintEvent->rect().left() + 0.5*this->width() - 0.5*size;

    float penSize = qMax(25, size/15);

    size -= penSize*2;

    int top = paintEvent->rect().bottom() - 0.8*size;

    QRectF rect(left+penSize, top, size, size);

    int startAngle = 200; // [deg]
    int maxAngle = 220; // [deg]

    QPen bgPen(QColor(160, 160, 160), penSize);
    bgPen.setCapStyle(Qt::SquareCap);
    painter.setPen(bgPen);
    painter.drawArc(rect, startAngle*16, (int)(-maxAngle*16));

    QPen fgPen(QColor(172, 43, 55), penSize + 0.3);
    fgPen.setCapStyle(Qt::SquareCap);
    painter.setPen(fgPen);
    painter.drawArc(rect, (startAngle-maxAngle)*16, 20*16);

    QPen fillPen(QColor(250, 250, 250), penSize);
    fillPen.setCapStyle(Qt::SquareCap);
    painter.setPen(fillPen);

    painter.drawArc(rect, startAngle*16, qMax(-maxAngle*16, (int)(-maxAngle*16 * (value/maxValue)) - 1));

    QWidget::paintEvent(paintEvent);
}

void GaugeWidget::resizeEvent(QResizeEvent *resizeEvent)
{
//    qDebug() << "Resize event: " << resizeEvent->size();
}