#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QRegularExpression>

class KissClient : public QObject {
Q_OBJECT
public:
    explicit KissClient(QObject *parent = nullptr);

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
