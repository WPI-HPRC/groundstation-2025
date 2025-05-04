//Clive

#include "GraphWindow.h"
#include "ui_GraphWindow.h"
#include <QGraphicsLinearLayout>
#include <QElapsedTimer>

#include "Frontend/Widgets/GraphWidget/ThreeAxisGraph.h"
#include "Frontend/Widgets/GraphWidget/OneAxisGraph.h"
#include "Frontend/Widgets/GraphWidget/FourAxisGraph.h"

void GraphWindow::scroll()
{
    if(!telemflag){
        for(GraphWidget* widget : allGraphs) {
            widget->yScream(seconds);
        }

        //TODO handle disconnect, right now it will continue to yScream which might be okay, tbd
        /*
        for(GraphWidget* widget : allGraphs) {
            widget->yZeroFill(seconds);
        }
        */
    }

    if(timerCount % 10 == 0)
    {
//        QElapsedTimer timerr;  // Elapsed time tracker
//        timerr.start();
        for (GraphWidget *widget: allGraphs)
        {
            widget->removeTail(seconds);
            widget->flushBuffersToSeries();
            widget->rescale();
        }
        ui->AltitudeGraph->viewport()->update();
        ui->AccelerationGraph->viewport()->update();
        ui->GyroGraph->viewport()->update();
        ui->MagGraph->viewport()->update();
        ui->QuatGraph->viewport()->update();
        ui->VelocityGraph->viewport()->update();
        ui->PositionGraph->viewport()->update();
        ui->AirbrakesGraph->viewport()->update();

//        qint64 elapsedTime = timerr.elapsed();  // Time in milliseconds
//        qDebug() << "Rendering took:" << elapsedTime << "ms";
    }
    seconds += (1.0 / samplerate);

    timerCount++;
//    telemflag = false;
}

void GraphWindow::telemetryAvailable(const HPRC::Telemetry& telemetry)
{
//    return;
    if(!telemetry.has_rocketpacket())
    {
        return;
    }

    acceleration->addToSeries(0,seconds, telemetry.rocketpacket().accelx());
    acceleration->addToSeries(1, seconds, telemetry.rocketpacket().accely());
    acceleration->addToSeries(2, seconds, telemetry.rocketpacket().accelz());

    gyro->addToSeries(0, seconds, telemetry.rocketpacket().gyrox());
    gyro->addToSeries(1, seconds, telemetry.rocketpacket().gyroy());
    gyro->addToSeries(2, seconds, telemetry.rocketpacket().gyroz());

    velocity->addToSeries(0,seconds, telemetry.rocketpacket().velx());
    velocity->addToSeries(1, seconds, telemetry.rocketpacket().vely());
    velocity->addToSeries(2, seconds, telemetry.rocketpacket().velz());

    mag->addToSeries(0,seconds, telemetry.rocketpacket().magx());
    mag->addToSeries(1, seconds, telemetry.rocketpacket().magy());
    mag->addToSeries(2, seconds, telemetry.rocketpacket().magz());

    position->addToSeries(0,seconds, telemetry.rocketpacket().posx());
    position->addToSeries(1, seconds, telemetry.rocketpacket().posy());
    position->addToSeries(2, seconds, telemetry.rocketpacket().posz());

    altitude->addToSeries(0, seconds, telemetry.rocketpacket().altitude());
    airbrakes->addToSeries(0, seconds, telemetry.rocketpacket().servoposition());

    quat->addToSeries(0, seconds, telemetry.rocketpacket().i());
    quat->addToSeries(1, seconds, telemetry.rocketpacket().j());
    quat->addToSeries(2, seconds, telemetry.rocketpacket().k());
    quat->addToSeries(3, seconds, telemetry.rocketpacket().w());

    telemflag = true;
}
//TODO just pass through the pointer to the seconds variable
GraphWindow::GraphWindow(QWidget *parent) :
        QMainWindow(), ui(new Ui::GraphWindow)
{
    telemflag = true;
    seconds = 0;
    samplerate = 20;
    resolution = 20;
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(1000 / samplerate);

    altitude = new OneAxisGraph("Altitude", resolution);
    ui->AltitudeGraph->setChart(altitude);
    allGraphs.push_back(altitude);
    altitude->setMinValue(0);
    altitude->dontScroll();
    ui->AltitudeGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->AltitudeGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->AltitudeGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->AltitudeGraph->setCacheMode(QGraphicsView::CacheBackground);

    acceleration = new ThreeAxisGraph("Acceleration", resolution);
    ui->AccelerationGraph->setChart(acceleration);
    allGraphs.push_back(acceleration);
    ui->AccelerationGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->AccelerationGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->AccelerationGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->AccelerationGraph->setCacheMode(QGraphicsView::CacheBackground);

    gyro = new ThreeAxisGraph("Gyro", resolution);
    ui->GyroGraph->setChart(gyro);
    allGraphs.push_back(gyro);
    ui->GyroGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->GyroGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->GyroGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->GyroGraph->setCacheMode(QGraphicsView::CacheBackground);

    mag = new ThreeAxisGraph("Mag", resolution);
    ui->MagGraph->setChart(mag);
    allGraphs.push_back(mag);
    ui->MagGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->MagGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->MagGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->MagGraph->setCacheMode(QGraphicsView::CacheBackground);

    quat = new FourAxisGraph("Quaternion", resolution);
    ui->QuatGraph->setChart(quat);
    allGraphs.push_back(quat);
    ui->QuatGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->QuatGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->QuatGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->QuatGraph->setCacheMode(QGraphicsView::CacheBackground);

    velocity = new ThreeAxisGraph("Velocity", resolution);
    ui->VelocityGraph->setChart(velocity);
    allGraphs.push_back(velocity);
    ui->VelocityGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->VelocityGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->VelocityGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->VelocityGraph->setCacheMode(QGraphicsView::CacheBackground);

    position = new ThreeAxisGraph("Position", resolution);
    ui->PositionGraph->setChart(position);
    allGraphs.push_back(position);
    ui->PositionGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->PositionGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->PositionGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->PositionGraph->setCacheMode(QGraphicsView::CacheBackground);

    airbrakes = new OneAxisGraph("Airbrakes", resolution);
    ui->AirbrakesGraph->setChart(airbrakes);
    allGraphs.push_back(airbrakes);
    airbrakes->setMinValue(0);
    ui->AirbrakesGraph->setRenderHint(QPainter::Antialiasing, false);
    ui->AirbrakesGraph->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    ui->AirbrakesGraph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->AirbrakesGraph->setCacheMode(QGraphicsView::CacheBackground);

    connect(timer, &QTimer::timeout, this, &GraphWindow::scroll);
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &GraphWindow::telemetryAvailable);
    scroll();
//    ui->Airbrakes->setTitleBarWidget(new QWidget(ui->Airbrakes));
    ui->centralwidget->hide();
//    Backend::getInstance().setGraphWindow(this);
}


GraphWindow::~GraphWindow()
{
    delete ui;
}
