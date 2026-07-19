//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_WORLD_H
#define BERESHITCORE_WORLD_H

#include <list>
#include <memory>
#include <cstdlib>
#include <array>

#include "Vector3.h"
#include "Component.h"
#include "Contact.h"
#include "GameObject.h"


class GameObject;

class World {
    private:
        std::list<GameObject*> children;
        GameObject* gizmos;
        bool allChildrenDirty = true;
        bool physicsChildrenDirty = true;
        bool physicsCollidersDirty = true;
        std::vector<GameObject*> cacheAllChildren;
        std::vector<GameObject*> cachePhysicsChildren;
        std::vector<std::shared_ptr<Collider>> cachePhysicsColliders;
        void GetAllChildren(std::vector<GameObject*>& result);
        void GetAllChildrenPhysics(std::vector<GameObject*>& result);
        void GetAllPhysicsColliders(std::vector<std::shared_ptr<Collider>>& result);
        void SetCacheAllChildren();
        void SetCachePhysicsChildren();
        void SetCachePhysicsColliders();
        void SetCache();





    public:
        double tick = 1/60.0;
        double speed = 1;
        int physics_epochs = 10;
        Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);

        World(bool* running_flag, std::list<GameObject*> children, GameObject* gizmos, Vector3 gravity, double tick, double speed, int physics_epochs);
        std::list<GameObject*> getChildren() {
            return children;
        };
        void AddChild(GameObject *child);

        std::vector<GameObject*>& getAllChildren();
        std::vector<GameObject*>& GetAllChildrenPhysics();
        std::vector<std::shared_ptr<Collider>>& GetAllPhysicsColliders();
        std::list<GameObject*> getGizmos() const;
        std::list<GameObject*> search_by_component(std::string name) const;
        void SolveCollections(const std::vector<Contact>&, double);

        void Start();
        void Exit() {
            std::exit(0);
        }

        void CallChildrenUpdate(const std::vector<GameObject *> & list, double dt, void (Component::*func)(double));

        [[nodiscard]] std::vector<Contact> SolveCollectionsFirstIteration(const std::vector<std::shared_ptr<Collider>>& colliders, double dt) const;

        void Update(bool updateComponen = false);
        void SetGizmos(const std::list<Contact>& );



};


#endif //BERESHITCORE_WORLD_H
