//
// Created by William Scheirey on 3/19/25.
//

#ifndef GROUNDSTATION_2025_REMOTECONTROLWINDOW_H
#define GROUNDSTATION_2025_REMOTECONTROLWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class RemoteControlWindow;
}
QT_END_NAMESPACE

class RemoteControlWindow : public QWidget
{
Q_OBJECT

public:
    explicit RemoteControlWindow(QWidget *parent = nullptr);

    ~RemoteControlWindow() override;

private:
    Ui::RemoteControlWindow *ui;

    bool videoIsActive = false;
    bool acksAreEnabled = true;

    uint64_t getDestinationAddress();
};


#endif //GROUNDSTATION_2025_REMOTECONTROLWINDOW_H
