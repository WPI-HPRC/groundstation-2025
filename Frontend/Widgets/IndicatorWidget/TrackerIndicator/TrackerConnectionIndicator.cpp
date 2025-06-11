//
// Created by William Scheirey on 6/10/25.
//

#include "TrackerConnectionIndicator.h"
#include "Tracker/Tracker.h"
TrackerConnectionIndicator::TrackerConnectionIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Tracker");
    connect(&Tracker::getInstance(), &Tracker::dataRead, this, [this](const QString _)
    {
            this->resetTimer();
    });
}