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

const double a = 6378137.0;           // semi-major axis in meters
const double e = 6.69437999014e-3; // Earth's eccentricity squared

struct Vec1  {
    double x1, y1, z1;
};
Vec1 gpsToXY(double lat_deg, double lon_deg, double alt_m) {
    double lat = lat_deg * M_PI / 180.0; //Degrees to Radians
    double lon = lon_deg * M_PI / 180.0; //Degrees to Radians

    double N = a / sqrt(1 - e * sin(lat) * sin(lat));
    double x1 = (N + alt_m) * cos(lat) * cos(lon);
    double y1 = (N + alt_m) * cos(lat) * sin(lon);
    double z1 = (N * (1 - e) + alt_m) * sin(lat);
    return {x1, y1, z1}; //Antenna
}

struct Vec2  {
    double x2, y2, z2;
};
Vec2 gpsToXY(double lat_deg, double lon_deg, double alt_m) {
    double lat = lat_deg * M_PI / 180.0; //Degrees to Radians
    double lon = lon_deg * M_PI / 180.0; //Degrees to Radians

    double N = a / sqrt(1 - e * sin(lat) * sin(lat));
    double x2 = (N + alt_m) * cos(lat) * cos(lon);
    double y2 = (N + alt_m) * cos(lat) * sin(lon);
    double z2 = (N * (1 - e) + alt_m) * sin(lat);
    return {x2, y2, z2}; //Rocket
}

double computeYaw(double x1, double y1, double x2, double y2) {
    double Yaw = atan2(y2-y1, x2-x1); //angle from x-axis in radians
    return Yaw; // Absolute yaw to the rocket in radians
}
double computePitch(double x1, double x2, double y1, double y2,double z1, double z2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    double horiz_distance = sqrt(dx*dx + dy*dy);
    double Pitch = atan2(dz,horiz_distance); //angle from xy-plane in radians
    return Pitch; //Absolute pitch to the rocket in radians
} /*
y1--> position of the antenna
 y2--> updated position of the rocket
 y2-y1--> relative vector

x1--> position of the antenna
 x2--> updated position of the rocket
 x2-x1--> relative vector
 */
