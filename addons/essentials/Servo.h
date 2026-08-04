//
// Created by yaly on 03/08/2026.
//

#ifndef BERESHITCORE_SERVO_H
#define BERESHITCORE_SERVO_H

#include "../../Component.h"

class Servo : public Component{
    private:
        GameObject* servo;
        GameObject* mount;
        Vector3 axis;
        double maxRotation;
        double minRotation;
        double speed;
        double maxSpeed;
        double torque;


    public:
        Servo(GameObject* mount, Vector3 axis, double maxRotation = 90, double minRotation = -90, double speed = 60, double maxSpeed = 1, double torque =0.1);
        void attach(GameObject &obj) override;

};


#endif //BERESHITCORE_SERVO_H
