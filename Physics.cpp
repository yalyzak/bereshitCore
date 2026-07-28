//
// Created by yaly on 20/07/2026.
//

#include "Physics.h"
#include "Collider.h"
#include "World.h"

RayCastHit Physics::RayCast(const Vector3 &origin, const Vector3 &direction, const Collider *layerMask, float maxDistance) {
        RayCastHit hit;
        if (layerMask != nullptr) {
            hit = layerMask->RayCast(origin, direction, maxDistance);
        }else {
            float dis = std::numeric_limits<float>::max();
            for (auto& collider : Physics::world->GetAllColliders()) {
               RayCastHit tempHit = collider->RayCast(origin, direction, maxDistance);
               if (tempHit.collider != nullptr && tempHit.distance < dis) {
                   dis = tempHit.distance;
                   hit = tempHit;
               }
            }

        }

        return hit;

}
