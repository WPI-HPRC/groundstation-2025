//
// Created by William Scheirey on 2/19/25.
//

#include "GyroGauge.h"
#include "Backend/Backend.h"

GyroGauge::GyroGauge(QWidget *parent): GaugeDisplay(parent)
{
    gauge = new GaugeWidget(500, this);
    gauge->lower();

    this->setGaugeWidget(gauge);
    this->updateLabel("Spin Rate (rpm)");

    gauge->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        float totalSpin = sqrt(data->gyrox()*data->gyrox() + data->gyroy()*data->gyroy() + data->gyroz()*data->gyroz())*9.549297;

        this->gauge->updateValue(totalSpin);
        this->updateNumber(QString::number(round(totalSpin)));
        if((int)currentValue != (int)totalSpin)
        {
            this->updateNumber(QString::number(round(totalSpin)));
        }
        currentValue = totalSpin;
    });
}