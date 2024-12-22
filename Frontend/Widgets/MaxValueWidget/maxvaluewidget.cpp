//
// Created by William Scheirey on 12/22/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MaxValueWidget.h" resolved

#include "maxvaluewidget.h"
#include "ui_MaxValueWidget.h"
#include <QLabel>

MaxValueWidget::MaxValueWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MaxValueWidget)
{
    ui->setupUi(this);
    ui->MaxValueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->MaxValueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->MaxValueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(&Backend::getInstance(), &Backend::newMaxRocketValues, this, &MaxValueWidget::newRocketValues);
    connect(&Backend::getInstance(), &Backend::newMaxPayloadValues, this, &MaxValueWidget::newPayloadValues);
}

MaxValueWidget::~MaxValueWidget()
{
    delete ui;
}

void MaxValueWidget::newRocketValues(Backend::MaxValues rocketValues)
{
    updateTable(0, 0, QString::number(rocketValues.maxAltitude));
    updateTable(1, 0, QString::number(rocketValues.maxAcceleration));
    updateTable(2, 0, QString::number(rocketValues.maxVelocity));
    updateTable(3, 0, QString::number(rocketValues.maxAngularVelocity));
    updateTable(4, 0, QString::number(rocketValues.maxTemperature));
    updateTable(5, 0, QString::number(rocketValues.minTemperature));
    updateTable(6, 0, QString::number(rocketValues.maxPressure));
    updateTable(7, 0, QString::number(rocketValues.minPressure));
    updateTable(8, 0, QString::number(rocketValues.maxRocketServoPosition));
}

void MaxValueWidget::newPayloadValues(Backend::MaxValues payloadValues)
{
    updateTable(0, 1, QString::number(payloadValues.maxAltitude));
    updateTable(1, 1, QString::number(payloadValues.maxAcceleration));
    updateTable(2, 1, QString::number(payloadValues.maxVelocity));
    updateTable(3, 1, QString::number(payloadValues.maxAngularVelocity));
    updateTable(4, 1, QString::number(payloadValues.maxTemperature));
    updateTable(5, 1, QString::number(payloadValues.minTemperature));
    updateTable(6, 1, QString::number(payloadValues.maxPressure));
    updateTable(7, 1, QString::number(payloadValues.minPressure));
    updateTable(8, 1, "-");
}

void MaxValueWidget::updateTable(int row, int column, const QString &value)
{
    QTableWidget *widget = ui->MaxValueTable;
    auto *cell = (QLabel *)widget->cellWidget(row, column);
    if(!cell)
    {
        widget->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setText(value);
    }
}
