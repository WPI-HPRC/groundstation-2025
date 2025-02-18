
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Frontend/Windows/CameraWindow/camerawindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("Main Window");

    CameraWindow cameraWindow;
    cameraWindow.showMaximized();

    Backend &backend = Backend::getInstance();
    backend.start();

    int code = QApplication::exec();

    backend.aprsHandler.stop();
    backend.flushFiles();
    return code;
}
