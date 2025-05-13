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



double computeYaw(double x1, double y1, double x2, double y2) {
    double Yaw = atan2(y2-y1, x2-x1); //angle from x-axis in radians
    return Yaw; // Absolute yaw to the rocket in radians
}
double computePitch(double x1, double x2, double y1, double y2,double z1, double z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    double horiz_distance = sqrt(dx*dx + dy*dy);
    double Pitch = atan2(dz,horiz_distance); //angle from xy-plane in radians
    return Pitch; //Absolute pitch to the rocket in radians
}
/*
y1--> position of the antenna
 y2--> updated position of the rocket
 y2-y1--> relative vector

x1--> position of the antenna
 x2--> updated position of the rocket
 x2-x1--> relative vector
 */

double x1 = convertLonToX(lon_antenna,lat_antenna_)

convertLonToX(double )


double y1= 0.0;
bool isYReferenceSet = false;
double convertLonToY(double y2){
    if (!isYReferenceSet){
        y1 = y2;
        isYReferenceSet = true;
    }
    return y2-y1; //y2 is the current longitude (y1 is the reference)
}


double convertLatToY(){
    return x2;
}
/*
 1. Initialize Antenna position--> using AntennaLon... from antenna GPS
 2. Rocket feeds telementry to UpdateRocketPosition, which is connected to Pose estimator
 3. Before reaching pose stimator telemetry goes through computeYaw and computePitch that calculates the required pitch and yaw angles
 4. Based on these values Pose estimator does simple addition/substraction to get the adjustment angle (Antenna has gyroscope)
 5. Done!


/*
 Converter from gps to x and y
 */