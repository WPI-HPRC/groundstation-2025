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
    this->updateLabel("G-FORCE");
    this->setUnits("");

    gauge->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](const HPRC::Telemetry& telemetry)
    {
        if(!telemetry.has_rocketpacket())
            return;
        const HPRC::RocketTelemetryPacket& data = telemetry.rocketpacket();
        float totalAccel = sqrt(data.accelx()*data.accelx() + data.accely()*data.accely() + data.accelz()*data.accelz());
        this->gauge->updateValue(totalAccel);
        this->updateNumber(QString::number(round(totalAccel)));
        if((int)currentValue != (int)totalAccel)
        {
            this->updateNumber(QString::number(round(totalAccel)));
        }
        currentValue = totalAccel;
    });
}