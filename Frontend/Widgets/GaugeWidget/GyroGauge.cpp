//
// Created by William Scheirey on 2/19/25.
//

#include "GyroGauge.h"
#include "Backend/Backend.h"

GyroGauge::GyroGauge(QWidget *parent): GaugeWidget(1000, parent)
{
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        float totalAccel = sqrt(data->gyrox()*data->gyrox() + data->gyroy()*data->gyroy() + data->gyroz()*data->gyroz());

        updateValue(totalAccel > 0 ? totalAccel : 0);
    });
}