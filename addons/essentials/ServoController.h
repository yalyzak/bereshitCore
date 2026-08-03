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
        double targetAngle;
        GameObject* mount;
        HingeJoint* joint;
        Rigidbody* rigidbody;
        double maxRotation;
        double minRotation;
        double speed;
        double maxSpeed;
        double maxTorque;

    public:
        void attach(GameObject &obj) override;
        void Fix(double dt);
        void TurnTo(double degrees, double dt);
        void ResetToDefault() override;
        [[nodiscard]] double GetTargetAngle() const {
            return targetAngle;
        }
};


#endif //BERESHITCORE_SERVOCONTROLLER_H
