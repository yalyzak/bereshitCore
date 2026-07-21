//
// Created by User on 20/07/2026.
//

#include "FixedJoint.h"

#include "Rigidbody.h"




void FixedJoint::SolveLinear(double dt) {
    auto IinvA = rbA->GetInvertWorld();
    auto IinvB = rbB->GetInvertWorld();
    double invertMassA = rbA->GetInvMass();
    double invertMassB = rbB->GetInvMass();
    Vector3 rA = transformA->quaternion.RotateConjugated(localAnchorA);
    Vector3 rB = transformB->quaternion.RotateConjugated(localAnchorB);

    Vector3 vA = rbA->velocity + rbA->angularVelocity.cross(-rA);
    Vector3 vB = rbA->velocity + rbA->angularVelocity.cross(-rA);

    Vector3 dv = vB - vA;

    Vector3 worldAnchorA = transformA->position + rA;
    Vector3 worldAnchorB = transformB->position + rB;

    Vector3 positionError = worldAnchorA + worldAnchorB;

    Vector3 bias = positionError * (beta / dt);

    // Effective mass matrix  K = (1/mA + 1/mB)*I + [rA]x * IinvA * [rA]x^T + [rB]x * IinvB * [rB]x^T

    double invertMass = invertMassA + invertMassB;

    BuildEffectiveMassMatrix(invertMass, rA, rB, *IinvA, *IinvB); // finds K

    Vector3 impulse = -Solve3x3(dv + bias);

    // if (!rbA->isKinematic) {
    //     rbA->velocity -= impulse * invertMassA;
    //     rbA->angularVelocity += rA.cross(impulse).MatrixMultiplication(*IinvA);
    // }
    // if (!rbB->isKinematic) {
    //     rbB->velocity += impulse * invertMassB;
    //     rbB->angularVelocity -= rB.cross(impulse).MatrixMultiplication(*IinvB);
    // }

    // Rigidbody::ApplyImpulsePair(*rbA, *rbB, impulse, rA, rB);
}

void FixedJoint::SolveAngular(double dt) {
    auto IA = rbA->GetInvertWorld();
    auto IB = rbB->GetInvertWorld();

    Quaternion q_rel = (
            transformA->quaternion.Inverse() *
            transformB->quaternion
    );

    Quaternion q_error = q_rel * initialRelativeRotation.Inverse();

    Vector3 error = {q_error.x, q_error.y, q_error.z};
    if (q_error.w < 0) {
        error = error * -1;

    }

    Vector3 angular_error = error * 2.0;

    Vector3 bias = angular_error * (beta / dt);

    Vector3 rel_w = rbB->angularVelocity - rbA->angularVelocity;

    AddMatrix(*IA, *IB); // result is in K

    Vector3 impulse = -Solve3x3((rel_w + bias));

    // if (!rbA->isKinematic) {
    //     rbA->angularVelocity -= impulse.MatrixMultiplication(*IA);
    // }
    // rbB->angularVelocity += impulse.MatrixMultiplication(*IB);
}


