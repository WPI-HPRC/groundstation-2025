//
// Created by William Scheirey on 2/20/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamTimeWidget.h" resolved

#include "livestreamtimewidget.h"
#include "ui_livestreamtimewidget.h"


LivestreamTimeWidget::LivestreamTimeWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::LivestreamTimeWidget)
{
    ui->setupUi(this);
}

LivestreamTimeWidget::~LivestreamTimeWidget()
{
    delete ui;
}
