//
// Created by William Scheirey on 1/1/25.
//

#include "JSInterface.h"
#include <QDebug>

void JsInterface::newPayloadPosition(double lat, double lng) {
    emit updatePayloadPoint(lat, lng);
}

void JsInterface::log(const QString& str) {
    qDebug() << "Log from JS: " << str;
}