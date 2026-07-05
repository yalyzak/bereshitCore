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
    std::string comp_name = comp->attach(this);
    return this;
}

GameObject * GameObject::AddComponent(std::list<std::shared_ptr<Component>> comp) {
    for (auto& component : comp) {
        AddComponent(component);
    }
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

std::list<GameObject*> GameObject::search_by_component(std::string name) {
    std::list<GameObject*> results;

    for (auto& component : components) {
        if (component->name == name) {
            results.push_back(this);
            break; // prevent duplicates
        }
    }

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& name) {
    for (auto& comp : components) {
        if (comp && comp->name == name) {
            return comp;
        }
    }
    return nullptr;
}

const std::list<std::shared_ptr<Component>>& GameObject::GetComponents() const {
    return components;
}









