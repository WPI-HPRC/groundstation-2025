//
// Created by clive on 10/24/2024.
//

#ifndef GS_BACKEND_2024_2025_GRAPHWIDGET_H
#define GS_BACKEND_2024_2025_GRAPHWIDGET_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QString>
#include <QMutex>

class GraphWidget : public QChart {

Q_OBJECT

public:
    explicit GraphWidget(const QString &title, int range, QGraphicsItem *parent);
    void addSeriesCustom(const QString &name);
    void rescale();
    void setMinValue(float minValue);
    QLineSeries* getSeries(int x);
    void addToSeries(int x, qreal valx, qreal valy);
    void yScream(qreal valx);
    void yZeroFill(qreal valx);
    void removeTail(qreal now);
    //TODO DESTROY
    //~CustomChart();

    void hideLegend();
    void dontScroll();

    void flushBuffersToSeries();

private:
    struct SeriesData {
        QLineSeries* series{};
        QVector<QPointF> buffer;
    };

    std::vector<SeriesData> dataSeries;

    int windowRange;
    float minValue;
    bool useMinValue = false;
    bool noScroll = false;
    qreal lastXUpdate;
    qreal lastYUpdate;
};
#endif //GS_BACKEND_2024_2025_GRAPHWIDGET_H
