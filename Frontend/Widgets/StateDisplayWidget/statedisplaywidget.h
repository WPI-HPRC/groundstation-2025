//
// Created by William Scheirey on 12/24/24.
//

#ifndef GS_BACKEND_2024_2025_STATEDISPLAYWIDGET_H
#define GS_BACKEND_2024_2025_STATEDISPLAYWIDGET_H

#include <QWidget>
#include <QString>
#include <Backend/Backend.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class StateDisplayWidget;
}
QT_END_NAMESPACE

class StateDisplayWidget : public QWidget
{
Q_OBJECT

public:

    explicit StateDisplayWidget(QWidget *parent = nullptr);

    void setMaxValues(Backend::MaxValues values, int state, int flightTime);
    QMetaObject::Connection telemetryConnection;

    ~StateDisplayWidget() override;

public slots:
    void telemetryAvailable(const HPRC::Telemetry& telemetry);

private:
    Ui::StateDisplayWidget *ui;
};


#endif //GS_BACKEND_2024_2025_STATEDISPLAYWIDGET_H
