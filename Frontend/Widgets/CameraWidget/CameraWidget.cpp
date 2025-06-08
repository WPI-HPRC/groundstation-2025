//
// Created by William Scheirey on 2/20/25.
//

#include "CameraWidget.h"
#include <QApplication>
#include <QVBoxLayout>


CameraWidget::CameraWidget(QWidget *parent): QWidget(parent)
{
    camera = nullptr;
    viewfinder = nullptr;
}


void CameraWidget::startCamera(const QCameraDevice& cameraDevice)
{
    camera = new QCamera(cameraDevice);  // Store camera in class member
    camera->start();  // Start the camera

    captureSession.setCamera(camera);
    viewfinder = new QVideoWidget();
    captureSession.setVideoOutput(viewfinder);

    QVBoxLayout layout(this);
    layout.addWidget(viewfinder);
    this->setLayout(&layout);

    viewfinder->resize(this->size());
}

void CameraWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(viewfinder)
    {
        viewfinder->resize(event->size());
    }
}