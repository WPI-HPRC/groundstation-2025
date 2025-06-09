//
// Created by William Scheirey on 6/8/25.
//

#ifndef GROUNDSTATION_2025_POINTER_H
#define GROUNDSTATION_2025_POINTER_H
#include <QObject>
#include <QTimer>
#include "Tracker/Tracker.h"
#include "Utility/SerialPort.h"

#define READ_BUFFER_LENGTH 64

class Pointer: public QObject
{
Q_OBJECT
public:
    static Pointer &getInstance()
    {
        static Pointer instance;
        return instance;
    }
    Pointer(const Pointer&) = delete;
    Pointer &operator=(const Pointer&) = delete;

    struct Pose
    {
        float azimuth_degrees;
        float elevation_degrees;
    };

    void connectToPort(const QSerialPortInfo& port, int baudRate);
    void disconnect();
    void read();

    QSerialPortInfo portInfo{};

signals:
    void newPoseData(float azimuth, float elevation);

    void portClosed(QSerialPortInfo);
    void portOpened(QSerialPortInfo);

    void dataRead(QString);

private:
    SerialPort *serialPort;
    char readBuffer[READ_BUFFER_LENGTH];
    uint readBufferIndex = 0;

    DataLogger *dataLogger;

    QTimer *readTimer;

private:
    explicit Pointer(QObject *parent = nullptr);

    static int utf8DigitsToInt(const char *encodedDigits, size_t length_bytes);
    static int bytesUntilCharacter(const char *buffer, char character);
    void handleMessage(const char *buffer);
};


#endif //GROUNDSTATION_2025_POINTER_H
