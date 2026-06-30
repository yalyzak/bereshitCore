//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Transform.h"
GameObject::GameObject() : transform(Transform()) {}

void GameObject::AddComponent(Component* comp) {
    components.push_back(comp);
    comp->SetParent(this);
}








