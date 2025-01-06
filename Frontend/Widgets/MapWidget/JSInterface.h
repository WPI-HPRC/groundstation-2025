//
// Created by William Scheirey on 1/1/25.
//

#ifndef GS_BACKEND_2024_2025_JSINTERFACE_H
#define GS_BACKEND_2024_2025_JSINTERFACE_H
#include <QObject>

class JsInterface: public QObject
{

public:
    void setMapName(const QString &name);
    void doReset();

    Q_OBJECT
public slots:
    void log(const QString& str);
    void newPayloadPosition(double lat, double lng);

signals:
    void updatePayloadPoint(const QString &, double, double);
    void setMap(const QString &);
    void reset();
};


#endif //GS_BACKEND_2024_2025_JSINTERFACE_H
