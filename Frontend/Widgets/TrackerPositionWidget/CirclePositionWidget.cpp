//
// Created by William Scheirey on 5/24/25.
//

#include "CirclePositionWidget.h"
#include <QPainter>
#include <cmath>
#include "Tracker/Tracker.h"

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

    float size = (float)qMin(paintEvent->rect().width(), paintEvent->rect().height()) * 0.9;

    float left = paintEvent->rect().left() + 0.5*this->width() - 0.5*size;

    float top = paintEvent->rect().top() + 0.5*this->height() - 0.5*size;

    QRectF rect(left, top, size, size);

    float sizeR = sqrt(2)*size;

    float valueRadius = size / 20;

    QBrush bgBrush(QColor(150,150,150));
    painter.setBrush(bgBrush);
    painter.drawEllipse(rect);

    painter.setPen(QColor(172, 43, 55));
    painter.setBrush(QBrush(QColor(172,43,55)));

    float R = sizeR/2 - valueRadius*6;

    QPointF valuePosition = rect.center() + QPointF(R * cos(value*M_PI/180), -R*sin(value*M_PI/180));
    painter.drawEllipse(valuePosition, valueRadius, valueRadius);

    float desiredRadius = size / 15;
    float R1 = sizeR/2 - valueRadius*6;

    QPointF desiredPosition = rect.center() + QPointF(R1 * cos(targetValue*M_PI/180), -R1*sin(targetValue*M_PI/180));
    painter.setPen(QColor(255,255,255));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(desiredPosition, desiredRadius, desiredRadius);

    QWidget::paintEvent(paintEvent);
    this->raise();
}

void CirclePositionWidget::resizeEvent(QResizeEvent *resizeEvent)
{
//    qDebug() << "Resize event: " << resizeEvent->size();
}