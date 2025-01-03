//
// Created by William Scheirey on 1/1/25.
//

#ifndef GS_BACKEND_2024_2025_JSINTERFACE_H
#define GS_BACKEND_2024_2025_JSINTERFACE_H
#include <QObject>

class JsInterface: public QObject
{
    Q_OBJECT
public slots:
    void log(const QString& str);
    void newPayloadPosition(double lat, double lng);

signals:
    void updatePayloadPoint(double lat, double lng);
};


#endif //GS_BACKEND_2024_2025_JSINTERFACE_H
