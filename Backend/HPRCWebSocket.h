//
// Created by William Scheirey on 2/1/25.
//

#ifndef GROUNDSTATION_2025_HPRCWEBSOCKET_H
#define GROUNDSTATION_2025_HPRCWEBSOCKET_H

#include <QtWebSockets>
#include <qabstractsocket.h>

class HPRCWebSocket : public QObject
{

Q_OBJECT

public:
    HPRCWebSocket();

    void connectToServer();

    void close();

    QWebSocket *m_socket;

private:
    QThread thread;

    void _ping();

    bool retryConnection;

public slots:

    void onConnected();

    void onDisconnected();

    void sslErrors(QList<QSslError> errs);

    void stateChanged(QAbstractSocket::SocketState state);

    void handshakeInterruptedOnError(const QSslError &error);

    void peerVerifyError(const QSslError &error);

    void aboutToClose();

    void bytesWritten(qint64 bytes);

    void readChannelFinished();

    void messageReceived(QString message);

    void _connectToServer();


signals:

    void open(QUrl);

    void open(QNetworkRequest);

    void closeTheSocket(QWebSocketProtocol::CloseCode, QString);

    void ping(QByteArray);

    void socketStateChanged(QAbstractSocket::SocketState state);
};


#endif //GROUNDSTATION_2025_HPRCWEBSOCKET_H
