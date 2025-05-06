//
// Created by William Scheirey on 9/28/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include "Backend/Backend.h"
#include "Backend/APRS/KissClient.h"
#include "Backend/APRS/DirewolfProcess.h"
#include "Frontend/Windows/GraphWindow/GraphWindow.h"


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

    connect(&Backend::getInstance().aprsHandler.direwolfProcess, &DirewolfProcess::direwolfOutput, ui->AprsDisplay, &AprsWidget::direwolfOutputAvailable);
    connect(&Backend::getInstance().aprsHandler.kissClient, &KissClient::output, ui->AprsDisplay, &AprsWidget::kissOutput);


    // From https://stackoverflow.com/questions/14288635/any-easy-way-to-store-dock-windows-layout-and-sizes-in-settings-with-qt
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

    GraphWindow *graphWindow = new GraphWindow(ui->GraphWindowContainer);
    graphWindow->setWindowFlags(Qt::Widget);
    
    QHBoxLayout *layout = new QHBoxLayout(ui->GraphWindowContainer);
    layout->addWidget(graphWindow);
    ui->GraphWindowContainer->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}
