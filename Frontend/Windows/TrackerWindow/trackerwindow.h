//
// Created by William Scheirey on 5/5/25.
//

#ifndef GROUNDSTATION_2025_TRACKERWINDOW_H
#define GROUNDSTATION_2025_TRACKERWINDOW_H

#include <QWidget>


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

private:
    void updateAngleDifferences();
    void updateTableValue(int row, int column, QString value);
    Ui::TrackerWindow *ui;
};


#endif //GROUNDSTATION_2025_TRACKERWINDOW_H
