//
// Created by William Scheirey on 2/19/25.
//

#include "GyroGauge.h"
#include "Backend/Backend.h"
#include "Utility/UnitConversions.h"

GyroGauge::GyroGauge(QWidget *parent): GaugeDisplay(parent)
{
    gauge = new GaugeWidget(500, this);
    gauge->lower();

    this->setGaugeWidget(gauge);
    this->updateLabel("Spin Rate (rpm)");

    gauge->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](const HPRC::Telemetry& telemetry)
    {
        if(!telemetry.has_rocketpacket())
            return;
        const HPRC::RocketTelemetryPacket& data = telemetry.rocketpacket();
        // Convert to rad/s to rpm
        float totalSpin = Utility::UnitConversion::RadPerSecToRevPerMin * sqrt(data.gyrox()*data.gyrox() + data.gyroy()*data.gyroy() + data.gyroz()*data.gyroz());

        this->gauge->updateValue(totalSpin);
        this->updateNumber(QString::number(round(totalSpin)));
        if((int)currentValue != (int)totalSpin)
        {
            this->updateNumber(QString::number(round(totalSpin)));
        }
        currentValue = totalSpin;
    });
}