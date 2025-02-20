//
// Created by William Scheirey on 2/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GaugeDisplay.h" resolved

#include "gaugedisplay.h"
#include "ui_GaugeDisplay.h"
#include <QFontDatabase>

#include "Frontend/Widgets/GaugeWidget/AccelerationGauge.h"

GaugeDisplay::GaugeDisplay(QWidget *parent) :
        QWidget(parent), ui(new Ui::GaugeDisplay)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/Fonts/JockeyOne-Regular.ttf");
    QString jockeyOne = QFontDatabase::applicationFontFamilies(id).at(0);
    int fontSize = this->ui->GaugeLabel->font().pointSize();
    this->ui->GaugeLabel->setFont(QFont(jockeyOne, 50));
    this->ui->UnitLabel->setFont(QFont(jockeyOne, 30));
    this->ui->ValueLabel->setFont(QFont(jockeyOne, 70));

}

void GaugeDisplay::setGaugeWidget(QWidget *widget)
{
    ui->gridLayout->removeWidget(this->ui->ValueLabel);

    ui->gridLayout->addWidget(widget, 0, 0, -1, 1);

    ui->gridLayout->addWidget(this->ui->ValueLabel, 0, 0, -1, 1);
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

void GaugeDisplay::setUnits(const QString &unitLabel)
{
    ui->UnitLabel->setText(unitLabel);
}

GaugeDisplay::~GaugeDisplay()
{
    delete ui;
}
