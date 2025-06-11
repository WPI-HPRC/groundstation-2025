//
// Created by Nic DeBruin on 6/10/25.
//

#ifndef TRACKERPOSEESTIMATOR_H
#define TRACKERPOSEESTIMATOR_H
#include <math.h>
#include <bits/valarray_after.h>

struct GPSData{
    double lat;
    double lon;
    double alt;
};

struct Coord {
    double x;
    double y;
};

enum DataSource {
    Stick = 0,
    Barometer = 1,
    EKF = 2,
    GPS = 3,
    APRS = 4
};

const double earthRadius = 6371e3; // meters


class TrackerPoseEstimator {

public:
    // getter funcs
    double getBarometricAltitude();
    double getEKFAltitude();
    double getGPSAltitude();
    double getAPRSAltitude();

    double getEKFDistance();
    double getGPSDistance();
    double getAPRSDistance();

    double getEKFHeading();
    double getGPSHeading();
    double getAPRSHeading();

    double getPitch(DataSource altitudeSource, DataSource distanceSource);
    double getYaw(DataSource altitudeSource, DataSource distanceSource);

    void setTrackerGPS();

    void setTrackerHeading();

private:


    // there's gonna be hella inline functions in here lmao
    Coord gpsToXYDist(GPSData point1, GPSData point2) {
        Coord coord;
        double radianLat1 = point1.lat * M_PI/180.0;
        double radianLat2 = point2.lat * M_PI/180.0;
        double radianLon1 = point1.lon * M_PI/180.0;
        double radianLon2 = point2.lon * M_PI/180.0;

        coord.y = std::sin(radianLon2 - radianLon1) * std::cos(radianLat2);
        coord.x = std::cos(radianLat1) * std::sin(radianLat2) - std::sin(radianLat1)*std::cos(radianLat2)*std::cos(radianLon2 - radianLon1);

        return coord;
    }
};





#endif //TRACKERPOSEESTIMATOR_H
