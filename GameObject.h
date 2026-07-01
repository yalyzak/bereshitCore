//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_GAMEOBJECT_H
#define BERESHITCORE_GAMEOBJECT_H

#include "Transform.h"
#include "Component.h"
#include "World.h"
#include <list>

class GameObject {
    private:
        World* world = nullptr;
        GameObject* parent = nullptr;
    public:
        std::string name;
        std::list<GameObject*> children;

        void setWorld(World* p) {
            if (world != nullptr) {
                throw std::runtime_error("World already set");
            }
            world = p;
        }

        World* getWorld() const {
            return world;
        }


        Transform transform;
        std::list<Component*> components;
        GameObject();
        void AddComponent(Component* comp);
        void setParent(GameObject* p) {
            parent = p;
        };

    std::list<GameObject*> getAllChildren();
};


#endif //BERESHITCORE_GAMEOBJECT_H
