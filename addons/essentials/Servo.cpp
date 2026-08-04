//
// Created by yaly on 03/08/2026.
//

#include "Servo.h"
#include "ServoController.h"

#include "../../GameObject.h"
#include "../../HingeJoint.h"

Servo::Servo(GameObject *mount, Vector3 axis, double maxRotation, double minRotation, double speed, double maxSpeed,
             double torque) : servo(nullptr), mount(mount), axis(axis), maxRotation(maxRotation), minRotation(minRotation), speed(speed), maxSpeed(maxSpeed),
                              torque(torque) {
}

void Servo::attach(GameObject &obj) {
    servo = &obj;
    HingeJoint* hinge_joint = new HingeJoint(mount, axis);
    ServoController* servo_controller = new ServoController(mount, hinge_joint, maxRotation, minRotation, speed, maxSpeed, torque);

    servo->AddComponent(hinge_joint);
    servo->AddComponent(servo_controller);
}
