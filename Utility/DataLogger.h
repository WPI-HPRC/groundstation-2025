//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_DATALOGGER_H
#define GS_BACKEND_2024_2025_DATALOGGER_H

#include "QFile"
#include "QJsonObject"
#include "QDir"
#include <iostream>
#include "../Constants.h"
#include "Utility.h"
#include "generated/telemetry/Packet.pb.h"

#define DEBUG_CSV false

class CSVWriter
{
public:
    QFile file;

    void open(const QString &fileName)
    {
        file.setFileName(fileName);

#if DEBUG_CSV
        if (!file.open(QIODeviceBase::NewOnly))
        {
            qDebug() << "Failed to open file: " << file.fileName();
        }
        else
        {
            qDebug() << "Opened File: " << file.fileName();
        }
#else
        file.open(QIODevice::NewOnly);
#endif
    }

    void write(const QJsonObject &jsonData)
    {
        QList<QString> headers = jsonData.keys();

        if (!hasWrittenHeaders)
        {
            _write(headers.join(",").append(",internalTimestamp").append("\n"));
            hasWrittenHeaders = true;
        }

        QString valueLine = "";

        for (const QString& value: headers)
        {
            valueLine.append(toJsonString(jsonData.value(value)).append(","));
        }
        valueLine.append(QString::asprintf("%lld", QDateTime::currentMSecsSinceEpoch()));
        valueLine.append("\n");

        _write(valueLine);
    }

private:
    bool hasWrittenHeaders;

    void _write(const QString &data)
    {
#if DEBUG_CSV
        QByteArray byteArray = data.toUtf8();
        qint64 bytesWritten = file.write(byteArray);
        if (bytesWritten != byteArray.length())
        {
            qDebug() << "Mismatch in bytes written. Length: " << byteArray.length() << ", written: " << bytesWritten;
        }
        else
        {
            qDebug() << "Successfully wrote " << bytesWritten << " bytes";
        }
#else
        file.write(data.toUtf8());
#endif

    }

    static QString toJsonString(const QJsonValue &value)
    {
        if (value.isDouble())
        {
            double number = value.toDouble();
            if (number == static_cast<int>(number))
            {
                return QString::number(static_cast<int>(number)); // Return as integer if it's a whole number
            }
            else
            {
                return QString::number(number, 'f', 10); // Format with decimals if it's not a whole number
            }
        }
        else if (value.isString())
        {
            return value.toString();
        }
        else if (value.isNull() || value.isUndefined())
        {
            return {}; // Return empty string for null or undefined values
        }
        else if (value.isBool())
        {
            return QString::number(value.toBool() ? 1 : 0);
        }
        else
        {
            // Handle other types if needed
            return {"Unsupported type"};
        }
    }
};

class DataLogger
{
public:

    explicit DataLogger(QString dirPrefix = "", bool needFiles = true);

    void writeTelemetry(const HPRC::Packet& packet);

    void writeTelemetry(const HPRC::Packet& packet, uint8_t rssi);

    void writeToByteFile(const char *text, size_t size);

    void writeToByteFile(const QString &str);

    void logLinkTest(const QJsonObject &jsonData);

    void logThroughputTest(const QJsonObject &jsonData);

    void logTransmitStatus(const QJsonObject &jsonData);

    void flushByteFile();

    void writeToTextFile(const char *text, size_t size);

    void writeToTextFile(const QString &str);

    void logTrackerData(const QJsonObject &jsonData);

    void logAprsData(const QJsonObject &jsonData);

    void flushTextFile();

    void flushDataFiles();

    static QString enclosingDirectory;

    QDir logDir;

    QString directoryPrefix;

    void openLogsDefault();
    void showFolder();

private:
    void createDirectory(const QString &dirName);
    void createFiles();

    void _writeTelemetry(const QJsonObject &jsonData, const HPRC::Packet& packet);

    bool needToCreateFiles;

    CSVWriter rocketLogFile;
    CSVWriter payloadLogFile;

    CSVWriter linkTestLogFile;
    CSVWriter throughputTestLogFile;

    CSVWriter transmitStatusLog;

    CSVWriter trackerLog;

    CSVWriter aprsLog;

    QFile byteLog;
    QFile textLog;
};

#endif //GS_BACKEND_2024_2025_DATALOGGER_H
