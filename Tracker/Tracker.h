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

    struct GPSData {
        double lat;
        double lon;
    };

    struct Pose
    {
        float azimuth_degrees;
        float elevation_degrees;
    };

    Pose lowpassBounds = {5, 5};

    Pose commandedPose{};
    Pose desiredPose{};
    Pose actualPose{};
    Pose pointerPose{};
    Pose poseDifference();

    float trackerLat;
    float trackerLong;
    float rocketLat;
    float rocketLong;
    float trackerHeading;
    float rocketAltitude;

    bool invertAz = false;

    float elevationLead = 0;
    void setElevationLead(float lead);

    bool enabled = true;
    bool manualControlEnabled = true;

    float zeroAltitude = 0;

    void connectToPort(const QSerialPortInfo& port, int baudRate);
    void disconnect();
    void send(const char *buffer, size_t length_bytes);
    void read();

    void logData();

    QSerialPortInfo portInfo{};

    void sendMessage_setPose(Pose newDesiredPose);
    void sendMessage_getPose();
    void sendMessage_home();
    void sendMessage_getGps();
    void sendMessage_getImu();
    void sendEstop_brake();
    void sendEstop_coast();

    qint64 sentTime;

    double gpsToDistance(GPSData gps1, GPSData gps2);
    double gpsToHeading(GPSData gps1, GPSData gps2);
    double calculatePitch(double altitude, double groundDist);

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

public slots:
    void newPointerPose(float azimuth, float elevation);

private:
    SerialPort *serialPort;
    char readBuffer[READ_BUFFER_LENGTH];
    uint readBufferIndex = 0;

    void sendString(QString str);

    DataLogger *dataLogger;

    QTimer *readTimer;
    QTimer *poseTimer;

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
