//
// Created by clive on 10/24/2024.
//

#include "GraphWidget.h"
#include <QtCharts/QChart>
#include <QGraphicsLinearLayout>

GraphWidget::GraphWidget(const QString &title, int range, QGraphicsItem *parent)
    : QChart(parent)  {
    setTitle(title);
    windowRange = range;

    auto *axisX = new QValueAxis();
    axisX->setRange(0, range);
    axisX->setTickCount(2);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, 1);

    this->addAxis(axisX, Qt::AlignLeft);
    this->addAxis(axisY, Qt::AlignBottom);
//    this->setContentsMargins(0, 0, 0, 0);
//    this->setTheme(QChart::ChartThemeDark);
}

void GraphWidget::hideLegend()
{
    this->legend()->hide();
}

void GraphWidget::addSeriesCustom(const QString &name)
{
    auto* series = new QLineSeries();
    series->setName(name);
    dataSeries.push_back(series);
    series->append(0,0);
    addSeries(series);
}

QLineSeries *GraphWidget::getSeries(int x) {
    return dataSeries.at(x);
}

void GraphWidget::addToSeries(int x, qreal valx, qreal valy) {
    dataSeries.at(x)->append(valx, valy);
}

void GraphWidget::yScream(qreal valx) {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0) {
            l->append(valx, l->at(l->count() - 1).y());
        }
    }
}

void GraphWidget::yZeroFill(qreal valx) {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0) {
            l->append(valx, 0);
        }
    }
}

//TODO would be MUCH better if QLineSeries was using a list instead of a vector
void GraphWidget::removeTail(qreal now) {
    if(noScroll)
        return;
    if(now > windowRange) {
        qreal lowestXValueDisplayed = now-windowRange;
        for (QLineSeries *l: dataSeries) {
            for(int i = 0; i < l->count(); i++) {
                if (l->at(i).x() < lowestXValueDisplayed) {
                    l->remove(i);
                } else {
                    break;
                }
            }
        }
    }
}
//TODO do directly on addSeriesCustom or is every rescale okay
void GraphWidget::rescale() {
    if(dataSeries.empty())
        return;
    if(dataSeries.at(0)->count() == 0)
        return;

    qreal minX = dataSeries.at(0)->at(0).x();
    qreal maxX = dataSeries.at(0)->points().last().x();
    qreal minY = dataSeries.at(0)->at(0).y();
    qreal maxY = dataSeries.at(0)->at(0).y();

    for(QLineSeries *l : dataSeries){
        for (const QPointF &point : l->points()) {
            minY = std::min(minY, point.y());
            maxY = std::max(maxY, point.y());
        }
    }

    if(useMinValue)
    {
        if(maxY == 0 && minY < 0)
        {
            maxY = minValue;
        }
        else
        {
            minY = minValue;
        }
    }

    if(noScroll)
    {
        minX = 0;
    }
    this->axes().at(0)->setRange(minX, maxX);
    this->axes().at(1)->setRange(minY*1.1, maxY*1.1);

    createDefaultAxes();
}

void GraphWidget::setMinValue(float newMinValue)
{
    useMinValue = true;
    minValue = newMinValue;
}

void GraphWidget::dontScroll()
{
    noScroll = true;
}
