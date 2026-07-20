//
// Created by yaly on 19/07/2026.
//

#ifndef BERESHITCORE_JOINT_H
#define BERESHITCORE_JOINT_H


#include "Component.h"
class GameObject;
class Transform;
#include "Vector3.h"
#include "Quaternion.h"


class Joint : public Component{
    private:
        double solve3x3Array[3][3];
        double solve2x2Array[2][2];
        virtual void CastAnchorDefault();
        Transform* transformA;
        Transform* transformB;

    public:
        Joint(GameObject* bodyB, Vector3 anchor, double beta = 0.2);
        GameObject* bodyB;
        GameObject* bodyA;
        Vector3 localAnchorB;
        Vector3 localAnchorA;
        Quaternion initialRelativeRotation;
        Vector3 rbB;
        Vector3 rbA;
        Vector3* worldAnchor;
        double beta;
        double invMassArray[3];
        Vector3 angularImpulse;
        double K[3][3];
        void attach(GameObject &obj) override;
        void CastAnchor();

};


#endif //BERESHITCORE_JOINT_H
