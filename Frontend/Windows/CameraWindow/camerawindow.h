//
// Created by William Scheirey on 2/18/25.
//

#ifndef GROUNDSTATION_2025_CAMERAWINDOW_H
#define GROUNDSTATION_2025_CAMERAWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class CameraWindow;
}
QT_END_NAMESPACE

class CameraWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit CameraWindow(QWidget *parent = nullptr);

    ~CameraWindow() override;

private:
    Ui::CameraWindow *ui;
};


#endif //GROUNDSTATION_2025_CAMERAWINDOW_H
