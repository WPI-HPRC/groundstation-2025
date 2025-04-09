//
// Created by William Scheirey on 1/6/25.
//

#include "RocketGpsIndicator.h"
#include "Backend/Backend.h"

RocketGpsIndicator::RocketGpsIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Rocket GPS");
    this->timeoutDuration_sec = 1;
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](Backend::Packet telemetry)
    {
        if(telemetry.packetType == GroundStation::Rocket)
        {
            if(telemetry.data.rocketData->gpslock())
            {
                this->resetTimer();
            }
        }
    });
}