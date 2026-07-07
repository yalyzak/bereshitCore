//
// Created by yaly on 30/06/2026.
//

#include <iostream>
#include <exception>

#include "World.h"
#include "GameObject.h"
#include "Rigidbody.h"



World::World(std::list<GameObject*> children) : children(children) {
    for (auto i = children.begin(); i != children.end(); ++i) {
        GameObject* child = *i;
        child->setWorld(this);
    }
}

World::World(bool *running_flag, std::list<GameObject *> children, GameObject* gizmos, Vector3 gravity,
    double tick, double speed, int physics_epochs) : gravity(gravity), tick(tick), physics_epochs(physics_epochs), speed(speed), gizmos(gizmos), children(children) {
}

std::list<GameObject*> World::getAllChildren() const {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->getAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}

std::list<GameObject *> World::getAllChildrenPhysics() const {
    std::list<GameObject *> objects;
    for (GameObject* child : getAllChildren()) {
        if (child->GetComponent("Rigidbody"))
        {
            objects.push_back(child);
        }
    }
    return objects;
}

std::list<GameObject*> World::search_by_component(std::string name) const{
    std::list<GameObject*> results;

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}

void World::Start() {
    auto children = getAllChildren();
    for (GameObject* child : children) {
        for (auto component : child->GetComponents()) {
            component->Start();
        }
    }
}

void World::CallChildrenUpdate(const std::list<GameObject *> &list, double dt) {
    for (auto child : list) {
        for (auto component : child->GetComponents()) {
            try {
                component->Update(dt);
            }
            catch (const std::exception& e) {
                std::cerr << "Error in component Update: " << e.what() << std::endl;
            }
            catch (...) {
                std::cerr << "Unknown error in component Update" << std::endl;
            }
        }
    }
}

void World::CallChildrenPhysicsUpdate(const std::list<GameObject *> &list, double dt) {
    for (auto child : children) {
        for (auto component : child->GetComponents()) {
            component->PhysicsUpdate(dt);
        }
    }
}

void World::Update(bool updateComponen = false) {
    double dt = tick;
    bool FirstIteration = true;
    auto allchildren = getAllChildren();
    if (updateComponen) {
        CallChildrenUpdate(allchildren, dt);
    }
    CallChildrenPhysicsUpdate(allchildren, dt);
    // auto children = getAllChildrenPhysics();







}
