//
// Created by William Scheirey on 3/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RemoteControlWindow.h" resolved

#include "remotecontrolwindow.h"
#include "ui_RemoteControlWindow.h"
#include "Backend/Backend.h"


RemoteControlWindow::RemoteControlWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::RemoteControlWindow)
{
    ui->setupUi(this);

    connect(ui->AirbrakesSlider, &QSlider::valueChanged, this, [](int value)
    {
        if(!Backend::getInstance().groundStationModem)
        {
            return;
        }
        uint16_t transmitValue = value;
        uint32_t command = 0x000000AB | transmitValue << 8;
        Backend::getInstance().groundStationModem->sendTransmitRequestCommand(0x0013A200422CDAC4, (uint8_t *)&command, 4);
    });
    connect(ui->ReadSDDirectoryButton, &QPushButton::released, this, []()
    {
        if(!Backend::getInstance().groundStationModem)
        {
            return;
        }
        uint8_t command = 0xCD;
        Backend::getInstance().groundStationModem->sendTransmitRequestCommand(0x0013A200422CDAC4, &command, 1);
    });
    connect(ui->ReadSDFileButton, &QPushButton::released, this, [this]()
    {
        if(!Backend::getInstance().groundStationModem)
        {
            return;
        }
        QString filename = ui->SDFileName->text();

        uint8_t request[filename.length() + 2];
        request[0] = 0xFC;
        request[filename.length() + 1] = '\0';
        memcpy(&request[1], (uint8_t *)filename.toUtf8().data(), filename.length());

        // We need to reverse the bytes for how the packets are read on the flight computer
        XBeeDevice::reverseBytes(request, (int)sizeof(request));

        Backend::getInstance().groundStationModem->sendTransmitRequestCommand(0x0013A200422CDAC4, request, 1);
    });

    connect(ui->ClearSDButton, &QPushButton::released, this, [this]()
    {
        if(!Backend::getInstance().groundStationModem)
        {
            return;
        }
        uint8_t command = 0xCC;
        Backend::getInstance().groundStationModem->sendTransmitRequestCommand(0x0013A200422CDAC4, &command, 1);
    });
}

RemoteControlWindow::~RemoteControlWindow()
{
    delete ui;
}
