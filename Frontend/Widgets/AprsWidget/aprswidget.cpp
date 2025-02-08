//
// Created by William Scheirey on 2/8/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AprsWidget.h" resolved

#include "aprswidget.h"
#include "ui_AprsWidget.h"

AprsWidget::AprsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::AprsWidget)
{
    ui->setupUi(this);
}

void AprsWidget::direwolfOutputAvailable(QString output)
{
    ui->DirewolfOutput->append(output);
}

AprsWidget::~AprsWidget()
{
    delete ui;
}
