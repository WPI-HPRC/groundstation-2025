//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMTIMEWIDGET_H
#define GROUNDSTATION_2025_LIVESTREAMTIMEWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class LivestreamTimeWidget;
}
QT_END_NAMESPACE

class LivestreamTimeWidget : public QWidget
{
Q_OBJECT

public:
    explicit LivestreamTimeWidget(QWidget *parent = nullptr);

    ~LivestreamTimeWidget() override;

private:
    Ui::LivestreamTimeWidget *ui;
};


#endif //GROUNDSTATION_2025_LIVESTREAMTIMEWIDGET_H
