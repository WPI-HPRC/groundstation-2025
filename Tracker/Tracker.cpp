//
// Created by William Scheirey on 5/5/25.
//

#include "Tracker.h"
Tracker::Tracker(QObject *parent): QObject(parent)
{

}

void Tracker::connectToPort(const QSerialPortInfo &port, int baudRate, DataLogger *dataLogger)
{
    serialPort = new SerialPort(port, baudRate, new DataLogger("tracker_"));
}

void Tracker::send(const char *buffer, size_t length_bytes)
{
//    Serial.write(buffer, length_bytes);
}

void Tracker::read()
{
    // Read bytes until there aren't any more to be read
    do
    {
        size_t bytesRead = serialPort->read(readBuffer, READ_BUFFER_LENGTH);

        if(readBuffer[readBufferIndex+bytesRead == 'E'])
        {
            // Now we know a packet has been read completely, so handle it here
            handleMessage(readBuffer);
            readBufferIndex = 0;
            // Optionally, break here if we only want to read/handle one packet maximum per loop
        }
        else
        {
            readBufferIndex += bytesRead;
        }
    } while (Serial.peek() != -1);
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

    float azimuth_degrees = (float)utf8DigitsToInt(buffer, buffer_index) / 100;

    char *remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float elevation_degrees = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    // Handle the data
}

void Tracker::handleData_gps(const char *buffer)
{
    int buffer_index = bytesUntilSemicolon(buffer);

    float longitude_decimal = (float)utf8DigitsToInt(buffer, buffer_index) / 100;

    char *remainingBuffer = (char *)&buffer[buffer_index + 1];
    buffer_index = bytesUntilSemicolon(remainingBuffer);

    float latitude_decimal = (float)utf8DigitsToInt(remainingBuffer, buffer_index) / 100;

    // Handle the data
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

    // Handle the data
}

void Tracker::handleData(const char *buffer)
{
    char dataType = buffer[0];

    switch (dataType)
    {
        case 'P':
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
    // Handle the response
}

void Tracker::handleEstopResponse_brake()
{
    // Handle the response
}

void Tracker::handleEstopResponse_coast()
{
    // Handle the response
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