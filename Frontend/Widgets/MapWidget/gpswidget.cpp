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
/*
    connect(ui->albans, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("albans");
    });
    connect(ui->spaceport, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("spaceport");
    });
    connect(ui->wpi, &QRadioButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->setMapName("wpi");
    });
    connect(ui->AutoChooseMap, &QCheckBox::toggled, this, [this](bool checked)
    {
        if(checked)
        {
            this->ui->MapSelectionContainer->setEnabled(false);
            this->ui->TheMapWidget->jsInterface->mapHasBeenChosen = false;
            this->ui->TheMapWidget->jsInterface->chooseMap();
        }
        else
        {
            this->ui->MapSelectionContainer->setEnabled(true);
            this->ui->TheMapWidget->jsInterface->mapHasBeenChosen = true;
            QString mapName = "";
            if(!this->ui->MapSelectionGroup->checkedButton())
            {
                this->ui->MapSelectionGroup->buttons().at(0)->setChecked(true);
            }
            mapName = this->ui->MapSelectionGroup->checkedButton()->objectName();

            this->ui->TheMapWidget->jsInterface->setMapName(mapName);
        }
    });
    connect(ui->TheMapWidget->jsInterface, &JsInterface::mapWasChosen, this, [this](const QString &mapName)
    {
       auto *button = ui->MapSelectionContainer->findChild<QRadioButton *>(mapName);
       if(button)
       {
           button->setChecked(true);
       }
    });
    connect(ui->ClearButton, &QPushButton::released, this, [this]()
    {
        this->ui->TheMapWidget->jsInterface->reset();
    });
*/
}

GpsWidget::~GpsWidget()
{
    delete ui;
}
