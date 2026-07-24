//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "World.h"


GameObject::GameObject(Vector3 position, Vector3 rotation, Vector3 scale,
                       const std::vector<GameObject*>& children, std::string name) : transform(position, rotation, scale), children(children), name(name) {}


GameObject* GameObject::AddComponent(Component* comp) {
    components.push_back(comp);
    comp->SetParent(this);
    comp->attach(*this);
    std::string comp_name = comp->GetName();
    return this;
}


std::vector<GameObject*> GameObject::GetAllChildren() {
    std::vector<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);
        child->GetAllChildren(allChildren);
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

std::vector<GameObject *> GameObject::SearchByComponent(const std::string &name) {
    std::vector<GameObject*> results;

    for (auto& component : components) {
        if (component->GetName() == name) {
            results.push_back(this);
            break; // prevent duplicates
        }
    }

    for (GameObject* child : children) {
        child->search_by_component(name, results);
    }

    return results;

}

std::vector<GameObject *> GameObject::SearchByName(const std::string &name) {
    std::vector<GameObject*> results;

    for (GameObject* child : children) {
        if (child->name == name) {
            results.push_back(child);
        }
        child->_SearchByName(name, results);
    }

    return results;
}

void GameObject::search_by_component(const std::string &name, std::vector<GameObject *> &results) {
    for (auto& component : components) {
        if (component->GetName() == name) {
            results.push_back(this);
            break; // prevent duplicates
        }
    }

    for (GameObject* child : children) {
        auto child_results = child->SearchByComponent(name);
        child->search_by_component(name, results);
    }

}

void GameObject::_SearchByName(const std::string &name, std::vector<GameObject *> &results) {
    for (const auto& child : children) {
        if (child->name == name) {
            results.push_back(child);
        }
        child->_SearchByName(name, results);
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
        auto* copyComponent = component->Copy();
        if (copyComponent != nullptr) {
            obj.AddComponent(copyComponent);
        }
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
    if (world) {
        world->AddChild(child);
    }
    children.push_back(child);
    child->parent = this;
}


Component* GameObject::GetComponent(const std::string& name) {
    for (auto* comp : components) {
        if (comp->GetName() == name) {
            return comp;
        }
    }
    return nullptr;
}



const std::vector<Component*>& GameObject::GetComponents() const {
    return components;
}









