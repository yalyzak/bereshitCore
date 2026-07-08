//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_COMPONENT_H
#define BERESHITCORE_COMPONENT_H
#include <stdexcept>
class GameObject;  // forward declaration

class Component {
    private:
        GameObject* parent = nullptr;
public:
    virtual std::string GetName() const {
        return "Component";
    }
    virtual ~Component() = default;

    bool Active = false;

    virtual void Start();
    virtual void Update(double dt);
    virtual void PhysicsUpdateFirstIteration(double dt);
    virtual void PhysicsUpdate(double dt);
    virtual std::string attach(GameObject* obj);


    void SetParent(GameObject* obj) {
        if (parent != nullptr) {
            throw std::runtime_error("World already set");
        }
        parent = obj;
    };
    GameObject* GetParent() {
        return parent;
    }
};


#endif //BERESHITCORE_COMPONENT_H
