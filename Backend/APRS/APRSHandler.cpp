//
// Created by William Scheirey on 2/8/25.
//

#include "APRSHandler.h"
#include <QMessageBox>
#include <QApplication>

APRSHandler::APRSHandler(QObject *parent): QObject(parent)
{
    connect(&direwolfProcess, &DirewolfProcess::direwolfOutput, this, &APRSHandler::direwolfOutput);
}

void APRSHandler::stop()
{
    direwolfProcess.stop();
}

void APRSHandler::start()
{
    direwolfProcess.start();
}

void APRSHandler::direwolfOutput(QString output)
{
    if (output.contains("Ready to accept KISS TCP client application"))
    {
        kissClient.start();
    }
    else if(output.contains("End of file on stdin.  Exiting."))
    {
        QMessageBox::critical(
                QApplication::activeWindow(),
                "Ground Station",
                "APRS Receiver not found!"
                );
    }
}