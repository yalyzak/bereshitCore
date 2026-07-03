//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_GAMEOBJECT_H
#define BERESHITCORE_GAMEOBJECT_H

#include "Transform.h"
#include "Component.h"
#include "World.h"
#include <list>
#include <memory>

class GameObject {
    private:
        World* world = nullptr;
        GameObject* parent = nullptr;
    public:
        Transform transform;
        std::string name;
        std::list<GameObject*> children;

        void setWorld(World* p) {
            if (world != nullptr) {
                throw std::runtime_error("World already set");
            }
            world = p;
        }

        World* GetWorld() const {
            return world;
        }


        std::list<std::shared_ptr<Component>> components;
        GameObject(const Vector3& position = Vector3(), const Vector3& rotation = Vector3(), const Vector3& scale = Vector3(), const std::list<GameObject*> children= {});

        GameObject* AddComponent(std::shared_ptr<Component> comp);
        void setParent(GameObject* p) {
            parent = p;
        };

    std::list<GameObject*> getAllChildren();
    std::list<Component*> search_by_component(std::string name);
};


#endif //BERESHITCORE_GAMEOBJECT_H
