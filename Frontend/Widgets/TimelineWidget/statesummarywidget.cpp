//
// Created by William Scheirey on 12/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StateSummaryWidget.h" resolved

#include "statesummarywidget.h"
#include "ui_StateSummaryWidget.h"

QList<QString> StateSummaryWidget::RocketStateNames = {
        "Pre-Launch",
        "Launch",
        "Coast",
        "Drogue Descent",
        "Main Descent",
        "Recovery",
        "Abort"
};

StateSummaryWidget::StateSummaryWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::StateSummaryWidget)
{
    ui->setupUi(this);
}


void StateSummaryWidget::setMaxValues(Backend::MaxValues values, int state, int flightTime)
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
    const char *altitudeLabel = Backend::getInstance().convertToEnglish ? "ft" : "m";

    ui->StateName->setText(RocketStateNames.at(state));

    ui->AccelerationLabel->setText(QString::asprintf("%0.2f - %0.2f %s",
                                                     values.minAcceleration,
                                                     values.maxAcceleration,
                                                     accelerationLabel
    ));
    ui->VelocityLabel->setText(QString::asprintf("%0.2f - %0.2f %s",
                                                 values.minVelocity,
                                                 values.maxVelocity,
                                                 velocityLabel
    ));
    ui->AltitudeLabel->setText(
            QString::asprintf("%0.2f %s",
                              values.minAltitude - Backend::getInstance().groundLevelAltitude,
                              altitudeLabel
    ));
}

StateSummaryWidget::~StateSummaryWidget()
{
    delete ui;
}
