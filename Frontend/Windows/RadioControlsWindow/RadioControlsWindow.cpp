//
// Created by William Scheirey on 8/24/24.
//

#include "RadioControlsWindow.h"
#include <QLabel>
#include "ui_RadioControlsWindow.h"

#define FindChild(name) name = findChild<decltype(name)>(QStringLiteral(#name).replace(0, 1, QString(#name)[0].toUpper()))

uint64_t getAddressBigEndian(const uint8_t *packet, size_t *index_io)
{
    uint64_t address = 0;

    for (int i = 0; i < 8; i++)
    {
        address |= (uint64_t) packet[*index_io] << (8 * (7 - i));
        (*index_io)++;
    }

    return address;
}

uint64_t getAddressBigEndian(const uint8_t *packet)
{
    size_t _ = 0;

    return getAddressBigEndian(packet, &_);
}

QByteArray hexToBytes(const QString &hexString)
{
    QByteArray rawBytes;
    QString cleanHexString = hexString;
    cleanHexString.remove(QRegularExpression("\\s")); // Remove all spaces and newlines
    rawBytes = QByteArray::fromHex(cleanHexString.toLatin1());
    return rawBytes;
}

void RadioControlsWindow::linkTestFailed()
{
    /*
    this->linkTest_Button->setEnabled(true);
    this->linkTest_Button->setText("Run Link Test");
    */
    lastLinkTestFailed = true;
    this->linkTestButtonPressed();
    ui->LinkTest_Button->setEnabled(true);
    ui->LinkTest_Button->setText("STOP");
}

void RadioControlsWindow::linkTestDataAvailable(LinkTestResults results, int iterationsLeft)
{
    ui->LinkTestResults_NoiseFloor->setText(QString::asprintf("-%d dBm", results.noiseFloor));
    ui->LinkTestResults_NoiseFloor->setEnabled(true);

    ui->LinkTestResults_MaxRssi->setEnabled(true);
    ui->LinkTestResults_MaxRssi->setText(QString::asprintf("-%d dBm", results.maxRssi));

    ui->LinkTestResults_MinRssi->setEnabled(true);
    ui->LinkTestResults_MinRssi->setText(QString::asprintf("-%d dBm", results.minRssi));

    ui->LinkTestResults_AvgRssi->setEnabled(true);
    ui->LinkTestResults_AvgRssi->setText(QString::asprintf("-%d dBm", results.avgRssi));

    ui->LinkTestResults_Success->setEnabled(true);
    ui->LinkTestResults_Success->setText(QString::asprintf("%d", results.success));

    ui->LinkTestResults_Retries->setEnabled(true);
    ui->LinkTestResults_Retries->setText(QString::asprintf("%d", results.retries));

    ui->LinkTestResults_RR->setEnabled(true);
    ui->LinkTestResults_RR->setText(QString::asprintf("%d", results.RR));

    ui->LinkTestResults_TotalPackets->setEnabled(true);
    ui->LinkTestResults_TotalPackets->setText(QString::asprintf("%d", results.iterations + results.retries));

    ui->LinkTestResults_PercentSuccess->setEnabled(true);
    ui->LinkTestResults_PercentSuccess->setText(QString::asprintf("%d", (int)((float)results.success / ((float)results.iterations + (float)results.retries) * 100)));

    ui->LinkTest_Button->setEnabled(true);
    if(iterationsLeft == 0)
    {
        ui->LinkTest_Button->setText("Run Link Test");
    }
    else if(iterationsLeft > 0)
    {
        ui->LinkTest_Button->setText(QString::asprintf("STOP - %d left", iterationsLeft));
    }
    else
    {
        ui->LinkTest_Button->setText("STOP");
    }

    std::cout << "New link test data" << std::endl;
}

void RadioControlsWindow::linkTestButtonPressed()
{
    loopLinkTest = ui->LinkTest_Loop->isChecked();
    if(!lastLinkTestFailed)
    {
        if (ui->LinkTest_Button->text().startsWith("STOP"))
        {
            std::cout << "Stopping link test" << std::endl;
            Backend::getInstance().cancelLinkTest();
            ui->LinkTest_Button->setText("Run Link Test");
            loopLinkTest = false;
            return;
        }
    }

    int payloadSize = ui->LinkTest_PayloadSize->value();
    int iterations = ui->LinkTest_Iterations->value();
    int repeat = ui->LinkTest_Repeat->value();
    QByteArray bytes = hexToBytes(ui->LinkTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());

    ui->LinkTest_Button->setText("Running link test...");
    ui->LinkTest_Button->setEnabled(false);
    std::cout << "Last link test failed ? " << (lastLinkTestFailed ? "YES" : "NO") << std::endl;
    Backend::getInstance().runLinkTest(address, payloadSize, iterations, lastLinkTestFailed ? 0 : repeat, loopLinkTest);
    if(lastLinkTestFailed)
    {
        lastLinkTestFailed = false;
    }
}

void RadioControlsWindow::throughputTestButtonPressed()
{
    if(throughputTestIsRunning)
    {
        throughputTestIsRunning = false;
        Backend::getInstance().cancelThroughputTest();
        ui->ThroughputTest_Button->setText("Run throughput test");
        ui->ThroughputTest_ParamsContainer->setEnabled(true);
        return;
    }

    QString originatingModulePortName = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(originatingModulePortName == "")
    {
        ui->ThroughputTest_Button->setText("Choose a connected module!");
        QTimer::singleShot(1000, [this]()
        {
            this->ui->ThroughputTest_Button->setText("Run throughput test");
        });
        return;
    }

    QByteArray bytes = hexToBytes(ui->ThroughputTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());
    uint duration = ui->ThroughputTest_Duration->value();
    uint8_t transmitOptions = ui->ThroughputTest_TransmitOptions->value();

    if(!ui->ThroughputTest_RangeScanning->isChecked())
    {
        uint8_t payloadSize = ui->ThroughputTest_PayloadSize->value();
        uint packetRate = ui->ThroughputTest_PacketRate->value();

        Backend::getInstance().runThroughputTest(originatingModulePortName, address, payloadSize, packetRate, duration,
                                                 transmitOptions);
    }
    else
    {
        QList<QList<int>> testParams;
        // Yuck
        for(
                int payloadSize = ui->ThroughputTest_MinPayloadSize->value();
                payloadSize <= ui->ThroughputTest_MaxPayloadSize->value() + ui->ThroughputTest_PayloadSizeStep->value();
                payloadSize += ui->ThroughputTest_PayloadSizeStep->value())
        {
            for(
                    int packetRate = ui->ThroughputTest_MinPacketRate->value();
                    packetRate <= ui->ThroughputTest_MaxPacketRate->value() + ui->ThroughputTest_PacketRateStep->value();
                    packetRate += ui->ThroughputTest_PacketRateStep->value())
            {
                testParams.append({payloadSize, packetRate});
            }
        }
        std::cout << testParams.count() << " Tests" << std::endl;
        std::cout << testParams.count() * duration << " seconds of tests. (" << testParams.count() * duration/60 << " minutes)" << std::endl;

        Backend::getInstance().runThroughputTestsWithRange(originatingModulePortName, address, testParams, duration, transmitOptions);
    }
    throughputTestIsRunning = true;
    ui->ThroughputTest_Button->setText("STOP");
    ui->ThroughputTest_ParamsContainer->setEnabled(false);
}

void RadioControlsWindow::throughputTestDataAvailable(float percentSuccess, uint numSuccess, float throughput)
{
    if(Backend::getInstance().throughputTestIndex >= 0 && throughputTestIsRunning)
    {
        ui->ThroughputTest_Button->setText(QString::asprintf("STOP - %lld left",
                                                             Backend::getInstance().throughputTests.count() - Backend::getInstance().throughputTestIndex));
    }
    else
    {
        ui->ThroughputTest_Button->setText("Run throughput test");
        throughputTestIsRunning = false;
        ui->ThroughputTest_ParamsContainer->setEnabled(true);
    }

    ui->ThroughputTestResults_PercentSuccess->setEnabled(true);
    ui->ThroughputTestResults_PercentSuccess->setText(QString::asprintf("%0.1f", percentSuccess));

    ui->ThroughputTestResults_NumSuccess->setEnabled(true);
    ui->ThroughputTestResults_NumSuccess->setText(QString::asprintf("%d", numSuccess));

    ui->ThroughputTestResults_Throughput->setEnabled(true);
    ui->ThroughputTestResults_Throughput->setText(QString::asprintf("%f kbps", throughput));
}

void RadioControlsWindow::rangeScanningBoxClicked(bool checked)
{
    if(checked)
    {
        ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(true);
    }
    else
    {
        ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(false);
    }
}

RadioControlsWindow::RadioControlsWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RadioControlsWindow)
{
    ui->setupUi(this);

    ui->LinkTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    ui->LinkTestResults_NoiseFloor->setEnabled(false);
    ui->LinkTestResults_MaxRssi->setEnabled(false);
    ui->LinkTestResults_MinRssi->setEnabled(false);
    ui->LinkTestResults_AvgRssi->setEnabled(false);
    ui->LinkTestResults_Success->setEnabled(false);
    ui->LinkTestResults_Retries->setEnabled(false);
    ui->LinkTestResults_RR->setEnabled(false);

    ui->LinkTestResults_TotalPackets->setEnabled(false);
    ui->LinkTestResults_PercentSuccess->setEnabled(false);

    ui->ThroughputTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    ui->ThroughputTestResults_NumSuccess->setEnabled(false);
    ui->ThroughputTestResults_PercentSuccess->setEnabled(false);
    ui->ThroughputTestResults_Throughput->setEnabled(false);

    ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(false);
    connect(ui->ThroughputTest_RangeScanning, &QCheckBox::clicked, this, &RadioControlsWindow::rangeScanningBoxClicked);

    connect(ui->LinkTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::linkTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::linkTestDataAvailable, this, &RadioControlsWindow::linkTestDataAvailable);

    connect(&Backend::getInstance(), &Backend::linkTestFailedSignal, this, &RadioControlsWindow::linkTestFailed);

    connect(ui->ThroughputTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::throughputTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::throughputTestDataAvailable, this, &RadioControlsWindow::throughputTestDataAvailable);

    connect(ui->RefreshSerialPortsButton, &QPushButton::pressed, []() {
        Backend::getInstance().getPorts();
    });


//    connect(serialPortListObj, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(serialPortChosen(QListWidgetItem*, QListWidgetItem*)));
}

/*
void RadioControlsWindow::serialPortChosen(QListWidgetItem *item, QListWidgetItem *_)
{
    radioModuleButton->setText("Connect");
    radioModuleButton->setEnabled(true);
}
 */

RadioControlsWindow::~RadioControlsWindow()
{
    delete ui;
}