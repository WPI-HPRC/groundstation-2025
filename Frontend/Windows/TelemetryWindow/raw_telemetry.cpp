//
// Created by Rafael on 28.09.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Raw_Telemetry.h" resolved

#include "raw_telemetry.h"
#include <QLabel>
#include <QTableWidget>
#include "ui_Raw_Telemetry.h"
#include <QSerialPort>
#include "Backend/Backend.h"

Raw_Telemetry::Raw_Telemetry(QWidget *parent) :
        QWidget(parent), ui(new Ui::Raw_Telemetry)
{
    ui->setupUi(this);
    connect (&Backend::getInstance(), &Backend::telemetryAvailable, this, &Raw_Telemetry::telemetryAvailable);
}
Raw_Telemetry::~Raw_Telemetry() {
    delete ui;
}

void Raw_Telemetry::telemetryAvailable(Backend::Telemetry telemetry){
    if(telemetry.packetType== GroundStation::Rocket)
    {
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        processRocketTelemetryData(*data);
    }
    else if(telemetry.packetType== GroundStation::Payload)
    {
        HPRC::PayloadTelemetryPacket *data = telemetry.data.payloadData;
        processPayloadTelemetryData(*data);
    }
    //resizing columns
    ui->Column_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Column_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Raw_Telemetry::processRocketTelemetryData(const HPRC::RocketTelemetryPacket &data)
{   //Rocket Column1 Update
    int decimalPlaces = Backend::getInstance().telemetryDecimalPlaces;
    updateTable1(0, 1, QString::number(data.accelx(), 'f', decimalPlaces));
    updateTable1(1,1, QString::number(data.accely(), 'f', decimalPlaces));
    updateTable1(2,1, QString::number(data.accelz(), 'f', decimalPlaces));
    updateTable1(3,1,QString::number(data.velx(), 'f', decimalPlaces));
    updateTable1(4,1,QString::number(data.vely(), 'f', decimalPlaces));
    updateTable1(5,1,QString::number(data.velz(), 'f', decimalPlaces));
    updateTable1(6,1,QString::number(data.posx(), 'f', decimalPlaces));
    updateTable1(7,1,QString::number(data.posx(), 'f', decimalPlaces));
    updateTable1(8,1,QString::number(data.posz(), 'f', decimalPlaces));
    updateTable1(9,1,QString::number(data.magx(), 'f', decimalPlaces));
    updateTable1(10,1,QString::number(data.magy(), 'f', decimalPlaces));
    updateTable1(11,1,QString::number(data.magz(), 'f', decimalPlaces));
    updateTable1(12,1,QString::number(data.gyrox(), 'f', decimalPlaces));
    updateTable1(13,1,QString::number(data.gyroy(), 'f', decimalPlaces));
    updateTable1(14,1,QString::number(data.gyroz(), 'f', decimalPlaces));

    //Rocket Column2 update
    updateTable2(0,1,QString::number(data.epochtime()));
    updateTable2(1,1,QString::number(data.timestamp()));
    updateTable2(2,1,QString::number(data.state()));
    //Rocket GPS Lock update
    if (data.gpslock()){
        updateTable2(3,1,"yes");
    }
    else {
        updateTable2(3,1,"no");
    }
    updateTable2(4,1,QString::number(data.satellites()));
    //...

    //Rocket Column3 update
    updateTable3(0,1, QString::number(data.i(), 'f', decimalPlaces));
    updateTable3(1,1, QString::number(data.j(), 'f', decimalPlaces));
    updateTable3(2,1, QString::number(data.k(), 'f', decimalPlaces));
    updateTable3(3,1, QString::number(data.w(), 'f', decimalPlaces));
    updateTable3(4,1,QString::number(data.gpslat(), 'f', decimalPlaces));
    updateTable3(5,1,QString::number(data.gpslong(), 'f', decimalPlaces));
    updateTable3(6,1,QString::number(data.gpsaltmsl(), 'f', decimalPlaces));
    updateTable3(7,1,QString::number(data.gpsaltagl(), 'f', decimalPlaces));
    updateTable3(8,1,QString::number(data.altitude(), 'f', decimalPlaces));
    updateTable3(9,1,QString::number(data.pressure(), 'f', decimalPlaces));
}

void Raw_Telemetry::processPayloadTelemetryData(const HPRC::PayloadTelemetryPacket &data)
{
    int decimalPlaces = Backend::getInstance().telemetryDecimalPlaces;
    //Payload Column1 Update
    updateTable1(0,2,QString::number(data.accelx(), 'f', decimalPlaces));
    updateTable1(1,2,QString::number(data.accely(), 'f', decimalPlaces));
    updateTable1(2,2,QString::number(data.accelz(), 'f', decimalPlaces));
    updateTable1(3,2,QString::number(data.velx(), 'f', decimalPlaces));
    updateTable1(4,2,QString::number(data.vely(), 'f', decimalPlaces));
    updateTable1(5,2,QString::number(data.velz(), 'f', decimalPlaces));
    updateTable1(6,2,QString::number(data.posx(), 'f', decimalPlaces));
    updateTable1(7,2,QString::number(data.posy(), 'f', decimalPlaces));
    updateTable1(8,2,QString::number(data.posz(), 'f', decimalPlaces));
    updateTable1(9,2,QString::number(data.magx(), 'f', decimalPlaces));
    updateTable1(10,2,QString::number(data.magy(), 'f', decimalPlaces));
    updateTable1(11,2,QString::number(data.magz(), 'f', decimalPlaces));
    updateTable1(12,2,QString::number(data.gyrox(), 'f', decimalPlaces));
    updateTable1(13,2,QString::number(data.gyroy(), 'f', decimalPlaces));
    updateTable1(14,2,QString::number(data.gyroz(), 'f', decimalPlaces));

    //Payload Column3 update
    updateTable3(0,2,QString::number(data.i(), 'f', decimalPlaces));
    updateTable3(1,2,QString::number(data.j(), 'f', decimalPlaces));
    updateTable3(2,2,QString::number(data.k(), 'f', decimalPlaces));
    updateTable3(3,2,QString::number(data.w(), 'f', decimalPlaces));
    updateTable3(4,2,QString::number(data.gpslat(), 'f', decimalPlaces));
    updateTable3(5,2,QString::number(data.gpslong(), 'f', decimalPlaces));
    updateTable3(6,2,QString::number(data.gpsaltmsl(), 'f', decimalPlaces));
    updateTable3(7,2,QString::number(data.gpsaltagl(), 'f', decimalPlaces));
    updateTable3(8,2,QString::number(data.altitude(), 'f', decimalPlaces));
    updateTable3(9,2,QString::number(data.pressure(), 'f', decimalPlaces));

    //Payload Column2 update
    updateTable2(0,2,QString::number(data.epochtime()));
    updateTable2(1,2,QString::number(data.timestamp()));
    updateTable2(2,2,QString::number(data.state()));
    //Payload GPS Lock update
    if (data.gpslock()){
        updateTable2(3,2,"yes");
    }
    else
    {
        updateTable2(3,2,"no");
    }
    updateTable2(4,2,QString::number(data.satellites()));
}

void Raw_Telemetry::updateTable1(int row, int column, const QString &value){ //function to update Column 1
    QTableWidget *widget = ui->Column_1;
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

void Raw_Telemetry::updateTable2(int row, int column, const QString &value){ //function to update Column 2
    QTableWidget *widget = ui->Column_2;
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
void Raw_Telemetry::updateTable3(int row, int column, const QString &value){ //function to update Column 3
    QTableWidget *widget = ui->Column_3;
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






