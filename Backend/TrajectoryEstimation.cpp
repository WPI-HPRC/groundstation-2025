// Created by Rafael on 29.01.25.

#include "TrajectoryEstimation.h"
#include <Eigen>

TrajectoryEstimation::TrajectoryEstimation():
  estimatedPosX(0), estimatedPosY(0), estimatedPosZ(0),
  estimatedVelX(0), estimatedVelY(0), estimatedVelZ(0),
  dt(0.001), Position{0,0,0}, Velocity{0,0,0} {}

// Process incoming telemetry data

void TrajectoryEstimation::processRocketData(const GroundStation::RocketTelemPacket &data) {
    if (data.gpsLock && data.timestamp > lastTimestamp) {
        lastTimestamp = data.timestamp;
        // Incoming telemetry ==> Update the Kalman Filter and estimate the position of the rocket based on telemetry

        Eigen::VectorXd posMeasurementVec(3);
        posMeasurementVec << data.posX, data.posY, data.posZ;

        kalmanPosX.update(posMeasurementVec);
        kalmanPosY.update(posMeasurementVec);
        kalmanPosZ.update(posMeasurementVec);
        estimatedPosX = posMeasurementVec(0);
        estimatedPosY = posMeasurementVec(1);
        estimatedPosZ = posMeasurementVec(2);

        Eigen::VectorXd velMeasurementVec(3);
        velMeasurementVec << data.velX, data.velY, data.velZ;

        kalmanVelX.update(velMeasurementVec);
        kalmanVelY.update(velMeasurementVec);
        kalmanVelZ.update(velMeasurementVec);
        estimatedVelX = velMeasurementVec(0);
        estimatedVelY = velMeasurementVec(1);
        estimatedVelZ = velMeasurementVec(2);

    } else {// No incoming telemetry ==> Estimate the trajectory by extrapolation
        kalmanPosX.predict(dt);
        kalmanPosY.predict(dt);
        kalmanPosZ.predict(dt);
        kalmanVelX.predict(dt);
        kalmanVelY.predict(dt);
        kalmanVelZ.predict(dt);

        estimatedPosX = kalmanPosX.getState()(0);
        estimatedPosY = kalmanPosY.getState()(1);
        estimatedPosZ = kalmanPosZ.getState()(2);
        estimatedVelX = kalmanVelX.getState()(0);
        estimatedVelY = kalmanVelY.getState()(1);
        estimatedVelZ = kalmanVelZ.getState()(2);}
}
double TrajectoryEstimation::getEstimatedPosX() const { return estimatedPosX; }
double TrajectoryEstimation::getEstimatedPosY() const { return estimatedPosY; }
double TrajectoryEstimation::getEstimatedPosZ() const { return estimatedPosZ; }

double TrajectoryEstimation::getEstimatedVelX() const {return estimatedVelX; }
double TrajectoryEstimation::getEstimatedVelY() const {return estimatedVelY; }
double TrajectoryEstimation::getEstimatedVelZ() const {return estimatedVelZ; }

//Kalman Filter Design
KalmanFilter::KalmanFilter() {
    // Initialize state vector: [x, y, z, vx, vy, vz] with zeros
    state = Eigen::VectorXd::Zero(6);

    // Initialize covariance matrix (high uncertainty initially)
    covariance = Eigen::MatrixXd::Identity(6, 6) * 1000;

    // State transition matrix (updated in predict)
    transition_matrix = Eigen::MatrixXd::Identity(6, 6);

    // Measurement matrix (only measuring position [x, y, z])
    measurement_matrix = Eigen::MatrixXd::Zero(3, 6);
    measurement_matrix.block<3,3>(0,0) = Eigen::MatrixXd::Identity(3,3);

    // Process and measurement noise matrices
    process_noise = Eigen::MatrixXd::Identity(6, 6) * 0.1;
    measurement_noise = Eigen::MatrixXd::Identity(3, 3) * 5;

    // Identity matrix
    identity = Eigen::MatrixXd::Identity(6, 6);

    // Noise factors (to be tuned based on system performance)
    process_noise_factor = 1.0;
    measurement_noise_factor = 1.0; // Velocity or Position???????????????
}

void KalmanFilter::initialize(const Eigen::VectorXd& initial_state, const Eigen::MatrixXd& initial_covariance) {
    state = initial_state;
    covariance = initial_covariance;
}

void KalmanFilter::predict(double dt) {
    // Update transition matrix with time step dt
    transition_matrix << 1, 0, 0, dt, 0, 0,
                         0, 1, 0, 0, dt, 0,
                         0, 0, 1, 0, 0, dt,
                         0, 0, 0, 1, 0, 0,
                         0, 0, 0, 0, 1, 0,
                         0, 0, 0, 0, 0, 1;

    // Predict new state and covariance
    state = transition_matrix * state;
    covariance = transition_matrix * covariance * transition_matrix.transpose() + process_noise_factor * process_noise;
}

void KalmanFilter::update(const Eigen::VectorXd& measurement) {
    // Compute Kalman gain
    Eigen::MatrixXd S = measurement_matrix * covariance * measurement_matrix.transpose() + measurement_noise_factor * measurement_noise;
    Eigen::MatrixXd K = covariance * measurement_matrix.transpose() * S.inverse();

    // Update state estimate with measurement
    state += K * (measurement - measurement_matrix * state);

    // Update covariance estimate
    covariance = (identity - K * measurement_matrix) * covariance;
}
Eigen::VectorXd KalmanFilter::getState() const {
    return state;
}

/* Tuning the Kalman Filter
 * 