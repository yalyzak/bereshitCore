//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Transform.h"


GameObject::GameObject(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale,
    const std::list<GameObject *> children) : transform(position, rotation, scale), children(children) {};

GameObject* GameObject::AddComponent(std::shared_ptr<Component> comp) {
    components.push_back(comp);
    comp->SetParent(this);
    return this;
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

std::list<std::shared_ptr<Component>> GameObject::search_by_component(std::string name) {
    std::list<std::shared_ptr<Component>> results;

    for (auto& component : components) {
        if (component->name == name) {
            results.push_back(component);
        }
    }

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}









