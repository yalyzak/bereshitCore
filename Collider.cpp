//
// Created by yaly on 08/07/2026.
//
#include <algorithm>
#include <unordered_set>

#include "Collider.h"
#include "Quaternion.h"
#include "GameObject.h"


Collider::Collider(const Transform*, bool is_trigger) : transform(transform), isTrigger(is_trigger){
}

std::pair<Vector3, Vector3> Collider::GetAabb() {
    if (!GetParent()->cache.aabbDirty) {
        return {cachedMin, cachedMax};
    }
    if (GetParent()->cache.rotationDirty) {
        auto abs_rot = GetQuaternion().ToMatrix3Abs(&GetParent()->cache);

        // Compute world extents
        auto world_half = halfSize.MatrixMultiplication(*abs_rot);
        cachedMin, cachedMax = GetPosition() - world_half, GetPosition() + world_half;
        // AABB min/max
        GetParent()->cache.aabbDirty = false;
        return {GetPosition() - world_half, GetPosition() + world_half};
    }
    cachedMin, cachedMax = GetPosition() - halfSize, GetPosition() + halfSize;
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


Quaternion Collider::GetQuaternion() {
    return transform->quaternion * deltaTransform.quaternion;
}

Vector3 Collider::GetPosition() {
    return transform->position + deltaTransform.position;
}

Vector3 Collider::GetSize() {
    return transform->scale + deltaTransform.scale;
}



