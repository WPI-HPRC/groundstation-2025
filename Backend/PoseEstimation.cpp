//
// Created by Rafael on 31.03.25.
//
#include "PoseEstimation.h"
#include <generated/telemetry/RocketTelemetryPacket.pb.h>

#include "Utility/Utility.h"

PoseEstimator::PoseEstimator(const GroundStation::RocketTelemPacket)
    :AntennaLon(),AntennaLat(),AntennaAlt(), Yaw(0.0), Pitch(0.0) {} //initialize antenna's location

void PoseEstimator::UpdateRocketPosition(GroundStation::RocketTelemPacket) {
    computeYaw();
    computePitch();
}

double PoseEstimator::computeYaw() const{
    return Yaw;
}

double PoseEstimator::getPitch() const{
    return Pitch;
}

void PoseEstimator::computeYaw()


