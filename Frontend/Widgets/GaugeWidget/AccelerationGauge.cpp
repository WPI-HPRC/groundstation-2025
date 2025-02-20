//
// Created by William Scheirey on 2/19/25.
//

#include "AccelerationGauge.h"
#include "Backend/Backend.h"

AccelerationGauge::AccelerationGauge(QWidget *parent): GaugeWidget(15, parent)
{
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        float totalAccel = sqrt(data->accelx()*data->accelx() + data->accely()*data->accely() + data->accelz()*data->accelz()) - 1;

        updateValue(totalAccel > 0 ? totalAccel : 0);
    });
}