//
// Created by ndebruin on 6/11/25.
//

#ifndef TRACKERTRAJECTORY_H
#define TRACKERTRAJECTORY_H
#include <math.h>

enum DataSource {
    Stick = 0,
    Barometer = 1,
    EKF = 2,
    GPS = 3,
    APRS = 4
};

struct GPSData {
    double lat;
    double lon;
};

struct Pos2D {
    double x;
    double y;
};

constexpr double DEG_TO_RAD = M_PI/180.0;
constexpr double RAD_TO_DEG = 180.0/M_PI;

class TrackerTrajectory {
public:

    void setAltitudeSource(DataSource source){altitudeSource = source;}

    void setGroundSource(DataSource source) {groundSource = source;}

    double getPitch() {
        switch (altitudeSource) {
            case Stick:
                return 0.0;
            case Barometer:
                return calculatePitch(getBarometerAlt(), getGroundDist());
            case GPS:
                return calculatePitch(getGPSAlt(), getGroundDist());
            case EKF:
                return calculatePitch(getEKFAlt(), getGroundDist());
            case APRS:
                return calculatePitch(getAPRSAlt(), getGroundDist());
        }
    }

    double getYaw() {
        switch (groundSource) {
            case Stick:
                return 0.0;
            case Barometer:
                return 0.0;
            case GPS:
                return getGPSHeading();
            case EKF:
                return getEKFHeading();
            case APRS:
                return getAPRSHeading();
        }
    }

    double getGroundDist() {
        switch (groundSource) {
            case Stick:
                return 0.0;
            case Barometer:
                return 0.0;
            case GPS:
                return getGPSDist();
            case EKF:
                return getEKFDist();
            case APRS:
                return getAPRSDist();
        }
    }

private:
    DataSource groundSource;
    DataSource altitudeSource;

    Pos2D gpsToDistance(GPSData gps1, GPSData gps2) {
        Pos2D dist;
        double dLon = DEG_TO_RAD * (gps2.lon - gps1.lon);
        dist.y = sin(dLon) * cos(DEG_TO_RAD*gps2.lat);
        dist.x = cos(DEG_TO_RAD*gps1.lat) * sin(DEG_TO_RAD*gps2.lat) -
            sin(DEG_TO_RAD*gps1.lat) * cos(DEG_TO_RAD*gps2.lat)*cos(dLon);
        return dist;
    }

    double calculatePitch(double altitude, double groundDist) {
        return atan2(altitude, groundDist) * RAD_TO_DEG;
    }

    double getBarometerAlt();
    double getGPSAlt();
    double getEKFAlt();
    double getAPRSAlt();

    double getGPSDist();
    double getEKFDist();
    double getAPRSDist();

    double getGPSHeading();
    double getEKFHeading();
    double getAPRSHeading();

};



#endif //TRACKERTRAJECTORY_H
