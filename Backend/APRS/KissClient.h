
#ifndef GROUNDSTATION_2025_KISSCLIENT_H
#define GROUNDSTATION_2025_KISSCLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QRegularExpression>

class KissClient : public QObject {
Q_OBJECT
public:
    explicit KissClient(QObject *parent = nullptr);
    void start();

private slots:
    void onConnected();

    void onDataReceived();

    void onError(QTcpSocket::SocketError error);

private:
    QTcpSocket *socket;

    void decodeAx25(const QByteArray &data);

    static QString decodeCallsign(QByteArray callsignData);

    void parseAprsPayload(const QByteArray &aprsData);

    double convertDMS(int degrees, double minutes, bool isNegative);
};

#endif //GROUNDSTATION_2025_KISSCLIENT_H

