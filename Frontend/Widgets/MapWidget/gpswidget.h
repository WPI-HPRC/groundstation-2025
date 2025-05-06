//
// Created by William Scheirey on 1/5/25.
//

#ifndef GS_BACKEND_2024_2025_GPSWIDGET_H
#define GS_BACKEND_2024_2025_GPSWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GpsWidget;
}
QT_END_NAMESPACE

class GpsWidget : public QWidget
{
Q_OBJECT

public:
    explicit GpsWidget(QWidget *parent = nullptr);

    ~GpsWidget() override;

private:
    Ui::GpsWidget *ui;
};


#endif //GS_BACKEND_2024_2025_GPSWIDGET_H
