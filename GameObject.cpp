//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "World.h"


GameObject::GameObject(Vector3 position, Vector3 rotation, Vector3 scale,
                       const std::list<GameObject*>& children, std::string name) : transform(position, rotation, scale), children(children), name(name) {}

GameObject* GameObject::AddComponent(std::shared_ptr<Component> comp) {
    components.push_back(comp);
    comp->SetParent(this);
    comp->attach(*this);
    std::string comp_name = comp->GetName();
    return this;
}

GameObject * GameObject::AddComponent(Component comp) {
    auto newComponent = std::make_shared<Component>(comp);
    components.push_back(newComponent);
    newComponent->SetParent(this);
    newComponent->attach(*this);
    std::string comp_name = newComponent->GetName();
    return this;
}

// GameObject * GameObject::AddComponent(std::list<std::shared_ptr<Component>> comp) {
//     for (auto& component : comp) {
//         AddComponent(component);
//     }
// }



std::list<GameObject *> GameObject::GetAllChildren() {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->GetAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}


void GameObject::GetAllChildrenPhysics(std::vector<GameObject*>& result) {
    for (GameObject* child : children) {
        if (child->isPhysicsObject()) {
            result.push_back(child);
        }

        child->GetAllChildrenPhysics(result);
    }
}

void GameObject::GetAllChildrenColliders(std::vector<Collider *> &result) {
    for (GameObject* child : children) {
        Collider* collider = child->GetComponent<Collider>();
        if (collider != nullptr) {
            result.push_back(collider);
        }

        child->GetAllChildrenColliders(result);
    }
}

void GameObject::GetAllChildrenJoints(std::vector<Joint *> &result) {
    for (GameObject* child : children) {
        Joint* joint = child->GetComponent<Joint>();
        if (joint != nullptr) {
            result.push_back(joint);
        }
        child->GetAllChildrenJoints(result);
    }
}

void GameObject::GetAllChildren(std::vector<GameObject *> &result) {
    for (GameObject* child : children) {
        result.push_back(child);
        child->GetAllChildren(result);
    }
}

GameObject GameObject::DeepCopy() const {
    GameObject obj(
        transform.position,
        transform.rotation,
        transform.scale,
        {},
        name
    );

    for (const auto& component : components) {
        obj.AddComponent(component->Copy());
    }

    for (const GameObject* child : children) {
        if (child != nullptr) {
            auto* copiedChild = new GameObject(child->DeepCopy());
            obj.AddChild(copiedChild);
        }
    }

    return obj;
}

void GameObject::AddChild(GameObject *child) {
    world->AddChild(child);
    children.push_back(child);
}

std::list<GameObject*> GameObject::search_by_component(std::string name) {
    std::list<GameObject*> results;

    for (auto& component : components) {
        if (component->GetName() == name) {
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
        if (comp && comp->GetName() == name) {
            return comp;
        }
    }
    return nullptr;
}



const std::list<std::shared_ptr<Component>>& GameObject::GetComponents() const {
    return components;
}









