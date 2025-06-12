//
// Created by William Scheirey on 5/5/25.
//

#ifndef GROUNDSTATION_2025_TRACKERWINDOW_H
#define GROUNDSTATION_2025_TRACKERWINDOW_H

#include <QWidget>
#include "Utility/DataSimulator/DataSimulator.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class TrackerWindow;
}
QT_END_NAMESPACE

class TrackerWindow : public QWidget
{
Q_OBJECT

public:
    explicit TrackerWindow(QWidget *parent = nullptr);

    ~TrackerWindow() override;

    DataSimulator *dataSimulator;
    bool simRunning = false;

private:
    void updateAngleDifferences();
    void updateTableValue(int row, int column, QString value);
    Ui::TrackerWindow *ui;
};


#endif //GROUNDSTATION_2025_TRACKERWINDOW_H
