//
// Created by William Scheirey on 5/5/25.
//

#ifndef GROUNDSTATION_2025_TRACKER_H
#define GROUNDSTATION_2025_TRACKER_H
#include <QObject>
#include "Utility/SerialPort.h"

#define READ_BUFFER_LENGTH 64 // Arbitrary length, but is longer than any currently-defined messages

class Tracker: public QObject
{
    Q_OBJECT
public:
    Tracker(QObject *parent = nullptr);
    void connectToPort(const QSerialPortInfo& port, int baudRate, DataLogger *dataLogger);
    void send(const char *buffer, size_t length_bytes);
    void read();

private:
    SerialPort *serialPort;
    char readBuffer[READ_BUFFER_LENGTH];
    uint readBufferIndex = 0;

private:
    static int utf8DigitsToInt(const char *encodedDigits, size_t length_bytes);
    static int bytesUntilSemicolon(const char *buffer);
    void handleMessage(const char *buffer);

    void handleData(const char *buffer);
    void handleResponse(const char *buffer);

    void handleData_pose(const char *buffer);
    void handleData_gps(const char *buffer);
    void handleData_imu(const char *buffer);

    void handleResponse_pose();
    void handleEstopResponse_brake();
    void handleEstopResponse_coast()
};


#endif //GROUNDSTATION_2025_TRACKER_H
