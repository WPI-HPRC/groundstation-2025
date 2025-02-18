//
// Created by William Scheirey on 2/18/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CameraWindow.h" resolved

#include "camerawindow.h"
#include "ui_CameraWindow.h"

#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimediaWidgets/QVideoWidget>

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

CameraWindow::CameraWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::CameraWindow)
{
    ui->setupUi(this);

    if(checkCameraAvailability())
    {
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for (const QCameraDevice &cameraDevice : cameras) {
            if(cameraDevice.isDefault())
            {
                QMediaCaptureSession captureSession;
                auto *camera = new QCamera(cameraDevice);
                captureSession.setCamera(camera);
                auto *viewfinder = new QVideoWidget();
                captureSession.setVideoOutput(viewfinder);
                viewfinder->show();

                camera->start(); // to start the camera
            }
            qDebug() << cameraDevice.isDefault();
        }
    }
}

CameraWindow::~CameraWindow()
{
    delete ui;
}
