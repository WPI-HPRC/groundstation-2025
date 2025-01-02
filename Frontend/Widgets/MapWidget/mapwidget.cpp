//
// Created by William Scheirey on 1/1/25.
//
// THANK YOU WEAVER GOLDMAN FOR MOST OF THE SOURCE CODE FOR THE MAP STUFF

// You may need to build the project (run Qt uic code generator) to get "ui_MapWidget.h" resolved

#include "mapwidget.h"
#include "ui_MapWidget.h"
#include <QVBoxLayout>
#include <QTimer>

MapWidget::MapWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::MapWidget)
{
    ui->setupUi(this);

    // Create a web engine view, and display an offline leaflet map webpage inside it
    mapView = new QWebEngineView(this);
    mapView->load(QUrl("qrc:/Maps/src/index.html"));
    mapWebChannel = new QWebChannel(mapView->page());
    mapView->page()->setWebChannel(mapWebChannel);

    // Create an interface for sending information to the leaflet map
    jsInterface = new JsInterface();
    mapWebChannel->registerObject(QString("qtLeaflet"), jsInterface);

    // Create a layout inside this widget to resize the webpage automatically
    auto *layout = new QVBoxLayout;
    layout->addWidget(mapView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    // Connect each instance of the widget to the payload update signal
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MapWidget::repaint);
    timer->start(2000);
}

MapWidget::~MapWidget()
{
    delete ui;
}
