//
// Created by William Scheirey on 2/8/25.
//

#ifndef GROUNDSTATION_2025_DIREWOLFPROCESS_H
#define GROUNDSTATION_2025_DIREWOLFPROCESS_H

#include <QObject>
#include <QProcess>

class DirewolfProcess: public QObject
{
    Q_OBJECT
public:
    explicit DirewolfProcess(QObject *parent = nullptr);
    void start();
    void stop();
    QProcess process;
    QString frequency = "434.55M";

signals:
    void direwolfOutput(QString);

private:
    QString originalScriptContents;
    QString direwolfConfig;
};


#endif //GROUNDSTATION_2025_DIREWOLFPROCESS_H
