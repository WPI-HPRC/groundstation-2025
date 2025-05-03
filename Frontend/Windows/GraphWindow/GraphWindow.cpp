//Clive

#include "GraphWindow.h"
#include "ui_GraphWindow.h"

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

    for(GraphWidget* widget : allGraphs) {
        widget->removeTail(seconds);
        widget->rescale();
    }
    seconds += (1.0 / samplerate);
//    telemflag = false;
}

void GraphWindow::telemetryAvailable(const HPRC::Telemetry& telemetry)
{
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
    QMainWindow(parent), ui(new Ui::GraphWindow)
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

    acceleration = new ThreeAxisGraph("Acceleration", resolution);
    ui->AccelerationGraph->setChart(acceleration);
    allGraphs.push_back(acceleration);

    gyro = new ThreeAxisGraph("Gyro", resolution);
    ui->GyroGraph->setChart(gyro);
    allGraphs.push_back(gyro);

    mag = new ThreeAxisGraph("Mag", resolution);
    ui->MagGraph->setChart(mag);
    allGraphs.push_back(mag);

    quat = new FourAxisGraph("Quaternion", resolution);
    ui->QuatGraph->setChart(quat);
    allGraphs.push_back(quat);

    velocity = new ThreeAxisGraph("Velocity", resolution);
    ui->VelocityGraph->setChart(velocity);
    allGraphs.push_back(velocity);

    position = new ThreeAxisGraph("Position", resolution);
    ui->PositionGraph->setChart(position);
    allGraphs.push_back(position);

    airbrakes = new OneAxisGraph("Airbrakes", resolution);
    ui->AirbrakesGraph->setChart(airbrakes);
    allGraphs.push_back(airbrakes);
    airbrakes->setMinValue(0);

    connect(timer, &QTimer::timeout, this, &GraphWindow::scroll);
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &GraphWindow::telemetryAvailable);
    scroll();
//    ui->Airbrakes->setTitleBarWidget(new QWidget(ui->Airbrakes));
    ui->centralwidget->hide();
}


GraphWindow::~GraphWindow()
{
    delete ui;
}
