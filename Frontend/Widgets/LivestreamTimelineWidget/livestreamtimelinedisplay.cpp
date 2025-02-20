//
// Created by William Scheirey on 2/20/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamTimelineDisplay.h" resolved

#include "livestreamtimelinedisplay.h"
#include "ui_LivestreamTimelineDisplay.h"
#include "Backend/Backend.h"

#include <QFontDatabase>

LivestreamTimelineDisplay::LivestreamTimelineDisplay(QWidget *parent) :
        QWidget(parent), ui(new Ui::LivestreamTimelineDisplay)
{
    ui->setupUi(this);


    int id = QFontDatabase::addApplicationFont(":/Fonts/centurygothic.ttf");
    QString font = QFontDatabase::applicationFontFamilies(id).at(0);
    this->ui->StateLabel->setFont(QFont(font, 30));
    this->ui->AltitudeLabel->setFont(QFont(font, 50));
    this->ui->UnofficialLabel->setFont(QFont(font, 15));


    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType != GroundStation::Rocket)
            return;
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;

        this->ui->Timeline->updateValue(data->altitude()*3.28084);
        this->ui->AltitudeLabel->setText(QString::asprintf("%d ft", (int)(data->altitude()*3.28084)));
        this->ui->StateLabel->setText(Backend::getInstance().RocketStateNames.at(data->state()));
        repaint();
    });
}

LivestreamTimelineDisplay::~LivestreamTimelineDisplay()
{
    delete ui;
}
