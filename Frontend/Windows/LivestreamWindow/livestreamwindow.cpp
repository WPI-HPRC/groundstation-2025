//
// Created by William Scheirey on 1/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamWindow.h" resolved

#include "livestreamwindow.h"
#include "ui_LivestreamWindow.h"


LivestreamWindow::LivestreamWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::LivestreamWindow)
{
    ui->setupUi(this);
}

LivestreamWindow::~LivestreamWindow()
{
    delete ui;
}
