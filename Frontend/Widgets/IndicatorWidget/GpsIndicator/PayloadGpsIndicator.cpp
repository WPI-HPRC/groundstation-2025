//
// Created by William Scheirey on 1/6/25.
//

#include "PayloadGpsIndicator.h"
#include "Backend/Backend.h"

PayloadGpsIndicator::PayloadGpsIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Payload GPS");
    this->timeoutDuration_sec = 1;
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](Backend::Packet telemetry)
    {
        if(telemetry.packetType == GroundStation::Payload)
        {
            if(telemetry.data.payloadData->gpslock())
            {
                this->resetTimer();
            }
        }
    });
}