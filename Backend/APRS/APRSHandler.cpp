//
// Created by William Scheirey on 2/8/25.
//

#include "APRSHandler.h"

APRSHandler::APRSHandler(QObject *parent): QObject(parent)
{
    direwolfProcess.start();
}