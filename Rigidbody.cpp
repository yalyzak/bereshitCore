//
// Created by yaly on 29/06/2026.
//

#include "Rigidbody.h"

#include "GameObject.h"
#include "World.h"

Rigidbody::Rigidbody() = default;

void Rigidbody::apply_gravity(const Vector3 &gravity) {
    force += gravity;
}


void Rigidbody::PhysicsUpdate(double dt) {

}

void Rigidbody::PhysicsUpdateFirstIteration(double dt) {
    apply_gravity(this->GetParent()->GetWorld()->gravity);
    integrate(dt);
}


void Rigidbody::integrate(double dt) {
    acceleration = force * invMass;
    GetParent()->transform.position += velocity * dt + acceleration * 0.5 * dt * dt;
    velocity += acceleration * dt;
    force.Zero();
    torque.Zero();
}
