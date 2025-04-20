//
// Created by William Scheirey on 12/16/24.
//

#include "RocketConnectionIndicator.h"
#include "Backend/Backend.h"

RocketConnectionIndicator::RocketConnectionIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Rocket");
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](const HPRC::Telemetry& telemetry)
    {
        if(telemetry.has_payloadpacket())
        {
            this->resetTimer();
        }
    });
}