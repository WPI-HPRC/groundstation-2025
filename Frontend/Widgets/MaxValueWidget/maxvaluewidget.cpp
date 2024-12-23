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
    ui->MaxValueTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(&Backend::getInstance(), &Backend::newMaxRocketValues, this, &MaxValueWidget::newRocketValues);
    connect(&Backend::getInstance(), &Backend::newMaxPayloadValues, this, &MaxValueWidget::newPayloadValues);

    connect(ui->ResetRocketButton, &QPushButton::released, &Backend::getInstance(), &Backend::resetMaxRocketValues);
    connect(ui->ResetPayloadButton, &QPushButton::released, &Backend::getInstance(), &Backend::resetMaxPayloadValues);
}

MaxValueWidget::~MaxValueWidget()
{
    delete ui;
}

void MaxValueWidget::updateMaxValues(Backend::MaxValues values, int tableColumn)
{
    if(Backend::getInstance().convertToEnglish)
    {
        using namespace Utility::UnitConversion;
        values.maxAltitude = meters2feet(values.maxAltitude);
        if(Backend::getInstance().convertFromGees)
        {
            values.maxAcceleration = gs2feet(values.maxAcceleration);
        }
        values.maxVelocity = meters2feet(values.maxVelocity);
        values.maxTemperature = cel2far(values.maxTemperature);
        values.minTemperature = cel2far(values.minTemperature);
        values.maxPressure = mbar2psi(values.maxPressure);
        values.minPressure = mbar2psi(values.minPressure);
    }
    else if(Backend::getInstance().convertFromGees)
    {
        values.maxAcceleration = Utility::UnitConversion::gs2meters(values.maxAcceleration);
    }

    updateTable(0, tableColumn, QString::number(values.maxAltitude));
    updateTable(1, tableColumn, QString::number(values.maxAcceleration));
    updateTable(2, tableColumn, QString::number(values.maxVelocity));
    updateTable(3, tableColumn, QString::number(values.maxAngularVelocity));
    updateTable(4, tableColumn, QString::number(values.maxTemperature));
    updateTable(5, tableColumn, QString::number(values.minTemperature));
    updateTable(6, tableColumn, QString::number(values.maxPressure));
    updateTable(7, tableColumn, QString::number(values.minPressure));
    updateTable(8, tableColumn, tableColumn == 0 ? QString::number(values.maxRocketServoPosition) : "-");
}

void MaxValueWidget::newRocketValues(Backend::MaxValues rocketValues)
{
    updateMaxValues(rocketValues, 0);
}

void MaxValueWidget::newPayloadValues(Backend::MaxValues payloadValues)
{
    updateMaxValues(payloadValues, 1);
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
