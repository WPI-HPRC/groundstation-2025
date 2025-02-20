//
// Created by William Scheirey on 2/20/25.
//

#include "CameraWidget.h"
#include <QPermission>
#include <QApplication>
#include <QVBoxLayout>

bool CameraWidget::checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

CameraWidget::CameraWidget(QWidget *parent): QWidget(parent)
{
    if(checkCameraAvailability())
    {
        this->init();
    }
}

void CameraWidget::init()
{
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission)) {
        case Qt::PermissionStatus::Undetermined:
            qDebug() << "Requesting permission";
            qApp->requestPermission(cameraPermission, this, &CameraWidget::init);
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

void CameraWidget::startCamera()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras) {
        if (cameraDevice.isDefault()) {
            camera = new QCamera(cameraDevice, this);  // Store camera in class member
            camera->start();  // Start the camera

            captureSession.setCamera(camera);
            viewfinder = new QVideoWidget(this);
            captureSession.setVideoOutput(viewfinder);

            QVBoxLayout layout(this);
            layout.addWidget(viewfinder);
            this->setLayout(&layout);
        }
    }
}

void CameraWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    viewfinder->resize(event->size());
    this->lower();
    viewfinder->lower();
}