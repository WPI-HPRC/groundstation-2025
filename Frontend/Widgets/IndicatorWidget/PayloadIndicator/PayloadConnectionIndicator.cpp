//
// Created by William Scheirey on 12/16/24.
//

#include "PayloadConnectionIndicator.h"
#include "Backend/Backend.h"

PayloadConnectionIndicator::PayloadConnectionIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Payload");
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](const HPRC::Telemetry& telemetry)
    {
        if(telemetry.has_payloadpacket())
        {
            this->resetTimer();
        }
    });
}