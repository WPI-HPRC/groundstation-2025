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

void TimelineWidget::rocketStateChanged(const Backend::MaxValues &maxValues, int lastState, int newState)
{
    qDebug() << "Reached" << newState;
    if(newState > NUM_STATES)
    {
        return;
    }

    if(newState == 0)
    {
        for (StateSummaryWidget *widget : stateWidgets)
        {
            widget->hideValues();
            widget->setEnabled(false);
        }
        return;
    }

    for(int i = NUM_STATES; i > newState; i--)
    {
        stateWidgets.at(NUM_STATES - i)->hideValues();
        stateWidgets.at(NUM_STATES - i)->setEnabled(false);
    }
   stateWidgets.at(NUM_STATES - lastState-1)->setMaxValues(maxValues, newState);
   stateWidgets.at(NUM_STATES - lastState-1)->setEnabled(true);
   stateWidgets.at(NUM_STATES - lastState-1)->showValues();
}
