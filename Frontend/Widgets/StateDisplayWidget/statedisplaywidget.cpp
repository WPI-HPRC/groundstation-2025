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

void StateDisplayWidget::telemetryAvailable(Backend::Packet telemetry)
{
    if(telemetry.packetType != GroundStation::Rocket)
    {
        return;
    }

    const char *accelerationLabel = Backend::getInstance().convertFromGees ? (Backend::getInstance().convertToEnglish ? "ft/s/s" : "m/s/s") : "G";
    const char *velocityLabel = Backend::getInstance().convertToEnglish ? "ft/s" : "m/s";
    const char *altitudeLabel = Backend::getInstance().convertToEnglish ? "ft" : "m";
    HPRC::RocketTelemetryPacket data = *telemetry.data.rocketData;
    if(data.state() == 0)
    {
        ui->TimeWidget->setVisible(false);
    }
    else
    {
        ui->TimeWidget->setVisible(true);
    }
    ui->StateName->setText(Backend::getInstance().RocketStateNames.at(data.state()));

    ui->AccelerationLabel->setText(QString::asprintf("%0.2f %s",
                                                     sqrt(data.accelx()*data.accelx() + data.accely()*data.accely() + data.accelz()*data.accelz()),
                                                     accelerationLabel
                                                     ));
    ui->VelocityLabel->setText(QString::asprintf("%0.2f %s",
                                                     sqrt(data.velx()*data.velx() + data.vely()*data.vely() + data.velz()*data.velz()),
                                                    velocityLabel
    ));

    ui->AglLabel->setText(QString::asprintf("%0.2f %s",
                                                 data.state() == 0 ? 0 : data.altitude() - Backend::getInstance().groundLevelAltitude,
                                                 altitudeLabel
    ));

    ui->MslLabel->setText(QString::asprintf("%0.2f %s",
                                            data.altitude(),
                                            altitudeLabel
    ));
}

StateDisplayWidget::~StateDisplayWidget()
{
    delete ui;
}
