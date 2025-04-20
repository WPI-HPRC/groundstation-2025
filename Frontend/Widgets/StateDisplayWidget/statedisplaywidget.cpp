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
    ui->TimeWidget->setLabel("T+");

    /*
    // Make it so setting the widget to be invisible doesn't effect its geometry.
    // From https://stackoverflow.com/questions/10794532/how-to-make-a-qt-widget-invisible-without-changing-the-position-of-the-other-qt
    QSizePolicy sp_retain = ui->TimeWidget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->TimeWidget->setSizePolicy(sp_retain);
*/
    
    ui->TimeWidget->setVisible(false);

    telemetryConnection = connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &StateDisplayWidget::telemetryAvailable);
}

void StateDisplayWidget::telemetryAvailable(const HPRC::Telemetry& telemetry)
{
    if(!telemetry.has_rocketpacket())
    {
        return;
    }

    const char *accelerationLabel = Backend::getInstance().convertFromGees ? (Backend::getInstance().convertToEnglish ? "ft/s/s" : "m/s/s") : "G";
    const char *velocityLabel = Backend::getInstance().convertToEnglish ? "ft/s" : "m/s";
    const char *altitudeLabel = Backend::getInstance().convertToEnglish ? "ft" : "m";
    const HPRC::RocketTelemetryPacket& packet = telemetry.rocketpacket();
    if(packet.state() == 0)
    {
        ui->TimeWidget->setVisible(false);
    }
    else
    {
        ui->TimeWidget->setVisible(true);
    }
    ui->StateName->setText(Backend::getInstance().RocketStateNames.at(packet.state()));

    ui->AccelerationLabel->setText(QString::asprintf("%0.2f %s",
                                                     sqrt(packet.accelx() * packet.accelx() + packet.accely() * packet.accely() + packet.accelz() * packet.accelz()),
                                                     accelerationLabel
                                                     ));
    ui->VelocityLabel->setText(QString::asprintf("%0.2f %s",
                                                     sqrt(packet.velx() * packet.velx() + packet.vely() * packet.vely() + packet.velz() * packet.velz()),
                                                    velocityLabel
    ));

    ui->AglLabel->setText(QString::asprintf("%0.2f %s",
                                            packet.state() == 0 ? 0 : packet.altitude() - Backend::getInstance().groundLevelAltitude,
                                            altitudeLabel
    ));

    ui->MslLabel->setText(QString::asprintf("%0.2f %s",
                                            packet.altitude(),
                                            altitudeLabel
    ));
}

StateDisplayWidget::~StateDisplayWidget()
{
    delete ui;
}
