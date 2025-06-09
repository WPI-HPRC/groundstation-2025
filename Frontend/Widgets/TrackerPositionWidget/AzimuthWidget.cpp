//
// Created by William Scheirey on 6/9/25.
//

#include "AzimuthWidget.h"
#include "Tracker/Tracker.h"
AzimuthWidget::AzimuthWidget(QWidget *parent): CirclePositionWidget(parent)
{
    repaint();
    value = 45;
    targetValue = 90;
    connect(&Tracker::getInstance(), &Tracker::newDesiredPose, this, [this](Tracker::Pose pose)
    {
        updateTargetValue(pose.azimuth_degrees);
    });
    connect(&Tracker::getInstance(), &Tracker::newPoseData, this, [this](Tracker::Pose pose)
    {
        updateTargetValue(pose.azimuth_degrees);
    });
}