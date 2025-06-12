//
// Created by William Scheirey on 2/8/25.
//

#include "Backend/APRS/KissClient.h"
#include "Backend/Backend.h"

KissClient::KissClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &KissClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &KissClient::onDataReceived);
    connect(socket, &QTcpSocket::errorOccurred, this, &KissClient::onError);
    connect(socket, &QTcpSocket::disconnected, this, &KissClient::onDisconnected);
}

void KissClient::start()
{
    if(isConnecting || isConnected)
    {
        return;
    }
//    qDebug() << "Connecting to Dire Wolf KISS TCP server...";
    isConnecting = true;
    socket->connectToHost("localhost", 8001);
}

void KissClient::onConnected()
{
//    qDebug() << "Connected to Dire Wolf KISS TCP server.";
    isConnecting = false;
    isConnected = true;
}

void KissClient::onDisconnected()
{
    isConnecting = false;
    isConnected = false;
}

void KissClient::onDataReceived()
{
    QByteArray data = socket->readAll();

//    emit output(QString::fromUtf8(data));

    if (data.startsWith((char)0xC0) && data.endsWith((char)0xC0)) {
        QByteArray payload = data.mid(1, data.size() - 2); // Remove KISS frame markers
        currentOutput = {};
        decodeAx25(payload);

        QJsonObject json;
        json.insert("Callsign", currentOutput.callsign);
        json.insert("ID", QString::number(currentOutput.ID));
        json.insert("GPS Lock", currentOutput.data.gpsLock ? "YES" : "NO");
        json.insert("Satellites", QString::number(currentOutput.data.satellites));
        json.insert("Altitude", QString::number(currentOutput.data.altitude));
        json.insert("Longitude", QString::number(currentOutput.data.longitude));
        json.insert("Latitude", QString::number(currentOutput.data.latitude));
        Backend::getInstance().dataLogger.logAprsData(json);

        emit output(currentOutput);
    }
}

void KissClient::onError(QTcpSocket::SocketError error)
{
    isConnecting = false;
    isConnected = false;
    socket->close();
    if (error != QTcpSocket::RemoteHostClosedError)
    {
        qDebug() << "Socket error:" << socket->errorString();
    }
}

void KissClient::decodeAx25(const QByteArray &data) {
    if (data.size() < 16) {
        qDebug() << "Packet too short for AX.25.";
        return;
    }

    // Decode source and destination callsigns
    QString sourceCallsign = decodeCallsign(data.mid(8, 7));

//    qDebug() << "APRS Packet From:" << sourceCallsign;

    // Extract the first F0 (PID) index to find the start of APRS data
    int f0Index = data.indexOf((char)0xF0);
    if (f0Index == -1 || f0Index + 1 >= data.size()) {
        // No APRS payload
        return;
    }

    QByteArray aprsData = data.mid(f0Index + 1);  // Extract payload after F0

    parseAprsPayload(aprsData);
}

QString KissClient::decodeCallsign(QByteArray callsignData) {
    if (callsignData.size() != 7) return "INVALID";

    QString callsign;
    for (int i = 0; i < 6; i++) {
        char c = (callsignData[i] >> 1) & 0x7F; // Shift right 1 bit
        if (c > 32 && c < 127) {
            callsign += c; // Valid ASCII character
        }
    }

    currentOutput.callsign = callsign;

    // Decode SSID (last byte, bits 1-4)
    int ssid = (callsignData[6] >> 1) & 0x0F;
    if (ssid > 0) {
        callsign += QString("-%1").arg(ssid);
    }
    currentOutput.ID = ssid;

    return callsign;
}

void KissClient::parseAprsPayload(const QByteArray &aprsData) {
    QString payload = QString::fromUtf8(aprsData);
    QRegularExpression regex(R"(\s*(\d{2})(\d+\.\d+)([NS])[/\\](\d{3})(\d+\.\d+)([EW]))");

    // Extract Altitude from the "A=" field (Altitude in meters)
    QRegularExpression altitudeRegex(R"(\s*A=([0-9]+))");
    QRegularExpressionMatch altitudeMatch = altitudeRegex.match(payload);
    if (altitudeMatch.hasMatch()) {
        int altitude = altitudeMatch.captured(1).toInt();
        currentOutput.data.altitude = altitude;
    } else {
        currentOutput.data.altitude = 0;
        qDebug() << "No altitude information found in APRS packet";
    }

    QRegularExpressionMatch match = regex.match(payload);
    if (match.hasMatch()) {
        float latitude = convertDMS(match.captured(1).toInt(), match.captured(2).toDouble(), match.captured(3) == "S");
        float longitude = convertDMS(match.captured(4).toInt(), match.captured(5).toDouble(), match.captured(6) == "W");

        currentOutput.data.latitude = latitude;
        currentOutput.data.longitude = longitude;
    } else {
        qDebug() << "No valid APRS location found.";
    }

    // Check if there is a GPS lock and satellite count in the APRS payload
    QRegularExpression gpsLockRegex(R"(\s*A=([0-9]+)\s+L(\d+))");
    QRegularExpressionMatch gpsMatch = gpsLockRegex.match(payload);
    if (gpsMatch.hasMatch()) {
        bool gpsLock = gpsMatch.captured(1).toInt() > 1;
        int satelliteCount = gpsMatch.captured(2).toInt();

        currentOutput.data.gpsLock = gpsLock;
        currentOutput.data.satellites = satelliteCount;
    } else {
        qDebug() << "No GPS lock or satellite info found.";
    }
}

float KissClient::convertDMS(int degrees, double minutes, bool isNegative) {
    float decimal = degrees + (minutes / 60.0);
    return isNegative ? -decimal : decimal;
}