//
// Created by William Scheirey on 5/5/25.
//

#include "Tracker.h"
Tracker::Tracker(QObject *parent): QObject(parent)
{
    dataLogger = new DataLogger("tracker_");
    readTimer = new QTimer(this);

    readTimer->setInterval(10);
    connect(readTimer, &QTimer::timeout, this, [this]()
    {
        read();
    });
    readTimer->start();
}

void Tracker::connectToPort(const QSerialPortInfo &port, int baudRate)
{
    portInfo = port;
    if(serialPort)
    {
        serialPort->close();
    }
    serialPort = new SerialPort(port, baudRate, dataLogger);
}

void Tracker::disconnect()
{
    QSerialPortInfo portInfo = serialPort->getPortInfo();
    serialPort->close();
    serialPort = nullptr;
    emit portClosed(portInfo);
}

void Tracker::send(const char *buffer, size_t length_bytes)
{
    if(!serialPort)
    {
        return;
    }
    serialPort->write(buffer, length_bytes);
}

void Tracker::read()
{
    if(!serialPort || !serialPort->isOpen())
    {
        return;
    }
    size_t bytesRead = serialPort->read(&readBuffer[readBufferIndex], READ_BUFFER_LENGTH - readBufferIndex);

    if(bytesRead == 0)
    {
        return;
    }

    QString str = "";
    for (int i = 0; i < bytesRead; i++)
    {
        str.append(QString::asprintf("%c", readBuffer[i]));
    }
    emit dataRead(str);

    handleMessage(readBuffer);
return;
    for(int i = 0; i < bytesRead; i++)
    {
        if (readBuffer[readBufferIndex + i] == 'E')
        {
            // Now we know a packet has been read completely, so handle it here
            handleMessage(&readBuffer[readBufferIndex]);
            readBufferIndex = i;
            // Optionally, break here if we only want to read/handle one packet maximum per loop
        }
    }
/*
    if(readBufferIndex < READ_BUFFER_LENGTH)
    {
        memcpy(&readBuffer, &readBuffer[readBufferIndex], bytesRead);
        readBufferIndex = readBuffer;
    }
    */
}


// Takes UTF-8 encoded digits and converts them into an integer. Assumes the most significant digit comes first
int Tracker::utf8DigitsToInt(const char *encodedDigits, size_t length_bytes)
{
    int intValue = 0;

    for(int i = 0; i < length_bytes; i++)
    {
        char currentChar = encodedDigits[i];

        if('0' <= currentChar && currentChar <= '9') // Make sure the byte we're reading is a valid UTF-8-encoded integer
        {
            // To find the magnitude of the current digit, take the total number of digits minus 1 and subtract from it the index of the current digit
            // Raise 10 to that power and multiply it by the digit's integer value, which is the encoded number as an integer minus the character '0' as an integer
            intValue += pow(10, (length_bytes - 1 - i)) * (currentChar - '0');
        }
        else
        {
            return -1;
        }
    }
    return intValue;
}

int Tracker::bytesUntilSemicolon(const char *buffer)
{
    // We assume there is actually a semicolon in the buffer
    int i = 0;
    while(buffer[i] != ';') { i++; }
    return i;
}

void Tracker::handleData_pose(const char *buffer)
{
    int buffer_index = bytesUntilSemicolon(buffer);

    float azimuth_degrees = 0;

    if(buffer[0] == '-')
    {
        azimuth_degrees = -(float)utf8DigitsToInt(&buffer[1], buffer_index-1) / 100;
    }
    else
    {
        azimuth_degrees = (float)utf8DigitsToInt(buffer, buffer_index) / 100;
    }

    char *remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float elevation_degrees = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    actualPose = {azimuth_degrees, elevation_degrees};

    // Handle the data
    emit newPoseData(actualPose);
}

void Tracker::handleData_gps(const char *buffer)
{
    int buffer_index = bytesUntilSemicolon(buffer);

    float longitude_decimal = (float)utf8DigitsToInt(buffer, buffer_index) / 100;

    char *remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float latitude_decimal = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    emit newGpsData(longitude_decimal, latitude_decimal);
}

void Tracker::handleData_imu(const char *buffer)
{
    int buffer_index = bytesUntilSemicolon(buffer);

    float gravityX_gs = (float)utf8DigitsToInt(buffer, buffer_index) / 100;

    char *remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float gravityY_gs = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float gravityZ_gs = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float heading_degrees = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    emit newImuData(gravityX_gs, gravityY_gs, gravityZ_gs, heading_degrees);
}

void Tracker::handleData(const char *buffer)
{
    char dataType = buffer[0];

    switch (dataType)
    {
        case 'L':
        {
            handleData_pose(&buffer[2]);
            break;
        }
        case 'G':
        {
            handleData_gps(&buffer[2]);
            break;
        }
        case 'I':
        {
            handleData_imu(&buffer[2]);
            break;
        }
        case 'B':
        {
            handleEstopResponse_brake();
            break;
        }
        case 'C':
        {
            handleEstopResponse_coast();
            break;
        }
        default:
        {
            return;
        }
    }
}

void Tracker::handleResponse_pose()
{
    emit newPoseResponse();
}

void Tracker::handleEstopResponse_brake()
{
    emit newEstopResponse_brake();
}

void Tracker::handleEstopResponse_coast()
{
    emit newEstopResponse_coast();
}

void Tracker::handleResponse(const char *buffer)
{
    char responseType = buffer[0];

    switch(responseType)
    {
        case 'P':
        {
            handleResponse_pose();
            break;
        }
        default:
        {
            return;
        }
    }
}

void Tracker::handleMessage(const char *buffer)
{
    // For now, we will just read the first byte since all message types are given by a single byte. In the future, we can change this
    char messageType = buffer[0];
    switch(messageType)
    {
        case 'D': // Command response
        {
            // Skip the semicolon in the message
            handleData(&buffer[2]);
            break;
        }
        case 'R': // Data response
        {
            handleResponse(&buffer[2]);
            break;
        }
        default:
        {
            // Should not happen
            return;
        }
    }
}

void Tracker::sendString(QString str)
{
    send(str.toStdString().c_str(), str.length());
}

void Tracker::sendMessage_setPose(Pose pose)
{
    QString str = QString::asprintf("S;P;%d;%d;E", qRound(pose.azimuth_degrees*100), qRound(pose.elevation_degrees*100));
    sendString(str);
    desiredPose = pose;
    emit newDesiredPose(pose);
}

void Tracker::sendMessage_getPose()
{
    sendString("G;L;E");
}

void Tracker::sendMessage_getGps()
{
    sendString("G;G;E");
}

void Tracker::sendMessage_getImu()
{
    sendString("G;I;E");
}

void Tracker::sendEstop_brake()
{
    sendString("G;B;E");
}

void Tracker::sendEstop_coast()
{
    sendString("G;C;E");
}

void Tracker::sendMessage_home()
{
    sendString("S;H;E");
}

Tracker::Pose Tracker::poseDifference()
{
    return {desiredPose.azimuth_degrees - actualPose.azimuth_degrees, desiredPose.elevation_degrees - actualPose.elevation_degrees};
}