//
// Created by William Scheirey on 12/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimelineWidget.h" resolved

#include "timelinewidget.h"
#include "ui_TimelineWidget.h"


TimelineWidget::TimelineWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);

    for(int i = 0; i < 4; i++)
    {
        auto *stateWidget = new StateSummaryWidget(this);
        stateWidget->setEnabled(false);
        ui->gridLayout->addWidget(stateWidget, i+1, 0);
        stateWidgets.append(stateWidget);
    }

    connect(&Backend::getInstance(), &Backend::rocketStateChanged, this, &TimelineWidget::rocketStateChanged);
}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}

void TimelineWidget::rocketStateChanged(const Backend::MaxValues &maxValues, const HPRC::RocketTelemetryPacket *lastPacket)
{
   stateWidgets.at(4 - lastPacket->state())->setMaxValues(maxValues, lastPacket->state(), 0);
   stateWidgets.at(4 - lastPacket->state())->setEnabled(true);
}
