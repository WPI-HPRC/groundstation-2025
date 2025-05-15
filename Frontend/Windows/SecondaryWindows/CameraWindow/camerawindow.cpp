//
// Created by William Scheirey on 2/18/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CameraWindow.h" resolved

#include "camerawindow.h"
#include "ui_CameraWindow.h"

#include <QPermission>

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void CameraWindow::init()
{
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission)) {
        case Qt::PermissionStatus::Undetermined:
            qDebug() << "Requesting permission";
            qApp->requestPermission(cameraPermission, this, &CameraWindow::init);
            return;
        case Qt::PermissionStatus::Denied:
            qWarning("Camera permission is not granted!");
            return;
        case Qt::PermissionStatus::Granted:
            qDebug() << "Permission Granted!";
            break;
    }
    startCamera();
}

void CameraWindow::startCamera()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras) {
        if (cameraDevice.isDefault()) {
            camera = new QCamera(cameraDevice, this);  // Store camera in class member
            camera->start();  // Start the camera

            captureSession.setCamera(camera);
            viewfinder = new QVideoWidget(this);
            captureSession.setVideoOutput(viewfinder);

            setCentralWidget(viewfinder);
        }
    }
}

CameraWindow::CameraWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::CameraWindow)
{
    ui->setupUi(this);

    if(checkCameraAvailability())
    {
        this->init();
    }
}

CameraWindow::~CameraWindow()
{
    delete ui;
}
