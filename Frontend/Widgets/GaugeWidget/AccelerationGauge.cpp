//
// Created by William Scheirey on 2/19/25.
//

#include "AccelerationGauge.h"
#include "Backend/Backend.h"
#include <QSizePolicy>

AccelerationGauge::AccelerationGauge(QWidget *parent): GaugeDisplay(parent)
{
    gauge = new GaugeWidget(13, this);
    gauge->lower();

    this->setGaugeWidget(gauge);
    this->updateLabel("Acceleration (G)");

    gauge->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        float totalAccel = sqrt(data->accelx()*data->accelx() + data->accely()*data->accely() + data->accelz()*data->accelz());
        this->gauge->updateValue(totalAccel);
        this->updateNumber(QString::number(round(totalAccel)));
        if((int)currentValue != (int)totalAccel)
        {
            this->updateNumber(QString::number(round(totalAccel)));
        }
        currentValue = totalAccel;
    });
}