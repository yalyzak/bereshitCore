//
// Created by yaly on 30/06/2026.
//

#include "Component.h"
// #include "GameObject.h"
void Component::Start() {
}

void Component::Update(double dt) {
}

void Component::PhysicsUpdate(double dt) {
}

std::string Component::attach(GameObject *obj) {
    return "Component";
}


