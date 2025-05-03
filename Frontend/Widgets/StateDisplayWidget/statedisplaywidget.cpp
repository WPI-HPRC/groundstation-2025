//
// Created by William Scheirey on 12/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StateDisplayWidget.h" resolved

#include "statedisplaywidget.h"
#include "ui_StateDisplayWidget.h"

StateDisplayWidget::StateDisplayWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::StateDisplayWidget)
{
    ui->setupUi(this);
    ui->RocketTimeWidget->setLabel("T+");
    ui->PayloadTimeWidget->setLabel("T+");



    // Make it so setting the widget to be invisible doesn't effect its geometry.
    // From https://stackoverflow.com/questions/10794532/how-to-make-a-qt-widget-invisible-without-changing-the-position-of-the-other-qt
    QSizePolicy sp_retain = ui->RocketTimeWidget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->RocketTimeWidget->setSizePolicy(sp_retain);

    sp_retain = ui->PayloadTimeWidget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->PayloadTimeWidget->setSizePolicy(sp_retain);
    
    ui->RocketTimeWidget->setVisible(false);
    ui->PayloadTimeWidget->setVisible(false);


    telemetryConnection = connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &StateDisplayWidget::telemetryAvailable);
}

void StateDisplayWidget::telemetryAvailable(const HPRC::Telemetry& telemetry)
{
    const char *accelerationLabel = Backend::getInstance().convertFromGees ? (Backend::getInstance().convertToEnglish ? "ft/s/s" : "m/s/s") : "G";
    const char *velocityLabel = Backend::getInstance().convertToEnglish ? "ft/s" : "m/s";
    const char *altitudeLabel = Backend::getInstance().convertToEnglish ? "ft" : "m";

    if(telemetry.has_rocketpacket())
    {
        const HPRC::RocketTelemetryPacket& packet = telemetry.rocketpacket();
        if(packet.state() > Backend::getInstance().RocketStateNames.length())
        {
            return;
        }
        if(packet.state() == 0)
        {
            ui->RocketTimeWidget->setVisible(false);
        }
        else
        {
            ui->RocketTimeWidget->setVisible(true);
        }
        ui->RocketStateName->setText(QString("Rocket: ") + Backend::getInstance().RocketStateNames.at(packet.state()));

        ui->RocketAccelerationLabel->setText(QString::asprintf("%0.2f %s",
                                                               sqrt(packet.accelx() * packet.accelx() + packet.accely() * packet.accely() + packet.accelz() * packet.accelz()),
                                                               accelerationLabel
        ));
        ui->RocketVelocityLabel->setText(QString::asprintf("%0.2f %s",
                                                           sqrt(packet.velx() * packet.velx() + packet.vely() * packet.vely() + packet.velz() * packet.velz()),
                                                           velocityLabel
        ));

        ui->RocketAglLabel->setText(QString::asprintf("%0.2f %s",
                                                      packet.state() == 0 ? 0 : packet.altitude() - Backend::getInstance().groundLevelAltitude,
                                                      altitudeLabel
        ));

        ui->RocketMslLabel->setText(QString::asprintf("%0.2f %s",
                                                      packet.altitude(),
                                                      altitudeLabel
        ));
    }
    else if(telemetry.has_payloadpacket())
    {
        const HPRC::PayloadTelemetryPacket& packet = telemetry.payloadpacket();

        if(packet.state() > Backend::getInstance().PayloadStateNames.length())
        {
            return;
        }

        if(packet.state() == 0)
        {
            ui->PayloadTimeWidget->setVisible(false);
        }
        else
        {
            ui->PayloadTimeWidget->setVisible(true);
        }
        ui->PayloadStateName->setText(QString("Payload: ") + Backend::getInstance().PayloadStateNames.at(packet.state()));

        ui->PayloadAccelerationLabel->setText(QString::asprintf("%0.2f %s",
                                                               sqrt(packet.accelx() * packet.accelx() + packet.accely() * packet.accely() + packet.accelz() * packet.accelz()),
                                                               accelerationLabel
        ));
        ui->PayloadVelocityLabel->setText(QString::asprintf("%0.2f %s",
                                                           sqrt(packet.velx() * packet.velx() + packet.vely() * packet.vely() + packet.velz() * packet.velz()),
                                                           velocityLabel
        ));

        ui->PayloadAglLabel->setText(QString::asprintf("%0.2f %s",
                                                      packet.state() == 0 ? 0 : packet.altitude() - Backend::getInstance().groundLevelAltitude,
                                                      altitudeLabel
        ));

        ui->PayloadMslLabel->setText(QString::asprintf("%0.2f %s",
                                                      packet.altitude(),
                                                      altitudeLabel
        ));
    }


}

StateDisplayWidget::~StateDisplayWidget()
{
    delete ui;
}
