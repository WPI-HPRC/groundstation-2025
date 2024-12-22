//
// Created by William Scheirey on 12/22/24.
//

#ifndef GS_BACKEND_2024_2025_MAXVALUEWIDGET_H
#define GS_BACKEND_2024_2025_MAXVALUEWIDGET_H

#include <QWidget>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MaxValueWidget;
}
QT_END_NAMESPACE

class MaxValueWidget : public QWidget
{
Q_OBJECT

public:
    explicit MaxValueWidget(QWidget *parent = nullptr);

    ~MaxValueWidget() override;

public slots:
    void newRocketValues(Backend::MaxValues rocketValues);
    void newPayloadValues(Backend::MaxValues payloadValues);

private:
    Ui::MaxValueWidget *ui;
    void updateTable(int row, int column, const QString &value);


};


#endif //GS_BACKEND_2024_2025_MAXVALUEWIDGET_H
