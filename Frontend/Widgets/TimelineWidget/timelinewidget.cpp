//
// Created by William Scheirey on 12/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimelineWidget.h" resolved

#include "timelinewidget.h"
#include "ui_TimelineWidget.h"

#define NUM_STATES 6

TimelineWidget::TimelineWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);

    for(int i = 0; i < NUM_STATES; i++)
    {
        auto *stateWidget = new StateSummaryWidget(this);
        stateWidget->setEnabled(i == NUM_STATES-1);
        stateWidget->setTitle(Backend::getInstance().RocketStateNames.at(NUM_STATES - i - 1));
        stateWidget->hideValues();
        ui->gridLayout_2->addWidget(stateWidget, i+1, 0);
        stateWidgets.append(stateWidget);

        if(i == NUM_STATES - 1)
        {
            stateWidget->setPreLaunch();
        }

    }

    connect(&Backend::getInstance(), &Backend::rocketStateChanged, this, &TimelineWidget::rocketStateChanged);
}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}

void TimelineWidget::rocketStateChanged(const Backend::MaxValues &maxValues, int lastState, int newState)
{
    // Abort State is always the last one
    if(newState >= NUM_STATES)
    {
        return;
    }

    for(int i = NUM_STATES-1; i > newState; i--)
    {
        stateWidgets.at(NUM_STATES - i)->hideValues();
        stateWidgets.at(NUM_STATES - i)->setEnabled(false);
    }

    if(newState > 0)
    {
        stateWidgets.at(NUM_STATES - lastState - 1)->setMaxValues(maxValues, lastState);
        stateWidgets.at(NUM_STATES - lastState - 1)->showValues();
        stateWidgets.at(NUM_STATES - newState - 1)->setStart(maxValues);
    }
    stateWidgets.at(NUM_STATES - newState - 1)->setEnabled(true);
}
