//
// Created by William Scheirey on 9/28/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include "Backend/Backend.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->EnglishUnitConversion, &QCheckBox::checkStateChanged, this, [](Qt::CheckState state)
    {
        if (state == Qt::Checked)
        {
            Backend::getInstance().convertToEnglish = true;
        }
        else
        {
            Backend::getInstance().convertToEnglish = false;
        }
        Backend::getInstance().forceMaxValuesUpdate();
    });

    connect(ui->GeeConversion, &QCheckBox::checkStateChanged, this, [](Qt::CheckState state)
    {
        if (state == Qt::Checked)
        {
            Backend::getInstance().convertFromGees = true;
        }
        else
        {
            Backend::getInstance().convertFromGees = false;
        }
        Backend::getInstance().forceMaxValuesUpdate();
    });

    connect(ui->SaveLayoutButton, &QPushButton::released, this, [this]()
    {
        QSettings settings;
        settings.setValue("DOCK_LOCATIONS",this->saveState(1));
    });

    QSettings settings;

    if(!settings.value("DOCK_LOCATIONS").isNull())
    {
        this->restoreState(settings.value("DOCK_LOCATIONS").toByteArray(),1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
