//
// Created by William Scheirey on 12/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StateDisplayWidget.h" resolved

#include "statedisplaywidget.h"
#include "ui_StateDisplayWidget.h"

QList<QString> StateDisplayWidget::RocketStateNames = {
        "Pre-Launch",
        "Launch",
        "Coast",
        "Drogue Descent",
        "Main Descent",
        "Recovery",
        "Abort"
};

StateDisplayWidget::StateDisplayWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::StateDisplayWidget)
{
    ui->setupUi(this);
    ui->TimeWidget->setLabel("T+");
    ui->TimeWidget->setVisible(false);

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &StateDisplayWidget::telemetryAvailable);
}

void StateDisplayWidget::telemetryAvailable(Backend::Telemetry telemetry)
{
    if(telemetry.packetType != GroundStation::Rocket)
    {
        return;
    }

    const char *accelerationLabel = Backend::getInstance().convertFromGees ? (Backend::getInstance().convertToEnglish ? "ft/s/s" : "m/s/s") : "G";
    const char *velocityLabel = Backend::getInstance().convertToEnglish ? "ft/s" : "m/s";
    const char *altitude = Backend::getInstance().convertToEnglish ? "ft" : "m";
    HPRC::RocketTelemetryPacket data = *telemetry.data.rocketData;
    if(data.state() == 0)
    {
        ui->TimeWidget->setVisible(false);
    }
    else
    {
        ui->TimeWidget->setVisible(true);
    }
    ui->StateName->setText(RocketStateNames.at(data.state()));

    ui->AccelerationLabel->setText(QString::asprintf("Acceleration: %0.2f %s",
                                                     sqrt(data.accelx()*data.accelx() + data.accely()*data.accely() + data.accelz()*data.accelz()),
                                                     accelerationLabel
                                                     ));
    ui->VelocityLabel->setText(QString::asprintf("Velocity: %0.2f %s",
                                                     sqrt(data.velx()*data.velx() + data.vely()*data.vely() + data.velz()*data.velz()),
                                                    velocityLabel
    ));

    ui->AltitudeLabel->setText(QString::asprintf("Altitude: %0.2f %s",
                                                 data.altitude(),
                                                 altitude
    ));
}

StateDisplayWidget::~StateDisplayWidget()
{
    delete ui;
}
