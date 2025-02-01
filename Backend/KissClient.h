#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QRegularExpression>

class KissClient : public QObject {
Q_OBJECT
public:
    KissClient(QObject *parent = nullptr) : QObject(parent) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &KissClient::onConnected);
        connect(socket, &QTcpSocket::readyRead, this, &KissClient::onDataReceived);
        connect(socket, &QTcpSocket::errorOccurred, this, &KissClient::onError);

        qDebug() << "Connecting to Dire Wolf KISS TCP server...";
        socket->connectToHost("localhost", 8001);
    }

private slots:
    void onConnected() {
        qDebug() << "Connected to Dire Wolf KISS TCP server.";
    }

    void onDataReceived() {
        QByteArray data = socket->readAll();
//        qDebug() << "Received raw KISS data:" << data.toHex(' ');

        if (data.startsWith((char)0xC0) && data.endsWith((char)0xC0)) {
            QByteArray payload = data.mid(1, data.size() - 2); // Remove KISS frame markers
            decodeAx25(payload);
        }
    }

    void onError(QTcpSocket::SocketError error) {
        qDebug() << "Socket error:" << socket->errorString();
    }

private:
    QTcpSocket *socket;

    void decodeAx25(const QByteArray &data) {
        if (data.size() < 16) {
            qDebug() << "Packet too short for AX.25.";
            return;
        }

        // Decode source and destination callsigns
        QString sourceCallsign = decodeCallsign(data.mid(8, 7));

        qDebug() << "APRS Packet From:" << sourceCallsign;

        // Extract the first F0 (PID) index to find the start of APRS data
        int f0Index = data.indexOf((char)0xF0);
        if (f0Index == -1 || f0Index + 1 >= data.size()) {
//            qDebug() << "No valid APRS payload found.";
            return;
        }

        QByteArray aprsData = data.mid(f0Index + 1);  // Extract payload after F0
//        qDebug() << "Raw APRS Payload:" << aprsData;

        parseAprsPayload(aprsData);
        qDebug() << "----------";
    }

    QString decodeCallsign(QByteArray callsignData) {
        if (callsignData.size() != 7) return "INVALID";

        QString callsign;
        for (int i = 0; i < 6; i++) {
            char c = (callsignData[i] >> 1) & 0x7F; // Shift right 1 bit
            if (c > 32 && c < 127) {
                callsign += c; // Valid ASCII character
            }
        }

        // Decode SSID (last byte, bits 1-4)
        int ssid = (callsignData[6] >> 1) & 0x0F;
        if (ssid > 0) {
            callsign += QString("-%1").arg(ssid);
        }

        return callsign;
    }

    void parseAprsPayload(const QByteArray &aprsData) {
        QString payload = QString::fromUtf8(aprsData);
        QRegularExpression regex(R"(\s*(\d{2})(\d+\.\d+)([NS])[/\\](\d{3})(\d+\.\d+)([EW]))");

        // Extract Altitude from the "A=" field (Altitude in meters)
        QRegularExpression altitudeRegex(R"(\s*A=([0-9]+))");
        QRegularExpressionMatch altitudeMatch = altitudeRegex.match(payload);
        if (altitudeMatch.hasMatch()) {
            int altitude = altitudeMatch.captured(1).toInt();
            qDebug() << "Altitude (feet):" << altitude;
        } else {
            qDebug() << "No altitude information found.";
        }

        QRegularExpressionMatch match = regex.match(payload);
        if (match.hasMatch()) {
            double latitude = convertDMS(match.captured(1).toInt(), match.captured(2).toDouble(), match.captured(3) == "S");
            double longitude = convertDMS(match.captured(4).toInt(), match.captured(5).toDouble(), match.captured(6) == "W");

            qDebug() << QString::asprintf("Position: (%0.3fºN, %0.3FºE)", latitude, longitude);
        } else {
            qDebug() << "No valid APRS location found.";
        }

        // Check if there is a GPS lock and satellite count in the APRS payload
        QRegularExpression gpsLockRegex(R"(\s*A=([0-9]+)\s+L(\d+))");
        QRegularExpressionMatch gpsMatch = gpsLockRegex.match(payload);
        if (gpsMatch.hasMatch()) {
            bool gpsLock = gpsMatch.captured(1).toInt() > 1;
            int satelliteCount = gpsMatch.captured(2).toInt();

            if (gpsLock) {
                qDebug() << "GPS Lock: YES";
                qDebug() << "Satellites:" << satelliteCount;
            } else {
                qDebug() << "GPS Lock: NO";
            }
        } else {
            qDebug() << "No GPS lock or satellite info found.";
        }

    }

    double convertDMS(int degrees, double minutes, bool isNegative) {
        double decimal = degrees + (minutes / 60.0);
        return isNegative ? -decimal : decimal;
    }
};
