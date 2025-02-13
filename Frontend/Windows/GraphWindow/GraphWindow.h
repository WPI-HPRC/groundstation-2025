#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QList>
#include <QValueAxis>
#include <QTimer>

#include "Frontend/Widgets/GraphWidget/GraphWidget.h"
#include "Backend/Backend.h"
namespace Ui {
    class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    static GraphWindow &getInstance()
    {
        static GraphWindow instance;
        return instance;
    }
    explicit GraphWindow(QWidget *parent = nullptr);
    //TODO enable_if?
    template <class T>
    GraphWidget* makeGraphSimple(std::initializer_list<T> list) {
        GraphWidget* graph = new GraphWidget("Velocity", 8, nullptr);
        for( auto elem : list) {
            graph->addSeriesCustom(elem);
        }
        //allGraphs.push_back();
        return graph;
    }
    qreal seconds;
    ~GraphWindow();

private:
    Ui::GraphWindow *ui;
    int resolution;
    bool telemflag;
    bool etmflag;
    bool disconnect;
    qreal samplerate;
    QTimer* timer;
    GraphWidget* acceleration;
    GraphWidget* quat;
    GraphWidget* gyro;
    GraphWidget* velocity;
    GraphWidget *mag;
    GraphWidget *altitude;
    GraphWidget *position;
    GraphWidget *airbrakes;
    std::vector<GraphWidget*> allGraphs;

public slots:
    void scroll();
    void telemetryAvailable(Backend::Telemetry telemetry);

};


#endif // GRAPHWINDOW_H
