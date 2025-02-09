//
// Created by William Scheirey on 2/8/25.
//

#ifndef GROUNDSTATION_2025_APRSHANDLER_H
#define GROUNDSTATION_2025_APRSHANDLER_H

#include <QObject>
#include "Backend/APRS/DirewolfProcess.h"
#include "Backend/APRS/KissClient.h"

class APRSHandler: public QObject
{
    Q_OBJECT
public:
    explicit APRSHandler(QObject *parent = nullptr);

    void stop();
    void start();

    DirewolfProcess direwolfProcess;
    KissClient kissClient;

public slots:
    void direwolfOutput(QString output);
};


#endif //GROUNDSTATION_2025_APRSHANDLER_H
