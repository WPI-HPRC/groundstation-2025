//
// Created by William Scheirey on 2/20/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamTimelineDisplay.h" resolved

#include "livestreamtimelinedisplay.h"
#include "ui_livestreamtimelinedisplay.h"
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


    connect(&Backend::getInstance(), &Backend::telemetryAvailable, [this](const HPRC::Telemetry& telemetry)
    {
        if(!telemetry.has_rocketpacket())
            return;
        const HPRC::RocketTelemetryPacket& data = telemetry.rocketpacket();

        this->ui->Timeline->updateValue(Utility::UnitConversion::meters2feet(data.altitude()));
        this->ui->AltitudeLabel->setText(QString::asprintf("%d ft", (int)(Utility::UnitConversion::meters2feet(data.altitude()))));
        this->ui->StateLabel->setText(Backend::getInstance().RocketStateNames.at(data.state()));
        repaint();
    });
}

LivestreamTimelineDisplay::~LivestreamTimelineDisplay()
{
    delete ui;
}
