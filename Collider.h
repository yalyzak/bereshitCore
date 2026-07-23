//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_COLLIDER_H
#define BERESHITCORE_COLLIDER_H

#include <memory>
#include <utility>
#include <vector>
#include <optional>
#include <variant>

#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "RayCastHit.h"

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
    protected:
        mutable bool isTrigger;
        Transform* transform;
        Vector3 halfSize = Vector3();
        Transform deltaTransform;
        mutable bool enter;
        mutable bool stay;
        const Collider* other;
        mutable Vector3 cachedMin;
        mutable Vector3 cachedMax;
    private:




        virtual std::optional<SatResult> Sat(const Collider* otherCollider) const;

public:


    bool GetIsTrigger() const {
        return isTrigger;
    }

    Collider(bool is_trigger =false);
    std::pair<Vector3, Vector3> GetAabb() const;
    static std::vector<std::pair<Collider*,Collider*>> SweepAndPrune(const std::vector<Collider*>& colliders);
    Quaternion GetQuaternion() const;
    Vector3 GetPosition() const;
    Vector3 GetSize() const;
    void attach(GameObject& obj) override;


    virtual ContactPoints CheckCollision(const Collider* collider2);
    virtual bool AabbCollision(const Collider* collider2);
    virtual ContactPoints GenerateContacts(SatResult&) const;
    virtual void HandleCollisionExit() const;
    virtual void CallCollisionEvent(const Collision& collision, void (Component::*func)(const Collision&)) const;
    virtual void OnCollisionEnter(const Collision& collision) const;
    virtual void OnCollisionStay(const Collision& collision) const;
    virtual void OnCollisionExit(const Collision& collision) const;
    virtual void OnTriggerEnter(const Collision& collision) const;
    virtual void OnTriggerStay(const Collision& collision) const;
    virtual void OnTriggerExit(const Collision& collision) const;
    virtual void HandleCollisionEvents(Collider*, ContactPoints*);
    virtual RayCastHit RayCast(const Vector3 &origin, const Vector3 &direction, double maxDistance) const;

    virtual Collider* Copy() const override;
};


class Vector3;
#endif //BERESHITCORE_COLLIDER_H
