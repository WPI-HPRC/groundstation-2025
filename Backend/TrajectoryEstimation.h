// Created by Rafael on 29.01.25.

#ifndef TRAJECTORYESTIMATION_H
#define TRAJECTORYESTIMATION_H

#include <iostream>
#include <array>
#include <Eigen>
#include "generated/telemetry/RocketTelemetryPacket.pb.h"
#include "Utility/Utility.h"

//Kalman Filter

class KalmanFilter {
public:
    KalmanFilter(); // Initialize filter parameters
    void initialize(const Eigen::VectorXd& initial_state, const Eigen::MatrixXd& initial_covariance); // Set initial state and covariance
    void predict(double dt); // Predict next state using time step dt
    void update(const Eigen::VectorXd& measurement); // Update state with new measurement
    [[nodiscard]] Eigen::VectorXd getState() const; // Collect current state estimate

private:
    Eigen::VectorXd state; // State vector: [x, y, z, vx, vy, vz]
    Eigen::MatrixXd covariance; // State covariance matrix
    Eigen::MatrixXd transition_matrix; // State transition matrix
    Eigen::MatrixXd measurement_matrix; // Measurement matrix
    Eigen::MatrixXd process_noise; // Process noise covariance
    Eigen::MatrixXd measurement_noise; // Measurement noise covariance
    Eigen::MatrixXd identity; // Identity matrix for covariance update

    double process_noise_factor; // Process noise scaling factor (to be tuned)
    double measurement_noise_factor; // Measurement noise scaling factor (to be tuned)
};

// ==== Trajectory Estimation =========

class TrajectoryEstimation {
public:
    TrajectoryEstimation();
    void processRocketData(const GroundStation::RocketTelemPacket &data);
    [[nodiscard]] double getEstimatedPosX() const;
    [[nodiscard]]double getEstimatedPosY() const;
    [[nodiscard]]double getEstimatedPosZ() const;
    [[nodiscard]]double getEstimatedVelX() const;
    [[nodiscard]]double getEstimatedVelY() const;
    [[nodiscard]]double getEstimatedVelZ() const;

private:
    KalmanFilter kalmanPosX, kalmanPosY, kalmanPosZ; //Kalman filters for position dimensions
    KalmanFilter kalmanVelX, kalmanVelY, kalmanVelZ; // +velocity dimensions
    double estimatedPosX;
    double estimatedPosY;
    double estimatedPosZ;
    double estimatedVelX;
    double estimatedVelY;
    double estimatedVelZ;

    uint32_t lastTimestamp =0; //first telemetry data updates this
    double dt; // Time step

    std::array<double, 3> Position; // State Vector [posX, posY, posZ]
    std::array<double, 3> Velocity;// State Vector [velX, velY, velZ]
};

#endif // TRAJECTORYESTIMATION_H

