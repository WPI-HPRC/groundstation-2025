//
// Created by William Scheirey on 2/8/25.
//

#include "DirewolfProcess.h"
#include <QApplication>
#include <QFile>

DirewolfProcess::DirewolfProcess(QObject *parent): QObject(parent)
{
    process.setProgram("bash");

    QFile script(":/APRS/run_aprs.sh");
    script.open(QIODevice::ReadOnly | QIODevice::Text);
    originalScriptContents = script.readAll();

    QFile direWolfFile(":/APRS/direwolf.conf");
    direWolfFile.open(QIODevice::ReadOnly | QIODevice::Text);
    direwolfConfig = direWolfFile.readAll();

    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        emit direwolfOutput(process.readAllStandardOutput());
    });

    QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
        emit direwolfOutput(process.readAllStandardOutput());
    });
}

void DirewolfProcess::start()
{
    QString scriptContents = originalScriptContents;
    scriptContents.replace("[FREQ]", frequency);
    scriptContents.replace("direwolf.conf", "/dev/stdin");

    process.setArguments(QStringList() << "-c" << scriptContents);
    process.start();
    process.write(direwolfConfig.toUtf8());
    process.closeWriteChannel();
}

void DirewolfProcess::stop()
{
    process.close();
}