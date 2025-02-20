//
// Created by William Scheirey on 2/20/25.
//


//
// Created by William Scheirey on 2/19/25.
//

#include "SpeedGauge.h"
#include "Backend/Backend.h"

SpeedGauge::SpeedGauge(QWidget *parent): GaugeDisplay(parent)
{
    gauge = new GaugeWidget(500, this);
    gauge->lower();

    this->setGaugeWidget(gauge);
    this->updateLabel("SPEED");
    this->setUnits("MPH");

    gauge->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        float speed = sqrt(data->velx()*data->velx() + data->vely()*data->vely() + data->velz()*data->velz())*2.23694;

        this->gauge->updateValue(speed);
        this->updateNumber(QString::number(round(speed)));
        if((int)currentValue != (int)speed)
        {
            this->updateNumber(QString::number(round(speed)));
        }
        currentValue = speed;
    });
}