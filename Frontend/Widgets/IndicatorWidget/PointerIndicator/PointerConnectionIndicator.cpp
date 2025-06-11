//
// Created by William Scheirey on 6/10/25.
//

#include "PointerConnectionIndicator.h"
#include "Tracker/Pointer.h"
PointerConnectionIndicator::PointerConnectionIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Pointer");
    connect(&Pointer::getInstance(), &Pointer::dataRead, this, [this](const QString _)
    {
            this->resetTimer();
    });
}