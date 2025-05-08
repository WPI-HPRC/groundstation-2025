//
// Created by Rafael on 31.03.25.
//
#include "PoseEstimation.h"

#include <absl/strings/str_format.h>

#include "absl/strings/internal/str_format/extension.h"
#include "Utility/Utility.h"

PoseEstimator::PoseEstimator(const GroundStation::RocketTelemPacket)   //GroundStation::AntennaPosition fix
    :AntennaLon(),AntennaLat(),AntennaAlt(), Yaw(0.0), Pitch(0.0) {} //initialize antenna's location

void PoseEstimator::UpdateRocketPosition(GroundStation::RocketTelemPacket) {
    computeYaw();
    computePitch();
}




double computeYaw(double x1, double y1, double x2, double y2) { //y2 is the new coordinate of rocket along y, x2 along x
    double Yaw = atan2(y2-y1, x2-x1); //radians
    return Yaw;
}
double computePitch(double z1, double z2, double x1, double x2, double y1, double y2 ) {
    //Equations
    double Pitch = atan2(z2,sqrt(y2*y2 + x2*x2))-atan2(z1,sqrt(y1*y1+x1*x1)); //z2 is updated altitude
    return Pitch;
}

double convertLon(){
    return Yposition;
}

double convertLat(){
    return Xposition;
}

/*
 1. Initialize Antenna position--> using AntennaLon... from antenna GPS
 2. Rocket feeds telementry to UpdateRocketPosition, which is connected to Pose estimator
 3. Before reaching pose stimator telemetry goes through computeYaw and computePitch that calculates the required pitch and yaw angles
 4. Based on these values Pose estimator does simple addition/substraction to get the adjustment angle (Antenna has gyroscope)
 5. Done!


 */