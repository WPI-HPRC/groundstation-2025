//
// Created by William Scheirey on 2/20/25.
//

#include "CameraWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QMediaFormat>

CameraWidget::CameraWidget(QWidget *parent): QWidget(parent)
{
    camera = nullptr;
    viewfinder = nullptr;
}

void CameraWidget::startRecording()
{
    if(!camera || !camera->isActive())
    {
        return;
    }
    recorder = new QMediaRecorder(camera);
    captureSession.setRecorder(recorder);

    QMediaFormat format;
    format.setVideoCodec(QMediaFormat::VideoCodec::H264);
    format.setAudioCodec(QMediaFormat::AudioCodec::MP3);
    recorder->setMediaFormat(format);
    recorder->setOutputLocation(QUrl("test.mp4"));
    recorder->record();
}

void CameraWidget::stopRecording()
{
    if (recorder)
    {
        recorder->stop();
    }
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