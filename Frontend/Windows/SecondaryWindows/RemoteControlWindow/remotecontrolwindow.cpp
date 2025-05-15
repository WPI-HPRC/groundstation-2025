//
// Created by William Scheirey on 3/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RemoteControlWindow.h" resolved

#include "remotecontrolwindow.h"
#include "ui_RemoteControlWindow.h"
#include "Backend/Backend.h"

uint64_t RemoteControlWindow::getDestinationAddress()
{
    QByteArray bytes = Backend::hexToBytes(ui->DestinationAddress->text());
    return Backend::getAddressBigEndian((uint8_t *)bytes.data());
}

RemoteControlWindow::RemoteControlWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::RemoteControlWindow)
{
    ui->setupUi(this);
    ui->DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");

    connect(ui->AirbrakesSlider, &QSlider::valueChanged, this, [this](int value)
    {
        HPRC::Packet packet;
        packet.mutable_command()->mutable_actuateairbrakes()->set_servovalue(value);

        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
    });
    connect(ui->ReadSDDirectoryButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        packet.mutable_command()->mutable_readsddirectory();
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
    });
    connect(ui->ReadSDFileButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        packet.mutable_command()->mutable_readsdfile()->set_filename(ui->SDFileName->text().toStdString().c_str());
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
    });

    connect(ui->ClearSDButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        packet.mutable_command()->mutable_clearsd();
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
    });
}

RemoteControlWindow::~RemoteControlWindow()
{
    delete ui;
}
