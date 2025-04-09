//
// Created by William Scheirey on 1/1/25.
//

#ifndef GS_BACKEND_2024_2025_MAPWIDGET_H
#define GS_BACKEND_2024_2025_MAPWIDGET_H

#include <QWidget>
#include <QWebChannel>
#include <QtWebEngineWidgets/QWebEngineView>
#include "Frontend/Widgets/MapWidget/JSInterface.h"
#include "Backend/Backend.h"

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

public slots:
    void telemetryAvailable(Backend::Packet telemetry);

private:
    Ui::MapWidget *ui;
};


#endif //GS_BACKEND_2024_2025_MAPWIDGET_H
