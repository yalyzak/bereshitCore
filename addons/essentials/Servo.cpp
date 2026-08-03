//
// Created by yaly on 03/08/2026.
//

#include "Servo.h"

#include "../../GameObject.h"
#include "../../HingeJoint.h"

Servo::Servo(GameObject *Mount, Vector3 Axis, double MaxRotation, double MinRotation, double Speed, double MaxSpeed,
             double Torque) {
    mount = Mount;
    axis = Axis;

    maxRotation = MaxRotation;
    minRotation = MinRotation;
    speed = Speed;
    maxSpeed = MaxSpeed;
    torque = Torque;
}

void Servo::attach(GameObject &obj) {
    servo = &obj;
    HingeJoint* hinge_joint = new HingeJoint(mount, axis);

    servo->AddComponent(hinge_joint);
}
