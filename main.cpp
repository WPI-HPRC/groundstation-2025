
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Frontend/Windows/LogWindow/logwindow.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"
#include "Frontend/Windows/GraphWindow/GraphWindow.h"

struct TestStruct
{
    uint64_t address;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("Main Window");

    GraphWindow graphWindow;
    graphWindow.showNormal();
    graphWindow.update();
    graphWindow.setWindowTitle("Graph View");

    Backend &backend = Backend::getInstance();
    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
