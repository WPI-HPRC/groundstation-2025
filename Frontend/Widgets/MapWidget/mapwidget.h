//
// Created by William Scheirey on 1/1/25.
//

#ifndef GS_BACKEND_2024_2025_MAPWIDGET_H
#define GS_BACKEND_2024_2025_MAPWIDGET_H

#include <QWidget>
#include <QWebChannel>
#include <QWebEngineView>
#include "Frontend/Widgets/MapWidget/JSInterface.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MapWidget;
}
QT_END_NAMESPACE

class MapWidget : public QWidget
{
Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);

    ~MapWidget() override;

    QWebEngineView *mapView;
    QWebChannel *mapWebChannel;
    JsInterface *jsInterface;

private:
    Ui::MapWidget *ui;
};


#endif //GS_BACKEND_2024_2025_MAPWIDGET_H
