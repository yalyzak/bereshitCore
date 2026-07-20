//
// Created by yaly on 19/07/2026.
//

#include "Joint.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"

void Joint::CastAnchorDefault() {
}

Joint::Joint(GameObject *bodyB, Vector3 anchor, double beta) : bodyB(bodyB), worldAnchor(&anchor), beta(beta) {
}

void Joint::attach(GameObject &obj) {
    bodyA = &obj;
    Rigidbody* rbA = bodyA->GetComponent<Rigidbody>();
    Rigidbody* rbB = bodyB->GetComponent<Rigidbody>();
    transformA = &bodyA->transform;
    transformB = &bodyB->transform;

    if (worldAnchor == nullptr) {
        CastAnchor();
    }


}

void Joint::CastAnchor() {
    Vector3* hit = Physics::RayCast(transformA->position, (transformB->position - transformA->position), bodyB->GetComponent<Collider>()).point;

    if (hit != nullptr) {
        worldAnchor = hit;

    }else {
        CastAnchorDefault();

        initialRelativeRotation = (bodyA->transform.quaternion.Inverse() * transformB->quaternion);

        localAnchorA = transformA->quaternion.RotateConjugated(*worldAnchor - transformA->position);
        localAnchorB = transformB->quaternion.RotateConjugated(*worldAnchor - transformB->position);
    }



}
