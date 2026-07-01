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

std::list<GameObject *> GameObject::getAllChildren() {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->getAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}









