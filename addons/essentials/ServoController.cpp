//
// Created by yaly on 03/08/2026.
//

#include "ServoController.h"

#include "../../GameObject.h"


void ServoController::attach(GameObject &obj) {
    rigidbody = obj.GetComponent<Rigidbody>();
}

void ServoController::Fix(double dt) {
    Vector3 axis = joint->GetWorldAxis();
    Quaternion relative_q = mount->transform.quaternion.Conjugate() * GetParent()->transform.quaternion;

    double current_angle = relative_q.ToEuler().dot(axis);

    double error = targetAngle - current_angle;

    error = std::fmod(error + 180.0, 360.0);
    if (error < 0)
        error += 360.0;
    error -= 180.0;

    double angular_velocity = rigidbody->angularVelocity.dot(axis);

    const double slow_distance = 20.0;

    double desired_velocity = maxSpeed * std::max(std::min(error / slow_distance, 1.0), -1.0);

    double velocity_error = desired_velocity - angular_velocity;

    double torque = velocity_error * maxTorque;

    torque = std::max(std::min(torque, torque), -torque);

    if (std::abs(error) < 0.3 && std::abs(angular_velocity) < 0.2) {
            torque = 0;
    }
    rigidbody->ApplyAngularImpulse(axis * torque * dt);
}

void ServoController::TurnTo(double degrees, double dt) {


}

void ServoController::ResetToDefault() {
    targetAngle = 0;
}
