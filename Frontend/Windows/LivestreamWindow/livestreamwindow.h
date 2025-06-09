//
// Created by William Scheirey on 1/27/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMWINDOW_H
#define GROUNDSTATION_2025_LIVESTREAMWINDOW_H

#include <QMainWindow>
#include "Frontend/Widgets/CameraWidget/CameraWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class LivestreamWindow;
}
QT_END_NAMESPACE

class LivestreamWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit LivestreamWindow(QWidget *parent = nullptr);
    ~LivestreamWindow() override;

private:
    Ui::LivestreamWindow *ui;
};


#endif //GROUNDSTATION_2025_LIVESTREAMWINDOW_H
