//
// Created by William Scheirey on 6/3/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H
#define GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H

#include <QWidget>


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

private:
    Ui::LivestreamControlWindow *ui;
};


#endif //GROUNDSTATION_2025_LIVESTREAMCONTROLWINDOW_H
