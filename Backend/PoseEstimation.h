//
// Created by Rafael on 31.03.25.

#ifndef POSEESTIMATION_H
#define POSEESTIMATION_H

#include <cmath>
#include "Utility/Utility.h"
#include <iostream>

class PoseEstimator
{
public:
    PoseEstimator(GroundStation::RocketTelemPacket);
    void UpdateRocketPosition(GroundStation::RocketTelemPacket);
    double computeYaw();
    double computePitch();
private:
    //Position of the Antenna
    double AntennaLat;
    double AntennaLon;
    double AntennaAlt;
    //Position of the Rocket
    double RocketLat;
    double RocketLon;
    double RocketAlt;

    double Yaw;
    double Pitch;

    static double toRadians(double deg);
    static double toDegrees(double rad);
};

class UpdateRocketPosition
{
    public:
};

class computeYaw
{
    public:
};

class computePitch
{
    public:
};
#endif //POSEESTIMATION_H
