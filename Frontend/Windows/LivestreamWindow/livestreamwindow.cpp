//
// Created by William Scheirey on 1/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamWindow.h" resolved

#include "livestreamwindow.h"
#include "ui_LivestreamWindow.h"

#include "Frontend/Images/Placeholder4x3Image.h"

LivestreamWindow::LivestreamWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::LivestreamWindow)
{
    ui->setupUi(this);

    auto *image = new Placeholder4x3Image();

    /*
    cameraWidget = new CameraWidget();
    cameraWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    ui->gridLayout->addWidget(cameraWidget->viewfinder, 0, 0, -1, -1);
    cameraWidget->viewfinder->lower();
     */
}

LivestreamWindow::~LivestreamWindow()
{
    delete ui;
}
