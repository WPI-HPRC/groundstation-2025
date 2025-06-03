
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Frontend/Windows/LivestreamWindow/livestreamwindow.h"

#include "Frontend/Windows/TrackerWindow/trackerwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Courier New");
//    QFont font("Minecraft");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    QIcon icon(":/Icons/logo.ico");

    MainWindow mainWindow;

    mainWindow.setWindowIcon(icon);
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("Main Window");
    mainWindow.update();

    TrackerWindow trackerWindow;
    trackerWindow.show();
    trackerWindow.setWindowTitle("Tracker");

    /*
    CameraWindow cameraWindow;
    cameraWindow.setWindowIcon(icon);
    cameraWindow.showMaximized();
     */
    LivestreamWindow livestreamWindow;
//    livestreamWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    livestreamWindow.showMaximized();
    livestreamWindow.setWindowTitle("Livestream");

    Backend &backend = Backend::getInstance();
    backend.start();


    int code = QApplication::exec();

    backend.aprsHandler.stop();
    backend.flushFiles();
    return code;
}
