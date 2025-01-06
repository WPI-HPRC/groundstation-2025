//
// Created by William Scheirey on 1/1/25.
//

#include "JSInterface.h"
#include <QDebug>

void JsInterface::chooseMap()
{
    if(mapHasBeenChosen)
    {
        return;
    }
    emit autoChooseMap();
}

void JsInterface::mapChosen(const QString &name)
{
    mapHasBeenChosen = true;
    emit mapWasChosen(name);
}

void JsInterface::newPayloadPosition(double lat, double lng) {
    emit addPoint("payload", lat, lng);
    chooseMap();
}

void JsInterface::newRocketPosition(double lat, double lng) {
    emit addPoint("rocket", lat, lng);
    chooseMap();
}

void JsInterface::log(const QString& str) {
    qDebug() << "Log from JS: " << str;
}

void JsInterface::setMapName(const QString &name)
{
    mapHasBeenChosen = true;
    emit setMap(name);
}

void JsInterface::doReset()
{
    emit reset();
}