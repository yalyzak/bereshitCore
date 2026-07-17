//
// Created by yaly on 30/06/2026.
//
#include <typeinfo>
#include <iostream>

#include "Component.h"

void Component::OnConstructed() {
}

void Component::Start() {
}

void Component::Update(double dt) {
}

void Component::PhysicsUpdateFirstIteration(double dt) {
}

void Component::PhysicsUpdate(double dt) {
}

void Component::attach(GameObject* obj) {
    SetName(GetTypeName());
}

std::string Component::GetTypeName() const {
    std::string name = typeid(*this).name();

    while (!name.empty() &&
           std::isdigit(static_cast<unsigned char>(name.front()))) {
        name.erase(name.begin());
           }

    return name;
}




