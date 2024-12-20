//
// Created by William Scheirey on 8/20/24.
//

#include "DataLogger.h"
#include "QDateTime"
#include <QJsonDocument>
#include <iostream>
#include <utility>
#include <QDesktopServices>

//#define OFFICIAL_TEST

QString DataLogger::enclosingDirectory = Constants::LogDirPath;

DataLogger::DataLogger(QString dirPrefix, bool needFiles)
{
    needToCreateFiles = needFiles;
    directoryPrefix = std::move(dirPrefix);
    createFiles();
}

void DataLogger::createDirectory(const QString &dirName)
{
    logDir.setPath(DataLogger::enclosingDirectory);
    logDir.setPath(logDir.path().append("/").append(dirName));

    if (logDir.path().startsWith("~"))
    {
        logDir.setPath(logDir.path().replace("~", QDir::homePath()));
    }

    if (!logDir.exists())
    {
        if (logDir.mkpath("."))
        {
//            std::cout << "Created log directory at " << logDir.path().toStdString() << std::endl;
        }
        else
        {
            std::cout << "Failed to create log directory at " << logDir.path().toStdString() << std::endl;
        }
    }
#if DEBUG_CSV
    else
    {
        qDebug("Log directory already exists");
    }
#endif
}

void DataLogger::createFiles()
{
    QDateTime currentTime = QDateTime::currentDateTime();

    QString timeString = currentTime.toString(Constants::LogTimeFormat);

    createDirectory(directoryPrefix.isEmpty() ? timeString : directoryPrefix.append("/"));

    if (needToCreateFiles)
    {
#ifndef OFFICIAL_TEST
        rocketLogFile.open(logDir.path().append("/").append(timeString).append("_rocket.csv"));
        payloadLogFile.open(logDir.path().append("/").append(timeString).append("_payload.csv"));
        transmitStatusLog.open(logDir.path().append("/").append(timeString).append("_transmit_status.csv"));
        linkTestLogFile.open(logDir.path().append("/").append(timeString).append("_linkTest.csv"));
        throughputTestLogFile.open(logDir.path().append("/").append(timeString).append("_throughputTest.csv"));

        byteLog.setFileName(logDir.path().append("/").append(timeString).append("_bytes.txt"));
        byteLog.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text);

        textLog.setFileName(logDir.path().append("/").append(timeString).append("_log.txt"));
        textLog.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text);
#else
        rocketLogFile.open(logDir.path().append("/").append(timeString).append("_rocket_OFFICIAL.csv"));
        payloadLogFile.open(logDir.path().append("/").append(timeString).append("_payload_OFFICIAL.csv"));
        transmitStatusLog.open(logDir.path().append("/").append(timeString).append("_transmit_status_OFFICIAL.csv"));
        linkTestLogFile.open(logDir.path().append("/").append(timeString).append("_linkTest_OFFICIAL.csv"));
        throughputTestLogFile.open(logDir.path().append("/").append(timeString).append("_throughputTest_OFFICIAL.csv"));
        byteLog.setFileName(logDir.path().append("/").append(timeString).append("_bytes_OFFICIAL.txt"));
        byteLog.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text);

        textLog.setFileName(logDir.path().append("/").append(timeString).append("_log_OFFICIAL.txt"));
        textLog.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text);
#endif
    }

}

void DataLogger::openLogsDefault()
{
    QDesktopServices::openUrl(QUrl("file://" + rocketLogFile.file.fileName(), QUrl::TolerantMode));
    QDesktopServices::openUrl(QUrl("file://" + payloadLogFile.file.fileName(), QUrl::TolerantMode));
    QDesktopServices::openUrl(QUrl("file://" + byteLog.fileName(), QUrl::TolerantMode));
}

void DataLogger::showFolder()
{
    QDesktopServices::openUrl("file://" + logDir.path());
}

void DataLogger::logLinkTest(const QJsonObject &jsonData)
{
    linkTestLogFile.write(jsonData);
    linkTestLogFile.file.flush();
}

void DataLogger::logThroughputTest(const QJsonObject &jsonData)
{
    throughputTestLogFile.write(jsonData);
    throughputTestLogFile.file.flush();
}

void DataLogger::logTransmitStatus(const QJsonObject &jsonData)
{
    transmitStatusLog.write(jsonData);
    transmitStatusLog.file.flush();
}

void DataLogger::writeToByteFile(const char *text, size_t size)
{
    byteLog.write(text, (qint64)size);
}

void DataLogger::writeToByteFile(const QString &str)
{
    writeToByteFile(str.toStdString().c_str(), str.toStdString().length());
}

void DataLogger::flushByteFile()
{
    byteLog.flush();
}

void DataLogger::writeToTextFile(const char *text, size_t size)
{
    textLog.write(text, (long long) size);
}

void DataLogger::writeToTextFile(const QString &str)
{
    writeToTextFile(str.toStdString().c_str(), str.toStdString().length());
}


void DataLogger::flushTextFile()
{
    textLog.flush();
}

void DataLogger::flushDataFiles()
{
    rocketLogFile.file.flush();
    payloadLogFile.file.flush();
}

void DataLogger::writeData(const QJsonObject &jsonData, GroundStation::PacketType packetType)
{
    switch (packetType)
    {
        case GroundStation::Rocket:
            rocketLogFile.write(jsonData);
            break;
        case GroundStation::Payload:
            payloadLogFile.write(jsonData);
            break;
        default:
            return;
    }
//    flushDataFiles();
}

void DataLogger::dataReady(const char *data, GroundStation::PacketType packetType)
{
    writeData(QJsonDocument::fromJson(data).object(), packetType);
}

void DataLogger::dataReady(const char *data, GroundStation::PacketType packetType, uint8_t rssi)
{
    QJsonObject json = QJsonDocument::fromJson(data).object();
    json.insert("rssi", -1 * (int) rssi);
    writeData(json, packetType);
}