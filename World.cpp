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
