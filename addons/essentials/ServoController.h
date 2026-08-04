//
// Created by yaly on 03/08/2026.
//

#ifndef BERESHITCORE_SERVOCONTROLLER_H
#define BERESHITCORE_SERVOCONTROLLER_H

#include "../../Component.h"
#include "../../HingeJoint.h"
class GameObject;

class ServoController : public Component {
    private:
        double targetAngle = 0.0;
        GameObject* mount;
        HingeJoint* joint;
        Rigidbody* rigidbody;
        double maxRotation;
        double minRotation;
        double inputSpeed;
        double maxSpeed;
        double maxTorque;

    public:
        ServoController(GameObject* mount, HingeJoint* joint, double maxRotation, double minRotation, double inputSpeed, double maxSpeed, double maxTorque);
        void attach(GameObject &obj) override;
        void Fix(double dt);
        void PhysicsUpdate(double dt) override;
        void TurnTo(double degrees, double dt);
        void ResetToDefault() override;
        [[nodiscard]] double GetTargetAngle() const {
            return targetAngle;
        }
};


#endif //BERESHITCORE_SERVOCONTROLLER_H
