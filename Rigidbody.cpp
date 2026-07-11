//
// Created by yaly on 29/06/2026.
//

#include "Rigidbody.h"

#include "GameObject.h"
#include "World.h"

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
