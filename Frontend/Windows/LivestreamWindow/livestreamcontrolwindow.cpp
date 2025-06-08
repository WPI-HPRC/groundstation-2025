//
// Created by William Scheirey on 6/3/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LivestreamControlWindow.h" resolved

#include "livestreamcontrolwindow.h"
#include "ui_LivestreamControlWindow.h"
#include "Frontend/Windows/LivestreamWindow/livestreamwindow.h"
#include <QPermission>
#include <QApplication>

bool LivestreamControlWindow::checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void LivestreamControlWindow::init()
{
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission)) {
        case Qt::PermissionStatus::Undetermined:
            qDebug() << "Requesting permission";
            qApp->requestPermission(cameraPermission, this, &LivestreamControlWindow::init);
            return;
        case Qt::PermissionStatus::Denied:
            qWarning("Camera permission is not granted!");
            return;
        case Qt::PermissionStatus::Granted:
            qDebug() << "Permission Granted!";
            break;
    }
    populateCameraList();
}

void LivestreamControlWindow::populateCameraList()
{
    ui->CameraDropdown->clear();
    cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras) {
        ui->CameraDropdown->addItem(cameraDevice.description());
    }
}

LivestreamControlWindow::LivestreamControlWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::LivestreamControlWindow)
{
    ui->setupUi(this);

    if(checkCameraAvailability())
    {
        this->init();
    }

    connect(ui->RefreshCamerasButton, &QPushButton::released, this, [this]()
    {
       populateCameraList();
    });

    connect(ui->ConnectToCameraButton, &QPushButton::released, this, [this]()
    {
        QCameraDevice camera = cameras.at(ui->CameraDropdown->currentIndex());
        
        for (QWidget* widget : QApplication::allWidgets()) {
            if (widget->objectName() == "LivestreamVideo") {
                ((CameraWidget *)widget)->startCamera(camera);
            }
        }
    });
}

LivestreamControlWindow::~LivestreamControlWindow()
{
    delete ui;
}
