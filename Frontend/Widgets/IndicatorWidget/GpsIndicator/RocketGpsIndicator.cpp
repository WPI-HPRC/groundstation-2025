//
// Created by William Scheirey on 1/6/25.
//

#include "RocketGpsIndicator.h"
#include "Backend/Backend.h"

RocketGpsIndicator::RocketGpsIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Rocket GPS");
    this->timeoutDuration_sec = 1;
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](HPRC::Telemetry telemetry)
    {
        if(telemetry.Message_case() == HPRC::Telemetry::kRocketPacket)
        {
            if(telemetry.rocketpacket().gpslock())
            {
                this->resetTimer();
            }
        }
    });
}