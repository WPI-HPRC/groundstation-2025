//
// Created by William Scheirey on 2/20/25.
//

#include "TimeWidgetFrame.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include <Backend/Backend.h>

TimeWidgetFrame::TimeWidgetFrame(QWidget *parent): QFrame(parent)
{
    int id = QFontDatabase::addApplicationFont(":/Fonts/centurygothic.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(id).at(0);
    font = new QFont(fontName, 30);

    connect(&Backend::getInstance(), &Backend::newGroundFlightTime, this, [this](uint32_t time)
    {
        if(time - currentTime > 1000)
        {
            currentTime = time;
            repaint();
        }
    });
}

void TimeWidgetFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int triangleWidth = 40;
    int height = 50;
    int boxWidth = 250;

    QRectF rect = QRectF(event->rect().width() - boxWidth, 0, boxWidth, height);
    painter.fillRect(rect, QColor(172, 43, 55));

    QPainterPath path;
    path.moveTo(event->rect().width() - boxWidth, 0);
    path.lineTo(event->rect().width() - boxWidth - triangleWidth, height*0.5);
    path.lineTo(event->rect().width() - boxWidth, height);

    painter.fillPath(path, QBrush(QColor(172, 43, 55)));

    painter.setFont(*font);
    painter.setPen(QColor(255, 255, 255));

    QString text = "LAUNCH SOON";

    if(currentTime != -1)
    {
        uint_fast8_t minutes = currentTime / (60*1000) % 60;
        uint_fast8_t seconds = currentTime / 1000 % 60;
        uint_fast16_t milliseconds = currentTime % 1000;
        text = QString::asprintf("T+%02d:%02d", minutes, seconds);
    }

    painter.drawText(rect,  Qt::AlignCenter, text);
}