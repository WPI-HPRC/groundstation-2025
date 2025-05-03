
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Frontend/Windows/CameraWindow/camerawindow.h"
#include "Frontend/Windows/GraphWindow//GraphWindow.h"

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

    /*
    CameraWindow cameraWindow;
    cameraWindow.setWindowIcon(icon);
    cameraWindow.showMaximized();
     */

    GraphWindow graphWindow;
    graphWindow.showNormal();
    graphWindow.update();
    graphWindow.setWindowTitle("Rocket Graphs");

    Backend &backend = Backend::getInstance();
    backend.start();


    int code = QApplication::exec();

    backend.aprsHandler.stop();
    backend.flushFiles();
    return code;
}
