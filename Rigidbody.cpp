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
    apply_gravity(this->GetParent()->getWorld()->gravity);
}
