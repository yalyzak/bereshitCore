//
// Created by yaly on 30/06/2026.
//

#include "World.h"
#include "GameObject.h"



World::World(std::list<GameObject*> children) : children(children) {
    for (auto i = children.begin(); i != children.end(); ++i) {
        GameObject* child = *i;
        child->setWorld(this);
    }
}

World::World(bool *running_flag, std::list<GameObject *> children, GameObject* gizmos, Vector3 gravity,
    double tick, double speed, int physics_epochs) : gravity(gravity), tick(tick), physics_epochs(physics_epochs), speed(speed), gizmos(gizmos), children(children) {
}

std::list<GameObject*> World::getAllChildren() {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->getAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}

std::list<std::shared_ptr<Component>>World::search_by_component(std::string name) {
    std::list<std::shared_ptr<Component>> results;

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}
