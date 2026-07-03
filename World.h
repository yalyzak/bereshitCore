//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_WORLD_H
#define BERESHITCORE_WORLD_H

#include "Vector3.h"
#include "Component.h"
#include <list>
#include <memory>
class GameObject;

class World {
    private:
        std::list<GameObject*> children;
        GameObject* gizmos;
        double tick;
        double speed;
        int physics_epochs;

    public:
        Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
        World(std::list<GameObject*> children);
        World(bool* running_flag, std::list<GameObject*> children, GameObject* gizmos, Vector3 gravity, double tick, double speed, int physics_epochs);
        std::list<GameObject*> getChildren() {
            return children;
        };

        std::list<GameObject*> getAllChildren();
        std::list<GameObject*> search_by_component(std::string name);

};


#endif //BERESHITCORE_WORLD_H
