//
// Created by yaly on 29/06/2026.
//

#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "World.h"


GameObject::GameObject(Vector3 position, Vector3 rotation, Vector3 scale,
                       const std::vector<GameObject*>& children, std::string name) : transform(cache, position, rotation, scale), children(children), name(name) {}


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

GameObject* GameObject::CopyHierarchy(const GameObject* original,GameObjectMap& objectMap){
    auto* copied = new GameObject(
        original->transform.position,
        original->transform.rotation,
        original->transform.scale,
        {},
        original->name
    );

    objectMap.emplace(original, copied);

    for (Component* component : original->components) {
        Component* copiedComponent = component->Copy();

        copied->components.push_back(copiedComponent);
        copiedComponent->SetParent(copied);

        // Do not call attach() yet.
    }

    for (GameObject* child : original->children) {
        GameObject* copiedChild = CopyHierarchy(child, objectMap);

        copied->children.push_back(copiedChild);
        copiedChild->parent = copied;
    }

    return copied;
}

void GameObject::RemapHierarchyReferences(
    GameObject* object,
    const GameObjectMap& objectMap)
{
    if (object == nullptr) {
        return;
    }

    for (Component* component : object->components) {
        if (component != nullptr) {
            component->RemapReferences(objectMap);
        }
    }

    for (GameObject* child : object->children) {
        RemapHierarchyReferences(child, objectMap);
    }
}

void GameObject::RemapHierarchy(
    GameObject* object,
    const GameObjectMap& objectMap)
{
    for (Component* component : object->components) {
        component->RemapReferences(objectMap);
    }

    for (GameObject* child : object->children) {
        RemapHierarchy(child, objectMap);
    }
}

void GameObject::AddComponentWithoutAttach(Component *component) {
    components.push_back(component);
    component->SetParent(this);
}
void GameObject::AttachHierarchy(GameObject* object)
{
    for (Component* component : object->components) {
        component->attach(*object);
    }

    for (GameObject* child : object->children) {
        AttachHierarchy(child);
    }
}

void GameObject::SetDefaultQuaternion() {
}


GameObject* GameObject::DeepCopy() const {
    GameObjectMap objectMap;

    GameObject* result = CopyHierarchy(this, objectMap);
    RemapHierarchy(result, objectMap);
    AttachHierarchy(result);

    return result;
}

void GameObject::AddChild(GameObject *child) {
    if (world) {
        world->AddChild(child);
    }
    children.push_back(child);
    child->parent = this;
    child->transform.SetParentTransform(&transform);
}

void GameObject::SetDefault() {
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









