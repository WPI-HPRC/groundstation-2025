
#include <QApplication>
#include <QProcess>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "Backend/APRS/KissClient.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QProcess process(&a);
    process.setProgram("bash");

    QFile script(":/APRS/run_aprs.sh");
    if (!script.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;
    QString scriptContents = script.readAll();

    QFile direWolfConf(":/APRS/direwolf.conf");
    if (!direWolfConf.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;

    QString confContents = direWolfConf.readAll();
    scriptContents.replace("direwolf.conf", "/dev/stdin");

    qDebug() << scriptContents;

    process.setArguments(QStringList() << "-c" << scriptContents);
    process.start();
    process.write(confContents.toUtf8());
    process.closeWriteChannel();

    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        std::cout << process.readAllStandardOutput().toStdString();
    });

    QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
        qDebug() << "Error:" << process.readAllStandardError();
    });

    QTimer::singleShot(1000, [](){
        KissClient kissClient;
    });

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("Main Window");

    Backend &backend = Backend::getInstance();
    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
