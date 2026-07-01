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

std::list<GameObject*> World::getAllChildren() {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->getAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}
