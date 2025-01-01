//
// Created by William Scheirey on 12/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StateSummaryWidget.h" resolved

#include "statesummarywidget.h"
#include "ui_StateSummaryWidget.h"


StateSummaryWidget::StateSummaryWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::StateSummaryWidget)
{
    ui->setupUi(this);

    QSizePolicy sp_retain = ui->Frame3->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->Frame3->setSizePolicy(sp_retain);

    sp_retain = ui->AltitudeLabel->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->AltitudeLabel->setSizePolicy(sp_retain);

    sp_retain = ui->TimeLabel->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->TimeLabel->setSizePolicy(sp_retain);
}


void StateSummaryWidget::setMaxValues(Backend::MaxValues values, int state)
{
    if(Backend::getInstance().convertToEnglish)
    {
        using namespace Utility::UnitConversion;
        values.maxAltitude = meters2feet(values.maxAltitude);
        if(Backend::getInstance().convertFromGees)
        {
            values.maxAcceleration = gs2feet(values.maxAcceleration);
        }
        values.maxVelocity = meters2feet(values.maxVelocity);
        values.maxTemperature = cel2far(values.maxTemperature);
        values.minTemperature = cel2far(values.minTemperature);
        values.maxPressure = mbar2psi(values.maxPressure);
        values.minPressure = mbar2psi(values.minPressure);
    }
    else if(Backend::getInstance().convertFromGees)
    {
        values.maxAcceleration = Utility::UnitConversion::gs2meters(values.maxAcceleration);
    }

    const char *accelerationLabel = Backend::getInstance().convertFromGees ? (Backend::getInstance().convertToEnglish ? "ft/s/s" : "m/s/s") : "G";
    const char *velocityLabel = Backend::getInstance().convertToEnglish ? "ft/s" : "m/s";

    ui->StateName->setText(Backend::getInstance().RocketStateNames.at(state));

    ui->AccelerationLabel->setText(QString::asprintf("%0.2f %s",
                                                     values.maxAcceleration,
                                                     accelerationLabel
    ));
    ui->VelocityLabel->setText(QString::asprintf("%0.2f %s",
                                                 values.maxVelocity,
                                                 velocityLabel
    ));
}

void StateSummaryWidget::hideValues()
{
    ui->Frame3->setVisible(false);
    ui->TimeLabel->setVisible(false);
    ui->AltitudeLabel->setVisible(false);
    ui->ProgressBar->setStyleSheet("");
}

void StateSummaryWidget::showValues()
{
    ui->Frame3->setVisible(true);
}

void StateSummaryWidget::setTitle(const QString &title)
{
    ui->StateName->setText(title);
}

void StateSummaryWidget::setStart(Backend::MaxValues values)
{
    uint32_t rocketFlightTime = Backend::getInstance().rocketFlightTime;
    uint_fast16_t hours = rocketFlightTime / (60 * 60 * 1000);
    uint_fast8_t minutes = rocketFlightTime / (60*1000) % 60;
    uint_fast8_t seconds = rocketFlightTime / 1000 % 60;
    uint_fast16_t milliseconds = rocketFlightTime % 1000;
    ui->TimeLabel->setText(QString::asprintf("T+%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->TimeLabel->setVisible(true);


    const char *altitudeLabel = Backend::getInstance().convertToEnglish ? "ft" : "m";
    ui->AltitudeLabel->setText(
            QString::asprintf("%0.2f %s",
                              values.minAltitude - Backend::getInstance().groundLevelAltitude,
                              altitudeLabel
            ));
    ui->AltitudeLabel->setVisible(true);
    ui->ProgressBar->setStyleSheet("color: #AF283A");
}

void StateSummaryWidget::setPreLaunch()
{
    QSizePolicy sp_retain = ui->TimeLabel->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(false);
    ui->TimeLabel->setSizePolicy(sp_retain);
}

StateSummaryWidget::~StateSummaryWidget()
{
    delete ui;
}
