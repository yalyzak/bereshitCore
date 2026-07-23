//
// Created by yaly on 23/07/2026.
//

#include "HingeJoint.h"

#include "Rigidbody.h"

void HingeJoint::SolveLinear(double dt) {
    auto IinvA = rbA->GetInvertWorld();
    auto IinvB = rbB->GetInvertWorld();
    double invertMassA = rbA->GetInvMass();
    double invertMassB = rbB->GetInvMass();
    Vector3 rA = transformA->quaternion.RotateConjugated(localAnchorA);
    Vector3 rB = transformB->quaternion.RotateConjugated(localAnchorB);

    Vector3 vA = rbA->velocity + rbA->angularVelocity.cross(-rA);
    Vector3 vB = rbB->velocity + rbB->angularVelocity.cross(-rB);

    Vector3 dv = vB - vA;

    Vector3 worldAnchorA = transformA->position + rA;
    Vector3 worldAnchorB = transformB->position + rB;

    Vector3 positionError = worldAnchorB - worldAnchorA;

    Vector3 bias = positionError * (beta / dt);

    // Effective mass matrix  K = (1/mA + 1/mB)*I + [rA]x * IinvA * [rA]x^T + [rB]x * IinvB * [rB]x^T

    double invertMass = invertMassA + invertMassB;

    BuildEffectiveMassMatrix(invertMass, rA, rB, *IinvA, *IinvB); // finds K

    Vector3 impulse = -Solve3x3(dv + bias);

    Rigidbody::ApplyImpulsePair(*rbA, *rbB, impulse, rA, rB);
}

void HingeJoint::SolveAngular(double dt) {
}
