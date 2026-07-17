//
// Created by yaly on 08/07/2026.
//
#include <algorithm>
#include <unordered_set>

#include "Collider.h"
#include "Quaternion.h"
#include "GameObject.h"
#include <iostream>


Collider::Collider(bool is_trigger) : isTrigger(is_trigger), other(nullptr) {

}

std::pair<Vector3, Vector3> Collider::GetAabb() {
    if (!GetParent()->cache.aabbDirty) {
        return {cachedMin, cachedMax};
    }
    if (GetParent()->cache.rotationDirty) {
        auto abs_rot = GetQuaternion().ToMatrix3Abs(&GetParent()->cache);

        // Compute world extents
        auto worldHalf = halfSize.MatrixMultiplication(abs_rot);
        cachedMin = GetPosition() - worldHalf;
        cachedMax = GetPosition() + worldHalf;
        // AABB min/max
        GetParent()->cache.aabbDirty = false;
        return {GetPosition() - worldHalf, GetPosition() + worldHalf};
    }
    cachedMin = GetPosition() - halfSize;
    cachedMax = GetPosition() + halfSize;
    GetParent()->cache.aabbDirty = false;
    return {GetPosition() - halfSize, GetPosition() + halfSize};
}

std::vector<
    std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>
>
Collider::SweepAndPrune(
    const std::list<std::shared_ptr<Collider>>& colliders
) {
    struct Endpoint {
        double position;
        bool isStart;
        std::shared_ptr<Collider> collider;
    };

    std::vector<Endpoint> endpoints;
    endpoints.reserve(colliders.size() * 2);

    // Build X-axis endpoints
    for (const auto& collider : colliders) {
        if (!collider) {
            continue;
        }

        const auto [minVector, maxVector] = collider->GetAabb();

        endpoints.push_back({
            minVector.x,
            true,
            collider
        });

        endpoints.push_back({
            maxVector.x,
            false,
            collider
        });
    }

    // Sort by X position.
    // When positions are equal, process starts before ends so touching
    // colliders are treated as overlapping.
    std::sort(
        endpoints.begin(),
        endpoints.end(),
        [](const Endpoint& a, const Endpoint& b) {
            if (a.position != b.position) {
                return a.position < b.position;
            }

            return a.isStart && !b.isStart;
        }
    );

    std::unordered_set<std::shared_ptr<Collider>> active;

    std::vector<
        std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>
    > candidatePairs;

    // Sweep along the X axis
    for (const auto& endpoint : endpoints) {
        const auto& collider = endpoint.collider;

        if (endpoint.isStart) {
            const auto [minA, maxA] = collider->GetAabb();

            for (const auto& other : active) {
                const auto [minB, maxB] = other->GetAabb();

                const bool overlapY =
                    minA.y <= maxB.y &&
                    maxA.y >= minB.y;

                const bool overlapZ =
                    minA.z <= maxB.z &&
                    maxA.z >= minB.z;

                if (overlapY && overlapZ) {
                    candidatePairs.emplace_back(collider, other);
                }
            }

            active.insert(collider);
        } else {
            active.erase(collider);
        }
    }

    return candidatePairs;
}


Quaternion Collider::GetQuaternion() const {
    return transform->quaternion * deltaTransform.quaternion;
}

Vector3 Collider::GetPosition() const {
    return transform->position + deltaTransform.position;
}

Vector3 Collider::GetSize() const {
    return transform->scale + deltaTransform.scale;
}

void Collider::attach(GameObject* obj) {
    transform = &obj->transform;
    halfSize = (obj->transform.scale + deltaTransform.scale) * 0.5;
}

ContactPoints Collider::CheckCollision(std::shared_ptr<Collider> collider2) {
    return {};
}

bool Collider::AabbCollision(std::shared_ptr<Collider> collider2) {
    return false;
}

std::optional<Collider::SatResult> Collider::Sat(std::shared_ptr<Collider> otherCollider) const {
    return std::nullopt;
}

ContactPoints Collider::GenerateContacts(SatResult&) const {
    ContactPoints contact_points;
    contact_points.contact_points.push_back(Vector3(1,1,1));
    return contact_points;
}


void Collider::HandleCollisionExit() {
    Collision collision(other, nullptr);
    if (!isTrigger) {
        if (stay || enter) {
            OnCollisionExit(collision);
        }
    }else{
        if (stay || enter) {
            OnTriggerExit(collision);
        }
    }

}

void Collider::CallCollisionEvent(
    const Collision& collision,
    void (Component::*function)(const Collision&)
) {
    for (const auto& component : GetParent()->GetComponents()) {
        // Skip this collider and every other Collider component
        if (dynamic_cast<Collider*>(component.get()) != nullptr)
            continue;

        (component.get()->*function)(collision);
    }
}

void Collider::OnCollisionEnter(const Collision &collision) {
    enter = true;
    CallCollisionEvent(collision, &Component::OnCollisionEnter);
}

void Collider::OnCollisionStay(const Collision &collision) {
    enter = false;
    stay = true;
    CallCollisionEvent(collision, &Component::OnCollisionStay);
}

void Collider::OnCollisionExit(const Collision& collision) {
    enter = false;
    stay = false;
    CallCollisionEvent(collision, &Component::OnCollisionExit);
}



void Collider::OnTriggerEnter(const Collision &collision) {
    enter = true;
    CallCollisionEvent(collision, &Component::OnTriggerEnter);
}

void Collider::OnTriggerStay(const Collision &collision) {
    enter = false;
    stay = true;
    CallCollisionEvent(collision, &Component::OnTriggerStay);
}

void Collider::OnTriggerExit(const Collision& collision) {
    enter = false;
    stay = false;
    CallCollisionEvent(collision, &Component::OnTriggerExit);
}

void Collider::HandleCollisionEvents(Collider* otherCollider, ContactPoints* result) {
    bool same_collider = (other.get() == otherCollider);
    other = std::shared_ptr<Collider>(otherCollider);
    Collision collision = Collision(other, result);
    if (!isTrigger) {
        if ((enter || stay) & same_collider){
            OnCollisionStay(collision);
        }else {
            OnCollisionEnter(collision);
        }
    }else {
        if ((enter || stay) & same_collider) {
            OnTriggerStay(collision);
        }else {
            OnTriggerEnter(collision);
        }
    }


}



