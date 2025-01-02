//
// Created by William Scheirey on 1/1/25.
//

#include "JSInterface.h"
#include <QDebug>

void JsInterface::payloadPoint(double lat, double lng) {
    emit updatePayloadPoint(lat, lng);
}

void JsInterface::log(const QString& str) {
    qDebug() << "Log from JS: " << str;
}

void JsInterface::targetPoint(double lat, double lng) {
    emit updateTargetPoint(lat, lng);
}