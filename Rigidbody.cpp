//
// Created by yaly on 29/06/2026.
//

#include "Rigidbody.h"

#include "GameObject.h"
#include "World.h"
#include "Vector3.h"

void Rigidbody::UpdateInertiaWorld() {
    if (isKinematic) {
        InvertWorld = {{
            {{0, 0, 0}},
            {{0, 0, 0}},
            {{0, 0, 0}}
        }};
        return;
    }

    auto R = GetParent()->transform.quaternion.ToMatrix3(&GetParent()->cache);

    // temp = R * inverse_inertia
    std::array<std::array<double, 3>, 3> temp{};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                temp[i][j] += R[i][k] * InvertWorld[k][j];
            }
        }
    }

    // Iinv_world = temp * R^T
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            InvertWorld[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                InvertWorld[i][j] += temp[i][k] * R[j][k]; // R^T[k][j] = R[j][k]
            }
        }
    }
}

Rigidbody::Rigidbody() {
    SetName("Rigidbody");
}

void Rigidbody::apply_gravity(const Vector3 &gravity) {
    force += gravity;
}


void Rigidbody::PhysicsUpdate(double dt) {

}

void Rigidbody::PhysicsUpdateFirstIteration(double dt) {
    if (!isKinematic) {
        apply_gravity(this->GetParent()->GetWorld()->gravity);
    }
    integrate(dt);
}


void Rigidbody::integrate(double dt) {
    acceleration = force * invMass;
    Vector3 pos = velocity * dt + acceleration * 0.5 * dt * dt;
    if (pos.magnitude() > 0) {
        GetParent()->cache.aabbDirty = true;
    }

    GetParent()->transform.position += velocity * dt + acceleration * 0.5 * dt * dt;
    velocity += acceleration * dt;
    force.Zero();
    torque.Zero();

}

void Rigidbody::SolveImpulse(Rigidbody &rb1, Rigidbody &rb2, const Vector3& contact_point, const Vector3& normal,
    const Vector3& penetration, double dt) {
        if (!rb1.isKinematic) {
            rb1.velocity += (rb1.force * rb1.invMass) * dt;

        }
        rb1.force.Zero();
        if (!rb2.isKinematic) {
            rb2.velocity += (rb2.force * rb2.invMass) * dt;
            rb2.force.Zero();
        }
        Vector3 r1 = contact_point - rb1.GetParent()->transform.position;
        Vector3 r2 = contact_point - rb2.GetParent()->transform.position;

        Vector3 v1_at_p = rb1.velocity - rb1.angular_velocity.cross(r1);
        Vector3 v2_at_p = rb2.velocity - rb2.angular_velocity.cross(r2);
        Vector3 relative_vel = v2_at_p - v1_at_p;
        double v_norm = relative_vel.dot(normal);

        if (v_norm >= 0) {
            return;
        }

        Vector3 rn1 = r1.cross(normal);
        Vector3 rn2 = r2.cross(normal);
        Vector3 term1(0, 0, 0);
        Vector3 term2(0, 0, 0);
        if (!rb1.isKinematic) {
            term1 = (rn1.MatrixMultiplication(*rb1.GetInvertWorld())).cross(r1);
        }
        if  (!rb2.isKinematic) {
            term2 = (rn2.MatrixMultiplication(*rb2.GetInvertWorld())).cross(r2);
        }

        double restitution = FindRestitution(rb1, rb2);
        if (rb1.isKinematic) {

        }
        double kLinear =  (rb1.isKinematic ? 0.0 : rb1.invMass) + (rb2.isKinematic ? 0.0 : rb2.invMass);
        double kAngular = normal.dot(term1 + term2);
        double inv_eff_mass = kLinear + kAngular;

        // Rigidbody.positional_correction(rb1, rb2, penetration, normal, inv_eff_mass)

        double J = -(1 + restitution) * v_norm / inv_eff_mass;

        Rigidbody::ApplyImpulsePair(rb1, rb2, normal * J, r1, r2);


        // rb1._apply_friction_impulse(rb2, relative_vel, normal, J, r1, r2)
}

double Rigidbody::FindRestitution(const Rigidbody &rb1, const Rigidbody &rb2) {
    return std::min(rb1.GetRestitution(), rb2.GetRestitution());
}

void Rigidbody::ApplyImpulsePair(Rigidbody& rb1, Rigidbody &rb2, const Vector3 &impulseVec,
    const Vector3 &r1, const Vector3 &r2) {
    Vector3 negative_impulse = -impulseVec;

    if (!rb1.isKinematic) {
        rb1.velocity += negative_impulse * rb1.invMass;
        rb1.ApplyTorqueImpulse(impulseVec, r1);
    }


    if (!rb2.isKinematic) {
        rb2.velocity += impulseVec * rb2.invMass;
        rb2.ApplyTorqueImpulse(negative_impulse, r2);
    }


}

void Rigidbody::ApplyTorqueImpulse(Vector3 impulse, Vector3 r) {
    Vector3 torqueImpulse = r.cross(impulse);
    Vector3 localTorqueImpulse = GetParent()->transform.quaternion.RotateConjugated(torqueImpulse);
    Vector3 local_delta_w = localTorqueImpulse * invertInertia;
    Vector3 ang_impulse = GetParent()->transform.quaternion.Rotate(local_delta_w);

    if (!Freeze_Rotation.x) {
        angular_velocity.x += ang_impulse.x;
    }
    if (!Freeze_Rotation.y) {
        angular_velocity.y += ang_impulse.y;
    }
    if (!Freeze_Rotation.z) {
        angular_velocity.z += ang_impulse.z;
    }
}


