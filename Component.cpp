//
// Created by yaly on 30/06/2026.
//
#include <typeinfo>
#include <iostream>

#include "Component.h"

void Component::Start() {
}

void Component::Update(double dt) {
}

void Component::PhysicsUpdateFirstIteration(double dt) {
}

void Component::PhysicsUpdate(double dt) {
}

std::string Component::attach(GameObject *obj) {
    std::string name = typeid(*this).name();

    while (!name.empty() && std::isdigit(name[0])) {
        name.erase(name.begin());
    }
    return name;
}


