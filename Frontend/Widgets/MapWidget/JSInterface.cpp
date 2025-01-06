//
// Created by William Scheirey on 1/1/25.
//

#include "JSInterface.h"
#include <QDebug>

void JsInterface::newPayloadPosition(double lat, double lng) {
    emit updatePayloadPoint("payload", lat, lng);
}

void JsInterface::log(const QString& str) {
    qDebug() << "Log from JS: " << str;
}

void JsInterface::setMapName(const QString &name)
{
    emit setMap(name);
}

void JsInterface::doReset()
{
    emit reset();
}