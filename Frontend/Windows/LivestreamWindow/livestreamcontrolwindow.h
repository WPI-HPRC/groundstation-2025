//
// Created by William Scheirey on 6/3/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H
#define GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H

#include <QWidget>
#include "Frontend/Widgets/CameraWidget/CameraWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LivestreamControlWindow;
}
QT_END_NAMESPACE

class LivestreamControlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LivestreamControlWindow(QWidget *parent = nullptr);

    ~LivestreamControlWindow() override;
    bool cameraIsConnected = false;
    bool recording = false;

private:
    bool checkCameraAvailability();
    void init();
    void populateCameraList();
    QList<QCameraDevice> cameras;
    Ui::LivestreamControlWindow *ui;
};


#endif //GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H
