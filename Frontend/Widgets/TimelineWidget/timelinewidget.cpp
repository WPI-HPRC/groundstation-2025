//
// Created by William Scheirey on 12/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimelineWidget.h" resolved

#include "timelinewidget.h"
#include "ui_TimelineWidget.h"

#define NUM_STATES 4

TimelineWidget::TimelineWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);

    for(int i = 0; i < NUM_STATES; i++)
    {
        auto *stateWidget = new StateSummaryWidget(this);
        stateWidget->setEnabled(false);
        stateWidget->setTitle(Backend::getInstance().RocketStateNames.at(NUM_STATES - i));
        stateWidget->hideValues();
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
    if(lastPacket->state() > NUM_STATES)
    {
        return;
    }
   stateWidgets.at(NUM_STATES - lastPacket->state())->setMaxValues(maxValues, lastPacket->state());
   stateWidgets.at(NUM_STATES - lastPacket->state())->setEnabled(true);
   stateWidgets.at(NUM_STATES - lastPacket->state())->showValues();
}
