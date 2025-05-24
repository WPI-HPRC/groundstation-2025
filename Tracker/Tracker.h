//
// Created by William Scheirey on 5/5/25.
//

#ifndef GROUNDSTATION_2025_TRACKER_H
#define GROUNDSTATION_2025_TRACKER_H
#include <QObject>
#include <QTimer>
#include "Utility/SerialPort.h"

#define READ_BUFFER_LENGTH 64 // Arbitrary length, but is longer than any currently-defined messages

class Tracker: public QObject
{
    Q_OBJECT
public:
    static Tracker &getInstance()
    {
        static Tracker instance;
        return instance;
    }
    Tracker(const Tracker&) = delete;
    Tracker &operator=(const Tracker&) = delete;

    struct Pose
    {
        float azimuth_degrees;
        float elevation_degrees;
    };

    Pose desiredPose{};
    Pose actualPose{};
    Pose poseDifference();

    bool enabled = true;
    bool manualControlEnabled = true;

    void connectToPort(const QSerialPortInfo& port, int baudRate);
    void disconnect();
    void send(const char *buffer, size_t length_bytes);
    void read();

    QSerialPortInfo portInfo{};

    void sendMessage_setPose(Pose newDesiredPose);
    void sendMessage_getPose();
    void sendMessage_home();
    void sendMessage_getGps();
    void sendMessage_getImu();
    void sendEstop_brake();
    void sendEstop_coast();

signals:
    void newPoseData(Pose);
    void newGpsData(float, float);
    void newImuData(float, float, float, float);
    void newPoseResponse();
    void newEstopResponse_brake();
    void newEstopResponse_coast();
    void newDesiredPose(Pose);

    void portClosed(QSerialPortInfo);
    void portOpened(QSerialPortInfo);

    void dataRead(QString);

private:
    SerialPort *serialPort;
    char readBuffer[READ_BUFFER_LENGTH];
    uint readBufferIndex = 0;

    void sendString(QString str);

    DataLogger *dataLogger;

    QTimer *readTimer;

private:
    explicit Tracker(QObject *parent = nullptr);

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
    void handleEstopResponse_coast();
};


#endif //GROUNDSTATION_2025_TRACKER_H
