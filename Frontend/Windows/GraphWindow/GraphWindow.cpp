//Clive

#include "GraphWindow.h"
#include "ui_GraphWindow.h"


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

    for(GraphWidget* widget : allGraphs) {
        widget->removeTail(seconds);
        widget->rescale();
    }
    seconds += (1.0 / samplerate);
    telemflag = false;
}

void GraphWindow::telemetryAvailable(Backend::Telemetry telemetry)
{

    acceleration->addToSeries(0,seconds, telemetry.data.rocketData->accelx());
    acceleration->addToSeries(1, seconds, telemetry.data.rocketData->accely());
    acceleration->addToSeries(2, seconds, telemetry.data.rocketData->accelz());

    position->addToSeries(0,seconds, telemetry.data.rocketData->posx());
    position->addToSeries(1, seconds, telemetry.data.rocketData->posy());
    position->addToSeries(2, seconds, telemetry.data.rocketData->posz());

    gyro->addToSeries(0,seconds, telemetry.data.rocketData->gyrox());
    gyro->addToSeries(1, seconds, telemetry.data.rocketData->gyroy());
    gyro->addToSeries(2, seconds, telemetry.data.rocketData->gyroz());

    velocity->addToSeries(0,seconds, telemetry.data.rocketData->velx());
    velocity->addToSeries(1, seconds, telemetry.data.rocketData->vely());
    velocity->addToSeries(2, seconds, telemetry.data.rocketData->velz());

    telemflag = true;
}
//TODO just pass through the pointer to the seconds variable
GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GraphWindow)
{
    telemflag = false;
    seconds = 0;
    samplerate = 20;
    resolution = 10;
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(1000 / samplerate);

    acceleration = new GraphWidget("Acceleration", QBrush(QColor(30, 30, 19)), resolution);
    acceleration->addSeriesCustom("x", QColor(245, 60, 25));
    acceleration->addSeriesCustom("y", QColor(24, 245, 47));
    acceleration->addSeriesCustom("z",QColor(25, 60, 255));
    ui->GraphA->setChart(acceleration);
    allGraphs.push_back(acceleration);

    position = new GraphWidget("Position", QBrush(QColor(30, 30, 19)), resolution);
    position->addSeriesCustom("x", QColor(245, 60, 25));
    position->addSeriesCustom("y", QColor(24, 245, 47));
    position->addSeriesCustom("z",QColor(25, 60, 255));
    ui->GraphA_2->setChart(position);
    allGraphs.push_back(position);

    //TODO testing of auto "random" color, decide actual gyro colors later, so far looks good tho
    gyro = new GraphWidget("Gyro", QBrush(QColor(30, 30, 19)), resolution);
    gyro->addSeriesCustom( "x");
    gyro->addSeriesCustom( "y");
    gyro->addSeriesCustom( "z");
    ui->GraphA_3->setChart(gyro);

    velocity = new GraphWidget("Velocity", QBrush(QColor(30, 30, 19)), 8);
    velocity->addSeriesCustom( "x", QColor(245, 160, 125));
    velocity->addSeriesCustom("y", QColor(124, 245, 147));
    velocity->addSeriesCustom("z", QColor(125, 160, 255));
    ui->GraphA_4->setChart(velocity);
    allGraphs.push_back(velocity);


    connect(timer, &QTimer::timeout, this, &GraphWindow::scroll);
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &GraphWindow::telemetryAvailable);
    scroll();
}


GraphWindow::~GraphWindow()
{
    delete ui;
}
