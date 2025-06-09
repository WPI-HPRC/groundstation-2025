//
// Created by William Scheirey on 6/8/25.
//

#include "Pointer.h"

//
// Created by William Scheirey on 5/5/25.
//

Pointer::Pointer(QObject *parent): QObject(parent)
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

void Pointer::connectToPort(const QSerialPortInfo &port, int baudRate)
{
    portInfo = port;
    if(serialPort)
    {
        serialPort->close();
    }
    serialPort = new SerialPort(port, baudRate, dataLogger);
}

void Pointer::disconnect()
{
    QSerialPortInfo portInfo = serialPort->getPortInfo();
    serialPort->close();
    serialPort = nullptr;
    emit portClosed(portInfo);
}

void Pointer::read()
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

    int bytesInPitch = bytesUntilCharacter(readBuffer, ',');
    float pitch = 0;
    if(readBuffer[0] == '-')
    {
        pitch = - (float) utf8DigitsToInt(&readBuffer[1], bytesInPitch-1) / 100;
    }
    else
    {
        pitch = (float) utf8DigitsToInt(readBuffer, bytesInPitch) / 100.f;
    }
    char *remainingBuffer = &readBuffer[bytesInPitch+1];
    int bytesInYaw = bytesUntilCharacter(remainingBuffer, ';');
    float yaw = 0;
    if(remainingBuffer[0] == '-')
    {
        yaw = - (float) utf8DigitsToInt(&remainingBuffer[1], bytesInYaw-1) / 100.f;
    }
    else
    {
        yaw = (float) utf8DigitsToInt(remainingBuffer, bytesInYaw) / 100.f;
    }

    QString str = "";
    for (int i = 0; i < bytesRead; i++)
    {
        str.append(QString::asprintf("%c", readBuffer[i]));
    }

    emit dataRead(str);
    emit newPoseData({yaw, pitch});
}


// Takes UTF-8 encoded digits and converts them into an integer. Assumes the most significant digit comes first
int Pointer::utf8DigitsToInt(const char *encodedDigits, size_t length_bytes)
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

int Pointer::bytesUntilCharacter(const char *buffer, const char character)
{
    // We assume there is actually a semicolon in the buffer
    int i = 0;
    while(buffer[i] != character) { i++; }
    return i;
}