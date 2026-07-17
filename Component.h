//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_COMPONENT_H
#define BERESHITCORE_COMPONENT_H
#include <stdexcept>
#include "Collision.h"

class GameObject;  // forward declaration

class Component {
    private:
        GameObject* parent = nullptr;
        std::string name = "Component";
        void OnConstructed();
public:
    virtual std::string GetName() const {
        return name;
    }
    void SetName(std::string name) {
        this->name = name;
    }
    virtual ~Component() = default;

    bool Active = false;

    virtual void Start();
    virtual void Update(double dt);
    virtual void PhysicsUpdateFirstIteration(double dt);
    virtual void PhysicsUpdate(double dt);
    virtual void attach(GameObject* obj);
    virtual std::string GetTypeName() const;

    virtual void OnCollisionEnter(const Collision&) {}
    virtual void OnCollisionStay(const Collision&) {}
    virtual void OnCollisionExit(const Collision&) {}

    virtual void OnTriggerEnter(const Collision&) {}
    virtual void OnTriggerStay(const Collision&) {}
    virtual void OnTriggerExit(const Collision&) {}

    void SetParent(GameObject* obj) {
        if (parent != nullptr) {
            throw std::runtime_error("parent already set");
        }
        parent = obj;
    };
    GameObject* GetParent() const {
        return parent;
    }
};


#endif //BERESHITCORE_COMPONENT_H
