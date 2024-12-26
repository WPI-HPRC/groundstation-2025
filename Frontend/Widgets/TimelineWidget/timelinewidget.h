//
// Created by William Scheirey on 12/26/24.
//

#ifndef GS_BACKEND_2024_2025_TIMELINEWIDGET_H
#define GS_BACKEND_2024_2025_TIMELINEWIDGET_H

#include <QWidget>
#include "Backend/Backend.h"
#include "Frontend/Widgets/TimelineWidget/statesummarywidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class TimelineWidget;
}
QT_END_NAMESPACE

class TimelineWidget : public QWidget
{
Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    QList<StateSummaryWidget *> stateWidgets;

    ~TimelineWidget() override;

public slots:
    void rocketStateChanged(const Backend::MaxValues &, const HPRC::RocketTelemetryPacket *);

private:
    Ui::TimelineWidget *ui;
};


#endif //GS_BACKEND_2024_2025_TIMELINEWIDGET_H
