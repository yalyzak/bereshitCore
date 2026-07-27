//
// Created by yaly on 23/07/2026.
//

#include "HingeJoint.h"

#include "Rigidbody.h"
#include "World.h"

Vector3 HingeJoint::GetWorldAxis() {
    return transformB->quaternion.RotateConjugated(axisLocal).normalized();
}

HingeJoint::HingeJoint(GameObject* bodyB, Vector3 axis, Vector3* anchor, double beta) : Joint(bodyB, anchor, beta) {
    axisLocal = axis.normalized();
}

HingeJoint * HingeJoint::Copy() const {
    return new HingeJoint(bodyB, axisLocal ,worldAnchor, beta);
}






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
    auto IinvA = rbA->GetInvertWorld();
    auto IinvB = rbB->GetInvertWorld();

    Vector3 axis_world = transformA->quaternion.RotateConjugated(axisLocal).normalized();

    Vector3 t1 = axis_world.Perpendicular();
    Vector3 t2 = axis_world.cross(t1).normalized();

    Vector3 rel_w = rbB->angularVelocity - rbA->angularVelocity;

    // Construct the 2-row Jacobian: J = [t1^T; t2^T]
    // Effective mass:  K_ang = J * (IinvA + IinvB) * J^T   (2x2)

    AddMatrix(*IinvA, *IinvB); // result is in K

    Vector3 Kt1 = t1.MatrixMultiplication(K);
    Vector3 Kt2 = t2.MatrixMultiplication(K);

    // J @ K_full @ J.T
    Vector2 K_ang[2];
    K_ang[0] = Vector2(t1.dot(Kt1), t1.dot(Kt2));
    K_ang[1] = Vector2(t2.dot(Kt1), t2.dot(Kt2));


    Vector2 vel_error(t1.dot(rel_w), t2.dot(rel_w));

    Quaternion q_rel = transformA->quaternion.Inverse() * transformB->quaternion;

    // self.clamp_rotation(q_rel, IinvA, IinvB, a, b)

    Quaternion q_error = q_rel * initialRelativeRotation.Inverse();

    Vector3 err_vec(q_error.x, q_error.y, q_error.z);
    if (q_error.w < 0) {
        err_vec = err_vec * -1;
    }
    Vector3 ang_error = err_vec * 2.0;

    Vector2 bias(t1.dot(ang_error) * (beta / dt), t2.dot(ang_error) * (beta / dt));

    Vector2 ang_impulse2d = -Solve2x2(vel_error + bias, K_ang);

    Vector3 ang_impulse = t1 * ang_impulse2d.x + t2 * ang_impulse2d.y;

    if (!rbA->IsKinematic()) {
        rbA->angularVelocity -= ang_impulse.MatrixMultiplication(*IinvA);

    }
    if (!rbB->IsKinematic()) {
        rbB->angularVelocity += ang_impulse.MatrixMultiplication(*IinvB);

    }
}
