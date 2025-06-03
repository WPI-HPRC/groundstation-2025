//
// Created by William Scheirey on 3/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RemoteControlWindow.h" resolved

#include "remotecontrolwindow.h"
#include "ui_RemoteControlWindow.h"
#include "Backend/Backend.h"
#include <QMessageBox>

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

    connect(ui->ToggleVideoButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        videoIsActive = !videoIsActive;
        packet.mutable_command()->mutable_setvideoactive()->set_videoactive(videoIsActive);
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());

        ui->ToggleVideoButton->setText(videoIsActive ? "Disable Video" : "Enable Video");

    });

    connect(ui->ToggleAcksButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        acksAreEnabled = !acksAreEnabled;
        packet.mutable_command()->mutable_setacksenabled()->set_acksenabled(acksAreEnabled);
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());

        ui->ToggleAcksButton->setText(acksAreEnabled ? "Disable Acknowledgements" : "Enable Acknowledgements");
    });

    connect(ui->EnableFlightModeButton, &QPushButton::released, this, [this]()
    {
        QMessageBox msgBox;
        msgBox.setText("Enable Flight Mode? You cannot reverse this action.");

        QPushButton *no = msgBox.addButton("please no", QMessageBox::NoRole);
        QPushButton *yes = msgBox.addButton("SEND IT", QMessageBox::YesRole);

        msgBox.setDefaultButton(no);

        msgBox.exec();

        if(msgBox.clickedButton() == yes)
        {
            HPRC::Packet packet;
            packet.mutable_command()->mutable_setflightmode()->set_flightmodeon(true);
            Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
            QMessageBox::information(this, "Enable Flight Mode Command Sent", "There's no going back now.");
        }

    });

    connect(ui->DisableFlightModeButton, &QPushButton::released, this, [this]()
    {
        QMessageBox msgBox;
        msgBox.setText("Disable Flight Mode? You will need to send this 3 times in a row.");

        QPushButton *no = msgBox.addButton("No ^-^", QMessageBox::YesRole);
        QPushButton *yes = msgBox.addButton("Yes :3", QMessageBox::NoRole);

        msgBox.setDefaultButton(no);

        msgBox.exec();

        if(msgBox.clickedButton() == yes)
        {
            HPRC::Packet packet;
            packet.mutable_command()->mutable_setflightmode()->set_flightmodeon(false);
            Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
            QMessageBox::information(this, "Disable Flight Mode Command Sent", "Reminder: you need to send this three times ina  row.");
        }
    });
    connect(ui->PowerCycleButton, &QPushButton::released, this, [this]()
    {
        HPRC::Packet packet;
        packet.mutable_command()->mutable_powercycle();
        Backend::getInstance().transmitPacketThroughModem(packet, getDestinationAddress());
    });


}

RemoteControlWindow::~RemoteControlWindow()
{
    delete ui;
}
