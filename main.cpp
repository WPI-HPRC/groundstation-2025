
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Frontend/Windows/CameraWindow/camerawindow.h"

#include "Frontend/Windows/TrackerWindow/trackerwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Courier New");
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

    Backend &backend = Backend::getInstance();
    backend.start();


    int code = QApplication::exec();

    backend.aprsHandler.stop();
    backend.flushFiles();
    return code;
}
