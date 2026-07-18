//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_COLLIDER_H
#define BERESHITCORE_COLLIDER_H

#include <memory>
#include <list>
#include <utility>
#include <vector>
#include <optional>
#include <variant>

#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Quaternion.h"

class Collider : public Component{
    public:
        enum class Source {
            a,
            b,
            c
        };
        struct SatResult {
            Vector3 normal;
            double penetration;
            Source type;
            std::variant<int, std::pair<int, int>> collisionAxisIndices;
            const Collider& A;
            const Collider& B;
        };

        struct Axes {
            Source source;
            std::variant<int, std::pair<int, int>> indices;
            Vector3 axis;
        };

    private:
        bool isTrigger;
        Transform* transform;
        Vector3 halfSize = Vector3();
        Transform deltaTransform;
        bool enter;
        bool stay;
        std::shared_ptr<Collider> other;
        Vector3 cachedMin;
        Vector3 cachedMax;



        virtual std::optional<SatResult> Sat(std::shared_ptr<Collider> otherCollider) const;

public:


    bool GetIsTrigger() {
        return isTrigger;
    }

    Collider(bool is_trigger =false);
    std::pair<Vector3, Vector3> GetAabb();
    static std::vector<std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>> SweepAndPrune(const std::list<std::shared_ptr<Collider>>& colliders);
    Quaternion GetQuaternion() const;
    Vector3 GetPosition() const;
    Vector3 GetSize() const;
    void attach(GameObject& obj) override;


    virtual ContactPoints CheckCollision(std::shared_ptr<Collider> collider2);
    virtual bool AabbCollision(std::shared_ptr<Collider> collider2);
    virtual ContactPoints GenerateContacts(SatResult&) const;
    virtual void HandleCollisionExit();
    virtual void CallCollisionEvent(const Collision& collision, void (Component::*func)(const Collision&));
    virtual void OnCollisionEnter(const Collision& collision);
    virtual void OnCollisionStay(const Collision& collision);
    virtual void OnCollisionExit(const Collision& collision);
    virtual void OnTriggerEnter(const Collision& collision);
    virtual void OnTriggerStay(const Collision& collision);
    virtual void OnTriggerExit(const Collision& collision);
    virtual void HandleCollisionEvents(Collider*, ContactPoints*);


};


class Vector3;
#endif //BERESHITCORE_COLLIDER_H
