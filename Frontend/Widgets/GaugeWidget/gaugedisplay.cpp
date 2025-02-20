//
// Created by William Scheirey on 2/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GaugeDisplay.h" resolved

#include "gaugedisplay.h"
#include "ui_GaugeDisplay.h"

#include "Frontend/Widgets/GaugeWidget/AccelerationGauge.h"

GaugeDisplay::GaugeDisplay(QWidget *parent) :
        QWidget(parent), ui(new Ui::GaugeDisplay)
{
    ui->setupUi(this);
}

void GaugeDisplay::setGaugeWidget(QWidget *widget)
{
    ui->gridLayout->removeWidget(this->ui->ValueLabel);

    ui->gridLayout->addWidget(widget, 0, 0, 3, 1);

    ui->gridLayout->addWidget(this->ui->ValueLabel, 0, 0, 2, 1);
}

void GaugeDisplay::updateNumber(const QString &numberString)
{
    ui->ValueLabel->setText(numberString);
    ui->ValueLabel->setStyleSheet("margin-top: 70px; color: white");
}

void GaugeDisplay::updateLabel(const QString &label)
{
    ui->GaugeLabel->setText(label);
}

GaugeDisplay::~GaugeDisplay()
{
    delete ui;
}
