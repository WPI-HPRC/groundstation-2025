//
// Created by William Scheirey on 12/26/24.
//

#ifndef GS_BACKEND_2024_2025_STATESUMMARYWIDGET_H
#define GS_BACKEND_2024_2025_STATESUMMARYWIDGET_H

#include <QWidget>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class StateSummaryWidget;
}
QT_END_NAMESPACE

class StateSummaryWidget : public QWidget
{
Q_OBJECT

public:
    static QList<QString> RocketStateNames;

    explicit StateSummaryWidget(QWidget *parent = nullptr);
    void setMaxValues(Backend::MaxValues values, int state, int flightTime);

    ~StateSummaryWidget() override;

private:
    Ui::StateSummaryWidget *ui;
};


#endif //GS_BACKEND_2024_2025_STATESUMMARYWIDGET_H
