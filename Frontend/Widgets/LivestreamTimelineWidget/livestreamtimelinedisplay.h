//
// Created by William Scheirey on 2/20/25.
//

#ifndef GROUNDSTATION_2025_LIVESTREAMTIMELINEDISPLAY_H
#define GROUNDSTATION_2025_LIVESTREAMTIMELINEDISPLAY_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class LivestreamTimelineDisplay;
}
QT_END_NAMESPACE

class LivestreamTimelineDisplay : public QWidget
{
Q_OBJECT

public:
    explicit LivestreamTimelineDisplay(QWidget *parent = nullptr);

    ~LivestreamTimelineDisplay() override;

private:
    Ui::LivestreamTimelineDisplay *ui;
};


#endif //GROUNDSTATION_2025_LIVESTREAMTIMELINEDISPLAY_H
