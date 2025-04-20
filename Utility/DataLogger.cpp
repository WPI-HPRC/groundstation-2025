//
// Created by William Scheirey on 8/20/24.
//

#include "DataLogger.h"
#include "QDateTime"
#include <QJsonDocument>
#include <iostream>
#include <utility>
#include <QDesktopServices>


#include <regex>
#include "protobuf/src/google/protobuf/util/json_util.h"
//#define OFFICIAL_TEST

QString DataLogger::enclosingDirectory = Constants::LogDirPath;


QJsonObject qJsonObjectFromPacket(const HPRC::Packet& packet)
{
    std::string str;

    absl::Status status;

    google::protobuf::util::JsonPrintOptions  options;
    options.always_print_fields_with_no_presence = true;

    switch (packet.Message_case())
    {
        case HPRC::Packet::kTelemetry:
        {
            const HPRC::Telemetry& telemetry = packet.telemetry();
            switch (telemetry.Message_case())
            {
                // TODO: Add a check to make sure the length of the packet matches whatever we're casting it into. Do once protobuf is fully integrated
                case HPRC::Telemetry::kRocketPacket:
                {
                    // Convert current packet to JSON
                    status = google::protobuf::util::MessageToJsonString(telemetry.rocketpacket(), &str, options);
                    if (status != absl::OkStatus())
                    {
                        std::cerr << "Error converting rocket packet to JSON string: " << status << std::endl;
                    }
                    break;
                }
                case HPRC::Telemetry::kPayloadPacket:
                {
                    // Convert current packet to JSON
                    status = google::protobuf::util::MessageToJsonString(telemetry.payloadpacket(), &str, options);
                    if (status != absl::OkStatus())
                    {
                        std::cerr << "Error converting payload packet to JSON string: " << status << std::endl;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        default:
        {
            break;
        }
    }


    str = std::regex_replace(str, std::regex("nan"), "0");
    str = std::regex_replace(str, std::regex("inf"), "0");

    return QJsonDocument::fromJson(str.c_str()).object();
}

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

void DataLogger::_writeTelemetry(const QJsonObject &jsonData, const HPRC::Packet& packet)
{
    if(packet.Message_case() != HPRC::Packet::kTelemetry)
        return;

    switch (packet.telemetry().Message_case())
    {
        case HPRC::Telemetry::kRocketPacket:
            rocketLogFile.write(jsonData);
            break;
        case HPRC::Telemetry::kPayloadPacket:
            payloadLogFile.write(jsonData);
            break;
        default:
            return;
    }
//    flushDataFiles();
}

void DataLogger::writeTelemetry(const HPRC::Packet& packet)
{
    _writeTelemetry(qJsonObjectFromPacket(packet), packet);
}

void DataLogger::writeTelemetry(const HPRC::Packet& packet, uint8_t rssi)
{
    QJsonObject json = qJsonObjectFromPacket(packet);
    json.insert("rssi", -1 * (int) rssi);
    _writeTelemetry(json, packet);
}