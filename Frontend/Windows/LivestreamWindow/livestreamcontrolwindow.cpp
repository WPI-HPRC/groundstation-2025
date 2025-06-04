//
// Created by William Scheirey on 6/3/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamControlWindow.h" resolved

#include "livestreamcontrolwindow.h"
#include "ui_LivestreamControlWindow.h"


LivestreamControlWindow::LivestreamControlWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::LivestreamControlWindow)
{
    ui->setupUi(this);
}

LivestreamControlWindow::~LivestreamControlWindow()
{
    delete ui;
}
