//
// Created by William Scheirey on 5/5/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TrackerWindow.h" resolved

#include "trackerwindow.h"
#include "ui_TrackerWindow.h"
#include "Tracker/Tracker.h"
#include "Backend/Backend.h"

TrackerWindow::TrackerWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::TrackerWindow)
{
    ui->setupUi(this);

    ui->AnglesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->AnglesTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(&Tracker::getInstance(), &Tracker::newPoseData, this, [this](Tracker::Pose pose)
    {
        updateTableValue(0, 1, QString::number(pose.azimuth_degrees, 'f', 2));
        updateTableValue(1, 1, QString::number(pose.elevation_degrees, 'f', 2));
        updateAngleDifferences();
    });
    connect(&Tracker::getInstance(), &Tracker::newGpsData, this, [this](float latitude_decimal, float longitude_decimal)
    {
       ui->LatitudeValue->setText(QString::number(latitude_decimal, 'f', 2));
       ui->LongitudeValue->setText(QString::number(longitude_decimal, 'f', 2));
    });
    connect(&Tracker::getInstance(), &Tracker::newDesiredPose, this, [this](Tracker::Pose pose)
    {
        updateTableValue(0, 0, QString::number(pose.azimuth_degrees, 'f', 2));
        updateTableValue(1, 0, QString::number(pose.elevation_degrees, 'f', 2));
        updateAngleDifferences();
    });

    connect(&Tracker::getInstance(), &Tracker::newImuData, [this](float gravityX_gs, float gravityY_gs, float gravityZ_gs, float heading_degrees)
    {
        ui->HeadingValue->setText(QString::number(heading_degrees));
    });

    connect(ui->AzimuthDial, &QDial::valueChanged, this, [this](int value)
    {
        ui->AzimuthDialValue->setText(QString::asprintf("%dº", value));
    });
    connect(ui->ElevationDial, &QDial::valueChanged, this, [this](int value)
    {
       ui->ElevationDialValue->setText(QString::asprintf("%dº", value));
    });

    connect(ui->SendPoseCommandButton, &QPushButton::released, this, [this]()
    {
        if(Tracker::getInstance().manualControlEnabled)
        {
            Tracker::getInstance().sendMessage_setPose(
                    {(float) ui->AzimuthDial->value(), (float) ui->ElevationDial->value()});
        }
    });

    connect(ui->EstopBrakeButton, &QPushButton::released, this, []()
    {
        Tracker::getInstance().sendEstop_brake();
    });
    connect(ui->EstopCoastButton, &QPushButton::released, this, []()
    {
        Tracker::getInstance().sendEstop_coast();
    });

    connect(ui->GetPoseButton, &QPushButton::released, this, []()
    {
        Tracker::getInstance().sendMessage_getPose();
    });

    connect(ui->HomeButton, &QPushButton::released, this, []()
    {
        Tracker::getInstance().sendMessage_home();
    });

    connect(ui->ToggleTrackerButton, &QPushButton::released, this, [this]()
    {
        Tracker::getInstance().enabled = !Tracker::getInstance().enabled;

        if(Tracker::getInstance().enabled)
        {
            ui->ToggleTrackerButton->setText("Disable Tracker");
            ui->SetPoseContainer->setEnabled(true);
            ui->ToggleManualModeButton->setEnabled(true);
        }
        else
        {
            ui->ToggleTrackerButton->setText("Enable Tracker");
            ui->SetPoseContainer->setEnabled(false);
            ui->ToggleManualModeButton->setEnabled(false);
        }
    });
    connect(ui->ToggleManualModeButton, &QPushButton::released, this, [this]()
    {
        Tracker::getInstance().manualControlEnabled = !Tracker::getInstance().manualControlEnabled;

        if(Tracker::getInstance().manualControlEnabled)
        {
            ui->ToggleManualModeButton->setText("Disable Manual Control");
            ui->SetPoseContainer->setEnabled(true);
        }
        else
        {
            ui->ToggleManualModeButton->setText("Enable Manual Control");
            ui->SetPoseContainer->setEnabled(false);
        }
    });
    connect(ui->TheSerialPortList, &SerialPortList::openSerialPort, this, [](const QString& portName, Backend::RadioModuleType _, int baud)
    {
        Tracker::getInstance().connectToPort(Backend::getTargetPort(portName), baud);
    });
    connect(ui->TheSerialPortList, &SerialPortList::closeSerialPort, this, [this](QString portName)
    {
        if(Tracker::getInstance().portInfo.portName() == portName)
        {
            Tracker::getInstance().disconnect();
        }
        else
        {
            ui->TheSerialPortList->serialPortOpened(Backend::getTargetPort(portName), true);
        }
    });
    connect(&Tracker::getInstance(), &Tracker::portClosed, ui->TheSerialPortList, &SerialPortList::serialPortClosed);
    connect(&Backend::getInstance(), &Backend::foundSerialPorts, ui->TheSerialPortList, &SerialPortList::serialPortsFound);
    connect(&Backend::getInstance(), &Backend::serialPortOpened,  ui->TheSerialPortList, &SerialPortList::serialPortOpened);
    connect(&Backend::getInstance(), &Backend::serialPortClosed,  ui->TheSerialPortList, &SerialPortList::serialPortClosed);

    connect(&Tracker::getInstance(), &Tracker::dataRead, this, [this](QString str)
    {
       ui->DataReadBox->append(str);
    });
    connect(ui->ClearTextButton, &QPushButton::released, this, [this]()
    {
        ui->DataReadBox->clear();
    });
}

void TrackerWindow::updateAngleDifferences()
{
    Tracker::Pose difference = Tracker::getInstance().poseDifference();
    updateTableValue(0, 2, QString::number(difference.azimuth_degrees, 'f', 2));
    updateTableValue(1, 2, QString::number(difference.elevation_degrees, 'f', 2));
}

void TrackerWindow::updateTableValue(int row, int column, QString value)
{
    auto *cell = (QLabel *)ui->AnglesTable->cellWidget(row, column);
    if(!cell)
    {
        ui->AnglesTable->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setText(value);
    }
}

TrackerWindow::~TrackerWindow()
{
    delete ui;
}
