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

    this->addAxis(axisX, Qt::AlignBottom);
    this->addAxis(axisY, Qt::AlignLeft);


    blockSignals(true);

    setAnimationOptions(QChart::NoAnimation);
}

void GraphWidget::hideLegend()
{
    this->legend()->hide();
}

void GraphWidget::addSeriesCustom(const QString &name)
{
    auto* series = new QLineSeries();
    series->setName(name);
    dataSeries.push_back(SeriesData{series});
    series->append(0, 0);
    addSeries(series);

    series->attachAxis(this->axes(Qt::Horizontal).first());
    series->attachAxis(this->axes(Qt::Vertical).first());
}


QLineSeries* GraphWidget::getSeries(int x) {
    if (x < 0 || x >= dataSeries.size()) return nullptr;
    return dataSeries[x].series;
}

void GraphWidget::addToSeries(int x, qreal valx, qreal valy) {
    if (x < 0 || x >= dataSeries.size()) return;
    dataSeries[x].buffer.append(QPointF(valx, valy));
}

void GraphWidget::yScream(qreal valx) {
    for (auto& data : dataSeries) {
        if (!data.buffer.isEmpty()) {
            qreal lastY = data.buffer.last().y();
            data.buffer.append(QPointF(valx, lastY));
        } else {
            data.buffer.append(QPointF(valx, 0)); // If no history, default to 0
        }
    }
    flushBuffersToSeries();
}


void GraphWidget::yZeroFill(qreal valx) {
    for (auto& data : dataSeries) {
        data.buffer.append(QPointF(valx, 0));
    }
    flushBuffersToSeries();
}

//TODO would be MUCH better if QLineSeries was using a list instead of a vector
void GraphWidget::removeTail(qreal now) {
    if (noScroll) return;

    if (now > windowRange) {
        qreal cutoff = now - windowRange;

        for (auto& data : dataSeries) {
            // Find the first point >= cutoff
            int i = 0;
            while (i < data.buffer.size() && data.buffer[i].x() < cutoff) {
                ++i;
            }

            if (i > 0) {
                data.buffer = data.buffer.mid(i);  // Drop the old points
            }
        }
    }

    flushBuffersToSeries();  // Flush the changes
}


void GraphWidget::flushBuffersToSeries() {
    for (auto& data : dataSeries) {
        if (!data.buffer.isEmpty()) {
            data.series->replace(data.buffer);
        }
    }
}

//TODO do directly on addSeriesCustom or is every rescale okay
void GraphWidget::rescale() {
    if (dataSeries.empty()) return;
    if (dataSeries[0].buffer.isEmpty()) return;

    qreal minX = dataSeries[0].buffer.first().x();
    qreal maxX = dataSeries[0].buffer.last().x();
    qreal minY = dataSeries[0].buffer.first().y();
    qreal maxY = minY;

    for (const auto& data : dataSeries) {
        for (const QPointF& point : data.buffer) {
            minY = std::min(minY, point.y());
            maxY = std::max(maxY, point.y());
        }
    }

    if (useMinValue) {
        if (maxY == 0 && minY < 0) {
            maxY = minValue;
        } else {
            minY = minValue;
        }
    }

    if (noScroll) {
        minX = 0;
    }

    this->axes().at(0)->setRange(minX, maxX);
    this->axes().at(1)->setRange(minY * 1.1, maxY * 1.1);
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
