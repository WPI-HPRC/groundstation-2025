//
// Created by William Scheirey on 2/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AprsWidget.h" resolved

#include "aprswidget.h"
#include "ui_AprsWidget.h"
#include "Backend/Backend.h"
#include <QCheckBox>

AprsWidget::AprsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::AprsWidget)
{
    ui->setupUi(this);

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
                                             "\nLatitude: %0.4fºE"
                                             "\nLongitude: %0.4fºN"
                                             "\n",
                                             output.callsign.toStdString().c_str(),
                                             output.ID,
                                             output.data.gpsLock ? "YES" : "NO",
                                             output.data.satellites,
                                             output.data.altitude,
                                             output.data.latitude,
                                             output.data.longitude
                                             ));
}

AprsWidget::~AprsWidget()
{
    delete ui;
}
