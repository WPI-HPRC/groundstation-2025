//
// Created by William Scheirey on 8/20/24.
//

#include "Backend.h"
#include <QSerialPort>
#include <QJsonDocument>
#include <string>
#include <utility>
#include <chrono>
#include "Constants.h"

QMap<std::string, Backend::ConversionFunction> Backend::metricToEnglish = {
        {"altitude", &Utility::UnitConversion::meters2feet},
        {"gpsaltmsl", &Utility::UnitConversion::meters2feet},
        {"gpsaltagl", &Utility::UnitConversion::meters2feet},

        {"velx", &Utility::UnitConversion::meters2feet},
        {"vely", &Utility::UnitConversion::meters2feet},
        {"velz", &Utility::UnitConversion::meters2feet},

        {"posx", &Utility::UnitConversion::meters2feet},
        {"posy", &Utility::UnitConversion::meters2feet},
        {"posz", &Utility::UnitConversion::meters2feet},

        {"pressure", &Utility::UnitConversion::mbar2psi},

        {"temperature", &Utility::UnitConversion::cel2far}
};

QMap<std::string, Backend::ConversionFunction> Backend::geeConversions_English = {
        {"accelx", &Utility::UnitConversion::gs2feet},
        {"accely", &Utility::UnitConversion::gs2feet},
        {"accelz", &Utility::UnitConversion::gs2feet},
};

QMap<std::string, Backend::ConversionFunction> Backend::geeConversions_Metric = {
        {"accelx", &Utility::UnitConversion::gs2meters},
        {"accely", &Utility::UnitConversion::gs2meters},
        {"accelz", &Utility::UnitConversion::gs2meters},
};
QSerialPortInfo getTargetPort(const QString& portName)
{
    QList serialPorts = QSerialPortInfo::availablePorts();

    QSerialPortInfo targetPort;

#ifdef DEBUG_SERIAL
    std::cout << "Available baud rates: \n";
    for (auto &baudRate: QSerialPortInfo::standardBaudRates())
    {
        std::cout << "\t" << baudRate << "\n";
    }

    std::cout << "\nFound serial ports: \n";
#endif

    for (auto &port: serialPorts)
    {
#ifdef DEBUG_SERIAL
        std::cout << "\n" << port.portName().toStdString() << "\n";
        std::cout << "\tManufacturer: " << port.manufacturer().toStdString() << "\n";
        std::cout << "\tSystem location: " << port.systemLocation().toStdString() << "\n";
        std::cout << "\tSerial number: " << port.serialNumber().toStdString() << "\n";
        std::cout.flush();
#endif

        if (port.portName().contains(portName))
        {
            targetPort = port;
        }
    }

    return targetPort;
}

void Backend::doConversions(google::protobuf::Message *message, const QMap<std::string, ConversionFunction> &conversionMap)
{
    const google::protobuf::Reflection *reflection = message->GetReflection();
    const google::protobuf::Descriptor *descriptor = message->GetDescriptor();

    for (const std::string& fieldName : conversionMap.keys())
    {
        const google::protobuf::FieldDescriptor *field = descriptor->FindFieldByLowercaseName(fieldName);

        if(field)
        {
            reflection->SetFloat(message,
                                 field,
                                 conversionMap.value(fieldName)(reflection->GetFloat(*message, field))
                                 );
        }
        else
        {
            qDebug() << "Couldn't find field " << fieldName.c_str();
        }
    }
}

void Backend::portOpened(const QSerialPortInfo& portInfo, bool success)
{
    emit(serialPortOpened(portInfo, success));
}

void Backend::portClosed(const QSerialPortInfo& portInfo)
{
    emit(serialPortClosed(portInfo));
}

RadioModule *Backend::getModuleWithName(const QString& name)
{
    RadioModule *module;
    for(int i = 0; i < radioModules.count(); i++)
    {
        if(radioModules.at(i)->serialPort->name().contains(name))
        {
            return radioModules.at(i);
        }
    }
    return nullptr;
}

bool Backend::moduleExistsWithName(const QString &name)
{
    return getModuleWithName(name) != nullptr;
}

void Backend::getPorts()
{
    QList serialPorts = QSerialPortInfo::availablePorts();
    emit foundSerialPorts(serialPorts);
}

void Backend::flushFiles()
{
    for (RadioModule *radioModule: this->radioModules)
    {
        radioModule->dataLogger->flushDataFiles();
        radioModule->dataLogger->flushByteFile();
        radioModule->dataLogger->flushTextFile();
    }
}

void Backend::linkTestFailed()
{
    emit linkTestFailedSignal();
}

void Backend::runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations, uint8_t repeat, bool loop)
{
    if(getTargetPort(GROUND_STATION_MODULE).isNull())
        return;

    RadioModule *module = getModuleWithName(GROUND_STATION_MODULE);

    std::cout << "Running link test" << std::endl;
    if(repeat != 0)
    {
        std::cout << "Repeat != 0" << std::endl;
        module->linkTestsLeft = loop ? -1 : repeat;
    }
    else
    {
        std::cout << "Repeat == 0" << std::endl;
        module->linkTestsLeft--;
    }

    std::cout << "LinkTestsLeft = " << module->linkTestsLeft << std::endl;

    module->sendLinkTestRequest(destinationAddress, payloadSize, iterations);
}

void Backend::cancelLinkTest()
{
    std::cout << "Cancelling link test" << std::endl;
    getModuleWithName(GROUND_STATION_MODULE)->linkTestsLeft = 0;
}

void Backend::_runThroughputTest(Backend::ThroughputTestParams params)
{
    throughputTestTimer->setInterval(1000 / (int)params.packetRate);

    params.receiveModule->receivingThroughputTest = true;
    params.receiveModule->throughputTestPacketsReceived = 0;
    params.receiveModule->logTransmitStatus = false;

    throughputTestStartTime = QDateTime::currentMSecsSinceEpoch();
    throughputTestTimer->start();
}

void Backend::cancelThroughputTest()
{
    throughputTestShouldStop = true;
    throughputTestIndex = -1;
}

void Backend::runThroughputTest(const QString& originatingPort, uint64_t destinationAddress, uint8_t payloadSize,
                                uint packetRate, uint duration, uint8_t transmitOptions)
{
    RadioModule *receiveModule = getModuleWithName(originatingPort);

    if(!receiveModule)
        return;

    throughputTestIndex = -1;

    throughputTestParams = {
      .receiveModule = receiveModule,
      .destinationAddress = destinationAddress,
      .payloadSize = payloadSize,
      .packetRate = packetRate,
      .duration = duration,
      .transmitOptions = transmitOptions
    };

    _runThroughputTest(throughputTestParams);
};

void Backend::runThroughputTestsWithRange(const QString &originatingPort, uint64_t destinationAddress,
                                          QList<QList<int>> params, uint duration, uint8_t transmitOptions)
{
    RadioModule *receiveModule = getModuleWithName(originatingPort);

    if(!receiveModule)
        return;

    throughputTestIndex = 0;

    throughputTests = std::move(params);

    throughputTestParams = {
            .receiveModule = receiveModule,
            .destinationAddress = destinationAddress,
            .payloadSize = (uint8_t)throughputTests.at(0).at(0),
            .packetRate = (uint8_t)throughputTests.at(0).at(1),
            .duration = duration,
            .transmitOptions = transmitOptions
    };

    _runThroughputTest(throughputTestParams);
}

void Backend::throughputTestTimerTicked()
{
    qint64 currentMs = QDateTime::currentMSecsSinceEpoch();

    if(currentMs - throughputTestStartTime >= throughputTestParams.duration*1000 || throughputTestShouldStop)
    {
        throughputTestShouldStop = false;
        throughputTestTimer->stop();
        this->throughputTestComplete();
        return;
    }

//    std::cout << "Timer ticked. Current ms: " << std::dec << currentMs << std::endl;
    mutex.lock();
    throughputTestParams.receiveModule->sendTransmitRequestCommand(throughputTestParams.destinationAddress, throughputTestParams.transmitOptions,
                                                                   0x00, throughputTestDummyLoad, throughputTestParams.payloadSize);
    mutex.unlock();
}

void Backend::throughputTestComplete()
{
    uint numPacketsReceived = throughputTestParams.receiveModule->throughputTestPacketsReceived;
    uint theoreticalPacketsReceived = throughputTestParams.packetRate * throughputTestParams.duration;

    float percentReceived = (float)numPacketsReceived / (float)theoreticalPacketsReceived * 100;

    float throughput = (float)numPacketsReceived * throughputTestParams.payloadSize / throughputTestParams.duration / 1000 * 8;

    ThroughputTestResults results = {
            .payloadSize = throughputTestParams.payloadSize,
            .packetRate = throughputTestParams.packetRate,
            .duration = throughputTestParams.duration,
            .transmitOptions = throughputTestParams.transmitOptions,
            .numPacketsReceived = numPacketsReceived,
            .percentReceived = percentReceived,
            .throughput = throughput
    };

    std::string str = JS::serializeStruct(results);

    QJsonObject obj = QJsonDocument::fromJson(str.c_str()).object();

    throughputTestParams.receiveModule->dataLogger->logThroughputTest(obj);

    emit throughputTestDataAvailable(percentReceived, numPacketsReceived, throughput);

    if(throughputTestIndex >= 0 && throughputTestIndex < throughputTests.count() - 1)
    {
        throughputTestIndex++;
        throughputTestParams.payloadSize = throughputTests.at(throughputTestIndex).at(0);
        throughputTestParams.packetRate = throughputTests.at(throughputTestIndex).at(1);
        _runThroughputTest(throughputTestParams);
    }
}

void Backend::sendEnergyDetectCommand(uint16_t msPerChannel)
{
    if(getTargetPort(GROUND_STATION_MODULE).isNull())
        return;

    getModuleWithName(GROUND_STATION_MODULE)->sendEnergyDetectCommand(msPerChannel);
}

void Backend::receiveAtCommandResponse(uint16_t command, const uint8_t *response, size_t response_length_bytes)
{
    emit receivedAtCommandResponse(command, response, response_length_bytes);
}

void Backend::linkTestComplete(LinkTestResults results, int iterationsLeft)
{
    emit linkTestDataAvailable(results, iterationsLeft);
}

void Backend::updateMaxRocketValues(Backend::MaxValues &maxValues, const HPRC::RocketTelemetryPacket& rocketData)
{
    bool newMaxValue = false;
    if(rocketData.altitude() > maxValues.maxAltitude || maxValues.maxAltitude == 0)
    {
        maxValues.maxAltitude = rocketData.altitude();
        newMaxValue = true;
    }
    if(rocketData.altitude() < maxValues.minAltitude || maxValues.minAltitude == 0)
    {
        maxValues.minAltitude = rocketData.altitude();
    }

    if(rocketData.pressure() > maxValues.maxPressure || maxValues.maxPressure == 0)
    {
        maxValues.maxPressure = rocketData.pressure();
        newMaxValue = true;
    }
    if(rocketData.pressure() < maxValues.minPressure || maxValues.minPressure == 0)
    {
        maxValues.minPressure = rocketData.pressure();
        newMaxValue = true;
    }

    if(rocketData.temperature() > maxValues.maxTemperature || maxValues.maxTemperature == 0)
    {
        maxValues.maxTemperature = rocketData.temperature();
        newMaxValue = true;
    }
    if(rocketData.temperature() < maxValues.minTemperature || maxValues.minTemperature == 0)
    {
        maxValues.minTemperature = rocketData.temperature();
        newMaxValue = true;
    }

    if(rocketData.servoposition() > maxValues.maxRocketServoPosition)
    {
        maxValues.maxRocketServoPosition = rocketData.servoposition();
        newMaxValue = true;
    }

    auto acceleration = (float)sqrt(pow(rocketData.accelx(), 2) + pow(rocketData.accely(), 2) + pow(rocketData.accelz(), 2));
    if(acceleration > maxValues.maxAcceleration || maxValues.maxAcceleration == 0)
    {
        maxValues.maxAcceleration = acceleration;
        newMaxValue = true;
    }
    if(acceleration < maxValues.minAcceleration || maxValues.minAcceleration == 0)
    {
        maxValues.minAcceleration = acceleration;
    }

    auto velocity = (float)sqrt(pow(rocketData.velx(), 2) + pow(rocketData.vely(), 2) + pow(rocketData.velz(), 2));
    if(velocity > maxValues.maxVelocity || maxValues.maxVelocity == 0)
    {
        maxValues.maxVelocity = velocity;
        newMaxValue = true;
    }
    if(velocity < maxValues.minVelocity || maxValues.minVelocity == 0)
    {
        maxValues.minVelocity = velocity;
    }

    auto angularVelocity = (float)sqrt(pow(rocketData.gyrox(), 2) + pow(rocketData.gyroy(), 2) + pow(rocketData.gyroz(), 2));
    if(angularVelocity > maxValues.maxAngularVelocity)
    {
        maxValues.maxAngularVelocity = angularVelocity;
        newMaxValue = true;
    }
    if(newMaxValue && (&maxValues == &maxRocketValues))
    {
        emit newMaxRocketValues(maxValues);
    }
}

void Backend::updateMaxPayloadValues(Backend::MaxValues &maxValues, HPRC::PayloadTelemetryPacket payloadData)
{
    bool newMaxValue = false;
    if(payloadData.altitude() > maxValues.maxAltitude)
    {
        maxValues.maxAltitude = payloadData.altitude();
        newMaxValue = true;
    }

    if(payloadData.pressure() > maxValues.maxPressure || maxValues.maxPressure == 0)
    {
        maxValues.maxPressure = payloadData.pressure();
        newMaxValue = true;
    }
    else if(payloadData.pressure() < maxValues.minPressure || maxValues.minPressure == 0)
    {
        maxValues.minPressure = payloadData.pressure();
        newMaxValue = true;
    }

    if(payloadData.temperature() > maxValues.maxTemperature || maxValues.maxTemperature == 0)
    {
        maxValues.maxTemperature = payloadData.temperature();
        newMaxValue = true;
    }
    else if(payloadData.temperature() < maxValues.minTemperature || maxValues.minTemperature == 0)
    {
        maxValues.minTemperature = payloadData.temperature();
        newMaxValue = true;
    }

    auto acceleration = (float)sqrt(pow(payloadData.accelx(), 2) + pow(payloadData.accely(), 2) + pow(payloadData.accelz(), 2));
    if(acceleration > maxValues.maxAcceleration)
    {
        maxValues.maxAcceleration = acceleration;
        newMaxValue = true;
    }

    auto velocity = (float)sqrt(pow(payloadData.velx(), 2) + pow(payloadData.vely(), 2) + pow(payloadData.velz(), 2));
    if(velocity > maxValues.maxVelocity)
    {
        maxValues.maxVelocity = velocity;
        newMaxValue = true;
    }

    auto angularVelocity = (float)sqrt(pow(payloadData.gyrox(), 2) + pow(payloadData.gyroy(), 2) + pow(payloadData.gyroz(), 2));
    if(angularVelocity > maxValues.maxAngularVelocity)
    {
        maxValues.maxAngularVelocity = angularVelocity;
        newMaxValue = true;
    }
    if(newMaxValue && (&maxPayloadValues == &maxValues))
    {
        emit newMaxPayloadValues(maxValues);
    }
}

void Backend::resetMaxRocketValues()
{
    maxRocketValues = {};
    emit newMaxRocketValues(maxRocketValues);
}

void Backend::resetMaxPayloadValues()
{
    maxPayloadValues = {};
    emit newMaxPayloadValues(maxPayloadValues);
}

void Backend::forceMaxValuesUpdate()
{
    emit newMaxRocketValues(maxRocketValues);
    emit newMaxPayloadValues(maxPayloadValues);
}

void Backend::updateTimes(const HPRC::RocketTelemetryPacket &rocketData)
{
    if (!groundFlightTime.isValid() // if we haven't started the launch-elapsed timer
        && (rocketData.state() > 0)) // and we're in a non-prelaunch state
    {
        std::cout << "Launched!" << std::endl;
        groundFlightTime.start(); // start a timer within the application
        rocketTimestampStart = rocketData.timestamp(); // get our start value for rocket time
    }
    if (groundFlightTime.isValid())
    {
        emit newGroundFlightTime(groundFlightTime.elapsed());
        rocketFlightTime = rocketData.timestamp() - rocketTimestampStart;
        emit newRocketFlightTime(rocketFlightTime);
    }
    else
    {
        emit newGroundFlightTime(0);
        emit newRocketFlightTime(0);
    }
}

void Backend::handleRocketTelemetry(Backend::Packet telemetry)
{
    HPRC::RocketTelemetryPacket *packet = telemetry.data.rocketData;
    updateMaxRocketValues(maxRocketValues, *packet);
    updateTimes(*packet);

    if(lastRocketPacket.state() == 0 && packet->state() == 1)
    {
        groundLevelAltitude = lastRocketPacket.altitude();
    }
    if(lastRocketPacket.state() != packet->state())
    {
        // Reset things when we reach prelaunch state

        if(packet->state() == 0)
        {
            stateMaxValues.clear();
        }

        currentStateMaxValues.minAltitude = lastRocketPacket.altitude();
        emit rocketStateChanged(currentStateMaxValues, lastRocketPacket.state(), packet->state());

        if(lastRocketPacket.state() != 6)
        {
            stateMaxValues.insert(lastRocketPacket.state(), currentStateMaxValues);
            currentStateMaxValues = {};
        }
    }

    updateMaxRocketValues(currentStateMaxValues, *packet);

    lastRocketPacket = *packet;
    if(convertToEnglish)
    {
        doConversions(packet, metricToEnglish);
        if(convertFromGees)
        {
            doConversions(packet, geeConversions_English);
        }
    }
    else if(convertFromGees)
    {
        doConversions(packet, geeConversions_Metric);
    }
}

void Backend::handlePayloadTelemetry(Backend::Packet telemetry)
{
    HPRC::PayloadTelemetryPacket *packet = telemetry.data.payloadData;
    updateMaxPayloadValues(maxPayloadValues, *packet);
    lastPayloadPacket = *packet;
    if(convertToEnglish)
    {
        doConversions(packet, metricToEnglish);
        if(convertFromGees)
        {
            doConversions(packet, geeConversions_English);
        }
    }
    else if(convertFromGees)
    {
        doConversions(packet, geeConversions_Metric);
    }
}

void Backend::handlePacketizedPacket(Backend::GenericPacket packet)
{
    // This denotes the packetized data is done being sent
    if(packet.length_bytes == 1)
    {
        switch(packet.data[0])
        {
            case GroundStation::SDDirectory:
                handleSDDirectoryContents(sDDirectoryContents);
                break;
            case GroundStation::SDFileContents:
                break;
            case GroundStation::Image:
            default:
                return;
        }
        return;
    }

    // Is there a better way to do this? Yuck
    Backend::PacketizedData *packetizedData;
    switch(packet.data[0])
    {
        case GroundStation::SDDirectory:
            packetizedData = &sDDirectoryContents;
            break;
        case GroundStation::SDFileContents:
            packetizedData = &sDFileContents;
            break;
        case GroundStation::Image:
            packetizedData = &image;
        default:
            return;
    }

    packetizedData->length_bytes += packet.length_bytes;
    packetizedData->num_packets_received++;
    // Skip the first byte, which is the frame type
    memcpy(packetizedData->currentBytePointer, &packet.data[1], packet.length_bytes);
    packetizedData->currentBytePointer += packet.length_bytes;
}

void Backend::handleSDDirectoryContents(Backend::PacketizedData data)
{
    QList<QByteArray> fileNames = QByteArray::fromRawData((const char *)data.bytes, data.length_bytes).split('\0');
    for(int i = 0; i < fileNames.length(); i++)
    {
        qDebug() << fileNames.at(i).toStdString();
    }
}

void Backend::receivePacket(Backend::Packet telemetry)
{
    switch (telemetry.packetType)
    {
        case GroundStation::Rocket:
            handleRocketTelemetry(telemetry);
            break;
        case GroundStation::Payload:
            handlePayloadTelemetry(telemetry);
            break;
        case GroundStation::SDDirectory: case GroundStation::SDFileContents: case GroundStation::Image:
            handlePacketizedPacket(telemetry.data.genericPacket);
            break;
        default:
            break;
    }

    emit telemetryAvailable(telemetry);
}

void Backend::setBaudRate(const QString &name, int baudRate)
{
    RadioModule *module = getModuleWithName(name);

    if(!module)
    {
        return;
    }

    module->setBaudRate(baudRate);
}

void Backend::disconnectFromModule(const QString &name)
{
    RadioModule *module = getModuleWithName(name);

    if(!module)
        return;

    module->disconnectPort();
}

void Backend::queryParameter(const QString &moduleName, uint16_t parameter)
{
    RadioModule *module = getModuleWithName(moduleName);

    if(!module)
        return;

    module->queryParameter(parameter);
}

void Backend::queryParameters(const QString &moduleName, const QList<uint16_t>& parameters)
{
    for(int i = 0; i < parameters.count(); i++)
    {
        queryParameter(moduleName, parameters.at(i));
    }
}

void Backend::setParameter(const QString &moduleName, uint16_t parameter, uint8_t *value, size_t valueSize_bytes)
{
    RadioModule *module = getModuleWithName(moduleName);

    if(!module)
        return;

    module->setParameter(parameter, value, valueSize_bytes);
}

void Backend::setParameter(const QString &moduleName, uint16_t parameter, uint8_t value)
{
    setParameter(moduleName, parameter, &value, 1);
}

void Backend::writeParameters(const QString &moduleName)
{
    RadioModule *module = getModuleWithName(moduleName);

    if(!module)
        return;
    module->writeChanges();
}

bool Backend::connectToModule(const QString& name, RadioModuleType moduleType, int baudRate)
{
    RadioModule *existingModule = getModuleWithName(name);
    if(existingModule)
    {
        if(!existingModule->serialPort->isOpen())
        {
            existingModule->connectPort();
        }
        return true;
    }

    QSerialPortInfo targetPort = getTargetPort(name);

    if(targetPort.isNull())
    {
        /*
        qDebug() << "Could not find module " << name;
        QTimer::singleShot(1000, [this, name, moduleType]()
        {
            this->connectToModule(name, moduleType);
        });
        */
        return false;
    }

    RadioModule *module;
    switch(moduleType)
    {
        case Rocket:
            module = new RocketTestModule(baudRate, new DataLogger(), targetPort);
            break;
        case Payload:
            module = new PayloadTestModule(baudRate, new DataLogger(), targetPort);
            break;
        default:
            module = new ServingRadioModule(baudRate, new DataLogger(), targetPort, webServer);
    }
    radioModules.append(module);
    return true;
}

void Backend::runRadioModuleCycles()
{
    mutex.lock();
    for (auto radioModule: this->radioModules)
    {
        radioModule->doCycle();
    }
    mutex.unlock();
}

void Backend::newBytesRead(QString text)
{
    emit newBytesReadAvailable(std::move(text));
}

void Backend::newBytesWritten(QString text)
{
    emit newBytesWrittenAvailable(std::move(text));
}

void Backend::updateThroughputSpeeds()
{
    RadioModule *module = getModuleWithName(GROUND_STATION_MODULE);

    if(!module){return;} // safety measure to prevent crashing the program if the radio isn't actually connected

    int multiple = (1000/throughputTimer->interval());

    RadioThroughputStats rocketStats{};
    RadioThroughputStats payloadStats{};
    RadioThroughputStats combinedStats{};
    RadioCountStats combinedCount{};

    combinedCount.bytesReceivedCount = module->rocketRadioStats.bytesReceivedCount + module->payloadRadioStats.bytesReceivedCount;
    combinedCount.packetsReceivedCount = module->rocketRadioStats.packetsReceivedCount + module->payloadRadioStats.packetsReceivedCount;

    rocketStats.bytesPerSecond = (module->rocketRadioStats.bytesReceivedCount - lastRocketCount.bytesReceivedCount) * multiple;
    rocketStats.packetsPerSecond = (module->rocketRadioStats.packetsReceivedCount - lastRocketCount.packetsReceivedCount) * multiple;

    payloadStats.bytesPerSecond = (module->payloadRadioStats.bytesReceivedCount - lastPayloadCount.bytesReceivedCount) * multiple;
    payloadStats.packetsPerSecond = (module->payloadRadioStats.packetsReceivedCount - lastPayloadCount.packetsReceivedCount) * multiple;

    combinedStats.bytesPerSecond = (rocketStats.bytesPerSecond + payloadStats.bytesPerSecond) * multiple;
    combinedStats.packetsPerSecond = (rocketStats.packetsPerSecond + payloadStats.packetsPerSecond) * multiple;

    emit rocketThroughputStats(rocketStats);
    emit payloadThroughputStats(payloadStats);
    emit combinedThroughputStats(combinedStats);

    emit rocketCountStats(module->rocketRadioStats);
    emit payloadCountStats(module->payloadRadioStats);
    emit combinedCountStats(combinedCount);

    emit droppedPackets(module->droppedPacketsCount);

    // update our "last" counters to get the difference next loop cycle
    lastRocketCount = module->rocketRadioStats;
    lastPayloadCount = module->payloadRadioStats;

    // get the latest error count from the radio module
    module->sendNextFrameImmediately = true;
    Backend::queryParameter(GROUND_STATION_MODULE, XBee::AtCommand::ErrorCount);
    // reset the radio's internal error count
    module->sendNextFrameImmediately = true;
    Backend::setParameter(GROUND_STATION_MODULE, XBee::AtCommand::ErrorCount, 0);
}

void Backend::updateRSSIInfo()
{
    RadioModule *module = getModuleWithName(GROUND_STATION_MODULE);

    if(!module){return;} // safety measure to prevent crashing the program if the radio isn't actually connected

    // ask for the latest RSSI from the radio module
    module->sendNextFrameImmediately = true;
    Backend::queryParameter(GROUND_STATION_MODULE, XBee::AtCommand::LastPacketRSSI);
}

void Backend::start()
{
    getPorts();

//    webServer = new WebServer(8001);
  
    QString simulationFile = ":/SimulationData/SamplePayloadData.csv";

    payloadDataSimulator = new DataSimulator(
            simulationFile,
            webServer,
            HPRC::PayloadTelemetryPacket::descriptor(),
            GroundStation::PacketType::Payload
            );

    simulationFile = ":/SimulationData/SampleRocketData.csv";
    rocketDataSimulator = new DataSimulator(
            simulationFile,
            webServer,
            HPRC::RocketTelemetryPacket::descriptor(),
            GroundStation::PacketType::Rocket
    );

    QSerialPortInfo modem = getTargetPort(GROUND_STATION_MODULE);
    if(!modem.isNull())
    {
        connectToModule(GROUND_STATION_MODULE, Default, 230400);
        groundStationModem = getModuleWithName(GROUND_STATION_MODULE);
        // reset the radio's internal error count
        groundStationModem->sendNextFrameImmediately = true;
        Backend::setParameter(GROUND_STATION_MODULE, XBee::AtCommand::ErrorCount, 0);
    }

    timer = new QTimer();
    timer->setInterval(5);

    loopCount = 0;

    connect(timer, &QTimer::timeout, this, &Backend::runRadioModuleCycles);
    timer->start();

    rtcTimer = new QTimer();
    rtcTimer->setInterval(100);

    connect(rtcTimer, &QTimer::timeout, [this]()
            {
                currentGroundEpoch = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                std::tm* currentLocalDateTime = std::localtime(&currentGroundEpoch);

                emit newGroundDateTime(currentLocalDateTime);
            }
    );
    rtcTimer->start();

    // timer to run math for calculating radio throughput
    // be careful running this too fast, or else it will bog down the radio with too many ATCommandRequests
    throughputTimer = new QTimer();
    throughputTimer->setInterval(500);

    connect(throughputTimer, &QTimer::timeout, this, &Backend::updateThroughputSpeeds);
//    throughputTimer->start();

    // use a separate timer for the RSSI data so it gets updated quicker
    // be careful running this too fast, or else it will bog down the radio with too many ATCommandRequests
    rssiTimer = new QTimer();
    rssiTimer->setInterval(50);

    connect(rssiTimer, &QTimer::timeout, this, &Backend::updateRSSIInfo);
//    rssiTimer->start();
}

Backend::Backend(QObject *parent) : QObject(parent)
{
    loopCount = 0;
    throughputTestTimer = new QTimer();
    connect(throughputTestTimer, &QTimer::timeout, this, &Backend::throughputTestTimerTicked);

    // Create 2^16 bytes for each, but we can always realloc if absolutely necessary
    sDFileContents.packetType = GroundStation::SDFileContents;
    sDFileContents.bytes = new uint8_t[65536];
    sDFileContents.currentBytePointer = sDFileContents.bytes;

    sDDirectoryContents.packetType = GroundStation::SDDirectory;
    sDDirectoryContents.bytes = new uint8_t[65536];
    sDDirectoryContents.currentBytePointer = sDDirectoryContents.bytes;

    image.packetType = GroundStation::Image;
    image.bytes = new uint8_t[1048576]; // Give 1mb of space for an image
    image.currentBytePointer = image.bytes;



//    stateMaxValues = QList<MaxValues>(6);

//    dummyLogger = new DataLogger();
}
