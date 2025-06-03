//
// Created by William Scheirey on 5/24/25.
//

#include "CirclePositionWidget.h"
#include <QPainter>

CirclePositionWidget::CirclePositionWidget(QWidget *parent): QWidget(parent)
{
    repaint();
    value = 45;
    targetValue = 90;
}
void CirclePositionWidget::updateValue(float newValue)
{
    value = newValue;
    repaint();
}

void CirclePositionWidget::updateTargetValue(float newTargetValue)
{
    targetValue = newTargetValue;
    repaint();
}

void CirclePositionWidget::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    float size = (float)qMin(paintEvent->rect().width(), paintEvent->rect().height());

    float left = paintEvent->rect().left() + 0.5*this->width() - 0.5*size;

    float penSize = 30;

    size -= penSize*2;

    float top = paintEvent->rect().top() + 0.5*this->height() - 0.5*size;

    QRectF rect(left, top, size, size);

    float sizeR = sqrt(2)*size;

    float valueRadius = size / 20;

    QPen bgPen(QColor(100, 100, 100), penSize);
    painter.setPen(bgPen);
    painter.drawEllipse(rect);

    QPen valuePen(QColor(172, 43, 55), penSize + 0.3);
    painter.setPen(QColor(172, 43, 55));
    painter.setBrush(QBrush(QColor(172,43,55)));

    float R = sizeR/2 - penSize + valueRadius/4;

    QPointF valuePosition = rect.center() + QPointF(R * cos(value), -R*sin(value));
    painter.drawEllipse(valuePosition, valueRadius, valueRadius);

    /*
    QPen fillPen(QColor(250, 250, 250), penSize);
    fillPen.setCapStyle(Qt::SquareCap);
    painter.setPen(fillPen);

    painter.drawArc(rect, startAngle*16, qMax(-maxAngle*16, (int)(-maxAngle*16 * (value/maxValue)) - 1));
*/
    QWidget::paintEvent(paintEvent);
    this->raise();
}

void CirclePositionWidget::resizeEvent(QResizeEvent *resizeEvent)
{
//    qDebug() << "Resize event: " << resizeEvent->size();
}