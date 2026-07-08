//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_WORLD_H
#define BERESHITCORE_WORLD_H

#include <list>
#include <memory>
#include <cstdlib>

#include "Vector3.h"
#include "Component.h"
#include "Contact.h"


class GameObject;

class World {
    private:
        std::list<GameObject*> children;
        GameObject* gizmos;


    public:
        double tick;
        double speed;
        int physics_epochs;
        Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
        World(bool* running_flag, std::list<GameObject*> children, GameObject* gizmos, Vector3 gravity, double tick, double speed, int physics_epochs);
        std::list<GameObject*> getChildren() {
            return children;
        };

        std::list<GameObject*> getAllChildren() const;
        std::list<GameObject*> getAllChildrenPhysics() const;
        std::list<GameObject*> search_by_component(std::string name) const;

        void Start();
        void Exit() {
            std::exit(0);
        }

        void CallChildrenUpdate(const std::list<GameObject *> & list, double dt);

        void CallChildrenPhysicsUpdate(const std::list<GameObject *> & list, double dt);

        [[nodiscard]] std::list<Contact> SolveCollectionsFirstIteration(const std::list<GameObject *> & list, double dt) const;

        void Update(bool updateComponen);
        virtual void UpdatePython(const std::list<GameObject*>*);



};


#endif //BERESHITCORE_WORLD_H
