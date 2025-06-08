//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_CAMERAWIDGET_H
#define GROUNDSTATION_2025_CAMERAWIDGET_H

#include <QWidget>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaRecorder>

#include <QResizeEvent>

class CameraWidget: public QWidget
{
public:
    explicit CameraWidget(QWidget *parent = nullptr);

    void startCamera(const QCameraDevice& cameraDevice);
    void startRecording();
    void stopRecording();
    void resizeEvent(QResizeEvent *event) override;

    QMediaCaptureSession captureSession;  // Move this to class scope
    QMediaRecorder *recorder;
    QCamera *camera;  // Store camera pointer here
    QVideoWidget *viewfinder;
};


#endif //GROUNDSTATION_2025_CAMERAWIDGET_H
