//
// Created by William Scheirey on 1/27/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamWindow.h" resolved

#include "livestreamwindow.h"
#include "ui_LivestreamWindow.h"

#include "Frontend/Images/Placeholder4x3Image.h"
#include "Frontend/Widgets/CameraWidget/CameraWidget.h"

LivestreamWindow::LivestreamWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::LivestreamWindow)
{
    ui->setupUi(this);

//    auto *w = new CameraWidget();

//    auto *mapWidget = new Placeholder4x3Image();

//    QGridLayout *layout = ui->gridLayout;
//    cameraWidget->viewfinder->setLayout(layout);
//    cameraWidget->captureSession.setVideoOutput(this->centralWidget());
//    ui->gridLayout->addWidget(w, 0, 0, -1, -1);
//    w->lower();
//    cameraWidget->viewfinder->lower();

//    cameraWidget->lower();
//    setCentralWidget(cameraWidget->viewfinder);
}

LivestreamWindow::~LivestreamWindow()
{
    delete ui;
}
