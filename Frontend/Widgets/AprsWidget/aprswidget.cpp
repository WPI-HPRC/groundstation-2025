//
// Created by William Scheirey on 2/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AprsWidget.h" resolved

#include "aprswidget.h"
#include "ui_AprsWidget.h"
#include "Backend/Backend.h"
#include <QTableWidget>
#include <QCheckBox>

AprsWidget::AprsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::AprsWidget)
{
    ui->setupUi(this);
    ui->DataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DataTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DataTable->setStyleSheet("QTableWidget {background-color: transparent;}"
                                               "QHeaderView::section {background-color: transparent;}"
                                               "QHeaderView {background-color: transparent;}"
                                               "QTableCornerButton::section {background-color: transparent;}");

    connect(&Backend::getInstance().aprsHandler.direwolfProcess.process, &QProcess::started, this, [this]()
    {
        this->ui->StopButton->setEnabled(true);
        this->ui->StartButton->setEnabled(false);
    });
    connect(&Backend::getInstance().aprsHandler.direwolfProcess.process, &QProcess::finished, this, [this]()
    {
        this->ui->StopButton->setEnabled(false);
        this->ui->StartButton->setEnabled(true);
    });

    connect(ui->HideTextOutput, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState checkstate)
    {
        if(checkstate == Qt::Checked)
        {
            this->ui->TextOutput->hide();
            this->ui->VerticalSpacer->changeSize(-1, -1, QSizePolicy::Preferred, QSizePolicy::Expanding);
        }
        else
        {
            this->ui->TextOutput->show();
            this->ui->VerticalSpacer->changeSize(-1, -1, QSizePolicy::Ignored, QSizePolicy::Ignored);

            for(int i = 0; i < this->ui->gridLayout->rowCount(); i++)
            {
                this->ui->gridLayout->setRowStretch(i, 0);
            }
        }
    });

    connect(ui->StartButton, &QPushButton::pressed, this, [this]()
    {
        Backend::getInstance().aprsHandler.start();
        ui->DirewolfOutput->clear();
        ui->KissOutput->clear();
    });

    connect(ui->StopButton, &QPushButton::pressed, this, []()
    {
        Backend::getInstance().aprsHandler.stop();
    });
}

void AprsWidget::direwolfOutputAvailable(const QString& output)
{
    ui->DirewolfOutput->append(output);
}

void AprsWidget::kissOutput(const KissClient::AprsOutput& output)
{
    ui->KissOutput->append(QString::asprintf("---| %s-%d |---"
                                             "\nGPS Lock: %s"
                                             "\nSatellites: %d"
                                             "\nAltitude: %d ft"
                                             "\nLatitude: %0.5fºE"
                                             "\nLongitude: %0.5fºN"
                                             "\n",
                                             output.callsign.toStdString().c_str(),
                                             output.ID,
                                             output.data.gpsLock ? "YES" : "NO",
                                             output.data.satellites,
                                             output.data.altitude,
                                             output.data.latitude,
                                             output.data.longitude
                                             ));
    if(output.ID == 13)
    {
        updateTable(0, 1, output.data.gpsLock ? "YES" : "NO");
        updateTable(1, 1, QString::number(output.data.satellites));
        updateTable(2, 1, QString::number(output.data.altitude));
        updateTable(3, 1, QString::number(output.data.latitude, 'f', 5));
        updateTable(4, 1, QString::number(output.data.longitude, 'f', 5));

    }
}

AprsWidget::~AprsWidget()
{
    delete ui;
}

void AprsWidget::updateTable(int row, int column, const QString &value)
{
    QTableWidget *widget = ui->DataTable;
    auto *cell = (QLabel *)widget->cellWidget(row, column);
    if(!cell)
    {
        widget->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setContentsMargins(12, 0, 0, 0);
        cell->setText(value);
    }
}
