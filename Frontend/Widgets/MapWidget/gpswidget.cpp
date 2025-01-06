//
// Created by William Scheirey on 1/5/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GpsWidget.h" resolved

#include "gpswidget.h"
#include "ui_GpsWidget.h"

GpsWidget::GpsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::GpsWidget)
{
    ui->setupUi(this);

    connect(ui->AlbansMap, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("albans");
    });
    connect(ui->SpaceportMap, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("spaceport");
    });
    connect(ui->WpiMap, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("wpi");
    });
    connect(ui->ClearButton, &QPushButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->reset();
    });

}

GpsWidget::~GpsWidget()
{
    delete ui;
}
