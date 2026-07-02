//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Transform.h"
GameObject::GameObject() : transform(Transform()) {}

GameObject::GameObject(Vector3 position, Vector3 rotation, Vector3 scale, std::list<GameObject*> children) : transform(position, rotation, scale), children(children) {}

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









