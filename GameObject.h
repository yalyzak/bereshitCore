//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_GAMEOBJECT_H
#define BERESHITCORE_GAMEOBJECT_H


#include <list>
#include <memory>


#include "Transform.h"
#include "Component.h"
class World;
#include "Cache.h"


class GameObject {
    private:
        World* world = nullptr;
        GameObject* parent = nullptr;
        std::list<GameObject*> children;
        std::list<std::shared_ptr<Component>> components;

    public:
        Transform transform;
        std::string name;
        Cache cache;


        void setWorld(World* p) {
            if (world != nullptr) {
                throw std::runtime_error("World already set");
            }
            world = p;
        }

        World* GetWorld() const {
            return world;
        }


        GameObject(const Vector3& position = Vector3(), const Vector3& rotation = Vector3(), const Vector3& scale = Vector3(), const std::list<GameObject*> children= {});

        GameObject* AddComponent(std::shared_ptr<Component> comp);
        // GameObject* AddComponent(std::list<std::shared_ptr<Component>> comp);

        void setParent(GameObject* p) {
            parent = p;
        };

    std::list<GameObject*> GetChildren() {
        return children;
    };
    std::list<GameObject*> GetAllChildren();
    std::list<GameObject*> search_by_component(std::string name);
    std::shared_ptr<Component> GetComponent(const std::string& name);
    template<typename T>
    std::shared_ptr<T> GetComponent1() const {
        for (const auto& component : components) {
            if (auto casted = std::dynamic_pointer_cast<T>(component)) {
                return casted;
            }
        }
        return nullptr;
    }
    template<typename T>
    T* GetComponent() const {
        for (const auto& component : components) {
            if (auto casted = std::dynamic_pointer_cast<T>(component)) {
                return casted.get();
            }
        }
        return nullptr;
    }
    const std::list<std::shared_ptr<Component>>& GetComponents() const;
    void GetAllChildrenPhysics(std::vector<GameObject*>& result);
    void GetAllChildren(std::vector<GameObject*>& result);

};




#endif //BERESHITCORE_GAMEOBJECT_H
