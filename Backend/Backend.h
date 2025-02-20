//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QMutex>
#include <QDateTime>
#include <QMutex>
#include <QMap>
#include "RadioModule.h"
#include "../Utility/WebServer.h"
#include "../Utility/DataLogger.h"
#include "Utility/json_struct.h"
#include "Utility/DataSimulator/DataSimulator.h"
#include "Utility/UnitConversions.h"
#include "Backend/APRS/APRSHandler.h"

#include "generated/telemetry/RocketTelemetryPacket.pb.h"
#include "generated/telemetry/PayloadTelemetryPacket.pb.h"

class Backend : public QObject
{
    Q_OBJECT
public:

    struct ThroughputTestParams
    {
        RadioModule *receiveModule;
        uint64_t destinationAddress;
        uint8_t payloadSize;
        uint packetRate;
        uint duration;
        uint8_t transmitOptions;
    };

    struct ThroughputTestResults
    {
        uint8_t payloadSize;
        uint packetRate;
        uint duration;
        uint8_t transmitOptions;
        uint numPacketsReceived;
        float percentReceived;
        float throughput;

        JS_OBJ(
                payloadSize,
                packetRate,
                duration,
                transmitOptions,
                numPacketsReceived,
                percentReceived,
                throughput
                );
    };

    struct Telemetry
    {
        GroundStation::PacketType packetType;
        union data
        {
            HPRC::RocketTelemetryPacket *rocketData;
            HPRC::PayloadTelemetryPacket *payloadData;
        } data;
    };

    struct MaxValues
    {
        float maxAltitude;
        float minAltitude;

        float minPressure;
        float maxPressure;

        float minTemperature;
        float maxTemperature;

        float maxAcceleration;
        float minAcceleration;

        float maxVelocity;
        float minVelocity;

        float maxAngularVelocity;
        float minAngularVelocity;

        uint32_t maxRocketServoPosition;
        uint32_t minRocketServoPosition;
    };

    enum RadioModuleType
    {
        Default,
        Rocket,
        Payload
    };

    static Backend &getInstance()
    {
        static Backend instance;
        return instance;
    }
    Backend(const Backend&) = delete;
    Backend &operator=(const Backend&) = delete;
    bool connectToModule(const QString& name, RadioModuleType moduleType, int baudRate);
    void disconnectFromModule(const QString& name);
    bool moduleExistsWithName(const QString &name);

    void queryParameter(const QString &moduleName, uint16_t parameter);
    void queryParameters(const QString &moduleName, const QList<uint16_t>& parameters);

    void setParameter(const QString &moduleName, uint16_t parameter, uint8_t *value, size_t valueSize_bytes);
    void setParameter(const QString &moduleName, uint16_t parameter, uint8_t value);
    void writeParameters(const QString &moduleName);

    void linkTestComplete(LinkTestResults results, int iterationsLeft);
    void receiveTelemetry(Backend::Telemetry telemetry);

    void runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations, uint8_t repeat, bool loop=false);
    void cancelLinkTest();
    void linkTestFailed();

    void cancelThroughputTest();
    void _runThroughputTest(ThroughputTestParams params);
    void throughputTestComplete();
    void runThroughputTest(const QString& originatingPort, uint64_t destinationAddress, uint8_t payloadSize, uint packetRate, uint duration, uint8_t transmitOptions);
    void sendEnergyDetectCommand(uint16_t msPerChannel);

    void runThroughputTestsWithRange(const QString& originatingPort, uint64_t destinationAddress, QList<QList<int>> params, uint duration, uint8_t transmitOptions);

    void receiveAtCommandResponse(uint16_t command, const uint8_t *response, size_t response_length_bytes);

    void newBytesRead(QString text);
    void newBytesWritten(QString text);

    void start();
    void flushFiles();
    void getPorts();

    void setBaudRate(const QString &name, int baudRate);

    QList<RadioModule *> radioModules;
    int loopCount;

    qint64 throughputTestStartTime{};
    QTimer *throughputTestTimer;
    ThroughputTestParams throughputTestParams{};
    uint8_t throughputTestDummyLoad[255]{};

    QList<QList<int>> throughputTests;
    int throughputTestIndex;

    DataSimulator *rocketDataSimulator;
    DataSimulator *payloadDataSimulator;

    bool convertToEnglish = false;
    bool convertFromGees = false;
    int maxValueDecimalPlaces = 3;
    int telemetryDecimalPlaces = 5;
    void forceMaxValuesUpdate();


    APRSHandler aprsHandler;
    RadioModule *groundStationModem;

    DataLogger *dummyLogger;

    HPRC::RocketTelemetryPacket lastRocketPacket{};
    HPRC::PayloadTelemetryPacket lastPayloadPacket{};

    float groundLevelAltitude = 0;
    MaxValues currentStateMaxValues;
    QList<MaxValues> stateMaxValues;

    const QList<QString> RocketStateNames = {
            "Pre-Launch",
            "Launch",
            "Coast",
            "Drogue Descent",
            "Main Descent",
            "Recovery",
            "Abort"
    };

    uint32_t rocketFlightTime = 0;

public slots:
    void portOpened(const QSerialPortInfo&, bool);
    void portClosed(const QSerialPortInfo&);
    void throughputTestTimerTicked();
    void runRadioModuleCycles();
    void updateThroughputSpeeds();
    void updateRSSIInfo();

    void resetMaxRocketValues();
    void resetMaxPayloadValues();

signals:
    void foundSerialPorts(QList<QSerialPortInfo>);
    void serialPortOpened(QSerialPortInfo, bool);
    void serialPortClosed(QSerialPortInfo);
    void linkTestDataAvailable(LinkTestResults, int);
    void linkTestFailedSignal();
    void newGroundDateTime(std::tm* currentDate);
    void newGroundFlightTime(uint32_t launchTime);
    void newRocketFlightTime(uint32_t launchTime);

    void throughputTestDataAvailable(float, uint, uint);
    void telemetryAvailable(Backend::Telemetry);

    void receivedAtCommandResponse(uint16_t, const uint8_t *, size_t);
    void newBytesReadAvailable(QString);
    void newBytesWrittenAvailable(QString);

    void rocketThroughputStats(RadioThroughputStats);
    void payloadThroughputStats(RadioThroughputStats);
    void combinedThroughputStats(RadioThroughputStats);
    void rocketCountStats(RadioCountStats);
    void payloadCountStats(RadioCountStats);
    void combinedCountStats(RadioCountStats);
    void droppedPackets(uint32_t);

    void newMaxRocketValues(const MaxValues &);
    void newMaxPayloadValues(const MaxValues &);

    void rocketStateChanged(const MaxValues &, int lastState, int newState);

private:
    explicit Backend(QObject *parent = nullptr);

    using ConversionFunction = float (*)(float);
    static void doConversions(google::protobuf::Message *message, const QMap<std::string, ConversionFunction> &conversionMap);

    static QMap<std::string, ConversionFunction> metricToEnglish;
    static QMap<std::string, ConversionFunction> geeConversions_English;
    static QMap<std::string, ConversionFunction> geeConversions_Metric;

    RadioModule *getModuleWithName(const QString& name);
    void updateMaxRocketValues(Backend::MaxValues &maxValues, const HPRC::RocketTelemetryPacket& rocketData);
    void updateMaxPayloadValues(Backend::MaxValues &maxValues, HPRC::PayloadTelemetryPacket payloadData);
    void updateTimes(const HPRC::RocketTelemetryPacket &rocketData);

    WebServer *webServer{};
    DataLogger *dataLogger{};

    QTimer *timer{};

    bool throughputTestShouldStop = false;

    // ground date/time
    QTimer *rtcTimer{};
    std::time_t currentGroundEpoch;

    QElapsedTimer groundFlightTime{};
    uint32_t rocketTimestampStart;
    QTimer *throughputTimer{};
    QTimer *rssiTimer{};

    RadioCountStats lastRocketCount;
    RadioCountStats lastPayloadCount;

    QMutex mutex;

    MaxValues maxRocketValues{};
    MaxValues maxPayloadValues{};
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
