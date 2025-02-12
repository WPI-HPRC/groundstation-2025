
#ifndef GROUNDSTATION_2025_KISSCLIENT_H
#define GROUNDSTATION_2025_KISSCLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QRegularExpression>

class KissClient : public QObject {
Q_OBJECT
public:

    struct AprsData
    {
        bool gpsLock;
        int satellites;
        int altitude;
        float longitude;
        float latitude;
    };

    struct AprsOutput
    {
        AprsData data;
        QString callsign;
        int ID;
    };

    explicit KissClient(QObject *parent = nullptr);
    void start();

    bool isConnected;
    bool isConnecting;

private slots:
    void onConnected();
    void onDisconnected();

    void onDataReceived();

    void onError(QTcpSocket::SocketError error);

signals:
    void output(AprsOutput);

private:
    QTcpSocket *socket;

    AprsOutput currentOutput;

    void decodeAx25(const QByteArray &data);

    QString decodeCallsign(QByteArray callsignData);

    void parseAprsPayload(const QByteArray &aprsData);

    float convertDMS(int degrees, double minutes, bool isNegative);
};

#endif //GROUNDSTATION_2025_KISSCLIENT_H

